from __future__ import annotations

import argparse
import json
import math
import platform
import sys
import tempfile
import time
from contextlib import nullcontext
from dataclasses import asdict, dataclass
from datetime import datetime, timezone
from pathlib import Path

import pymembrane as mb


EXPECTED_COUNTS = {
    0: (12, 20, 30),
    1: (42, 80, 120),
    2: (162, 320, 480),
    3: (642, 1280, 1920),
    4: (2562, 5120, 7680),
}


@dataclass
class BenchmarkRow:
    subdivision: int
    vertices: int
    faces: int
    edges: int
    workflow: str
    operations: int
    wall_s: float
    rate: float
    rate_units: str


def _normalize(point: tuple[float, float, float], radius: float) -> tuple[float, float, float]:
    norm = math.sqrt(sum(value * value for value in point))
    scale = radius / norm
    return (point[0] * scale, point[1] * scale, point[2] * scale)


def _midpoint(
    vertices: list[tuple[float, float, float]],
    cache: dict[tuple[int, int], int],
    i: int,
    j: int,
    radius: float,
) -> int:
    key = (i, j) if i < j else (j, i)
    if key in cache:
        return cache[key]
    p = vertices[i]
    q = vertices[j]
    midpoint = ((p[0] + q[0]) * 0.5, (p[1] + q[1]) * 0.5, (p[2] + q[2]) * 0.5)
    vertices.append(_normalize(midpoint, radius))
    cache[key] = len(vertices) - 1
    return cache[key]


def make_icosphere(
    subdivision: int,
    radius: float = 1.0,
) -> tuple[list[tuple[float, float, float]], list[tuple[int, int, int]]]:
    phi = (1.0 + math.sqrt(5.0)) / 2.0
    vertices = [
        _normalize((-1.0, phi, 0.0), radius),
        _normalize((1.0, phi, 0.0), radius),
        _normalize((-1.0, -phi, 0.0), radius),
        _normalize((1.0, -phi, 0.0), radius),
        _normalize((0.0, -1.0, phi), radius),
        _normalize((0.0, 1.0, phi), radius),
        _normalize((0.0, -1.0, -phi), radius),
        _normalize((0.0, 1.0, -phi), radius),
        _normalize((phi, 0.0, -1.0), radius),
        _normalize((phi, 0.0, 1.0), radius),
        _normalize((-phi, 0.0, -1.0), radius),
        _normalize((-phi, 0.0, 1.0), radius),
    ]
    faces = [
        (0, 11, 5),
        (0, 5, 1),
        (0, 1, 7),
        (0, 7, 10),
        (0, 10, 11),
        (1, 5, 9),
        (5, 11, 4),
        (11, 10, 2),
        (10, 7, 6),
        (7, 1, 8),
        (3, 9, 4),
        (3, 4, 2),
        (3, 2, 6),
        (3, 6, 8),
        (3, 8, 9),
        (4, 9, 5),
        (2, 4, 11),
        (6, 2, 10),
        (8, 6, 7),
        (9, 8, 1),
    ]

    for _ in range(subdivision):
        cache: dict[tuple[int, int], int] = {}
        refined_faces: list[tuple[int, int, int]] = []
        for a, b, c in faces:
            ab = _midpoint(vertices, cache, a, b, radius)
            bc = _midpoint(vertices, cache, b, c, radius)
            ca = _midpoint(vertices, cache, c, a, radius)
            refined_faces.extend(((a, ab, ca), (b, bc, ab), (c, ca, bc), (ab, bc, ca)))
        faces = refined_faces

    expected = EXPECTED_COUNTS.get(subdivision)
    if expected is not None:
        expected_vertices, expected_faces, _ = expected
        if len(vertices) != expected_vertices or len(faces) != expected_faces:
            raise ValueError(
                f"unexpected icosphere size at subdivision {subdivision}: "
                f"got {(len(vertices), len(faces))}, expected {(expected_vertices, expected_faces)}"
            )
    return vertices, faces


def edge_count_from_faces(faces: list[tuple[int, int, int]]) -> int:
    edges: set[tuple[int, int]] = set()
    for a, b, c in faces:
        for i, j in ((a, b), (b, c), (c, a)):
            edge = (i, j) if i < j else (j, i)
            edges.add(edge)
    return len(edges)


def write_mesh_files(
    directory: Path,
    subdivision: int,
    vertices: list[tuple[float, float, float]],
    faces: list[tuple[int, int, int]],
) -> tuple[Path, Path]:
    vertex_file = directory / f"vertices_sphere_s{subdivision}.inp"
    face_file = directory / f"faces_sphere_s{subdivision}.inp"
    with vertex_file.open("w", encoding="utf-8") as handle:
        for index, (x, y, z) in enumerate(vertices):
            handle.write(f"{index} {x:.17g} {y:.17g} {z:.17g} 1\n")
    with face_file.open("w", encoding="utf-8") as handle:
        for index, (a, b, c) in enumerate(faces):
            handle.write(f"{index} {a} {b} {c} 1 1\n")
    return vertex_file, face_file


def _build_system(vertex_file: Path, face_file: Path) -> tuple[mb.System, float]:
    box = mb.Box(4.0, 4.0, 4.0)
    system = mb.System(box)
    system.read_mesh_from_files(files={"vertices": str(vertex_file), "faces": str(face_file)})
    edge_lengths = system.compute.edge_lengths()
    avg_edge_length = sum(edge_lengths) / len(edge_lengths)
    return system, avg_edge_length


def _build_common_evolver(system: mb.System, avg_edge_length: float) -> mb.Evolver:
    evolver = mb.Evolver(system)
    evolver.add_force("Mesh>Harmonic", {"k": {"0": "350.0"}, "l0": {"0": str(avg_edge_length)}})
    evolver.add_force(
        "Mesh>Limit",
        {"lmin": {"0": str(0.7 * avg_edge_length)}, "lmax": {"0": str(1.3 * avg_edge_length)}},
    )
    evolver.add_force("Mesh>Bending>Dihedral", {"kappa": {"0": "1.0"}})
    return evolver


def _benchmark_workflow(
    subdivision: int,
    vertices: int,
    faces: int,
    edges: int,
    workflow: str,
    operations: int,
    units: str,
    func,
) -> BenchmarkRow:
    start = time.perf_counter()
    func()
    wall = time.perf_counter() - start
    rate = float(operations) / wall if wall > 0.0 else float("inf")
    return BenchmarkRow(
        subdivision=subdivision,
        vertices=vertices,
        faces=faces,
        edges=edges,
        workflow=workflow,
        operations=operations,
        wall_s=wall,
        rate=rate,
        rate_units=units,
    )


def run_one_subdivision(
    subdivision: int,
    steps: int,
    keep_meshes: bool,
    mesh_root: Path | None,
) -> list[BenchmarkRow]:
    rows: list[BenchmarkRow] = []

    if keep_meshes:
        assert mesh_root is not None
        mesh_dir = mesh_root / f"s{subdivision}"
        mesh_dir.mkdir(parents=True, exist_ok=True)
        tempdir_cm = None
    else:
        tempdir_cm = tempfile.TemporaryDirectory(prefix=f"pymembrane-size-s{subdivision}-")
        mesh_dir = Path(tempdir_cm.name)

    try:
        expected = EXPECTED_COUNTS.get(subdivision)
        if expected is None:
            raise ValueError(f"unsupported subdivision level {subdivision}")

        def mesh_generation() -> tuple[list[tuple[float, float, float]], list[tuple[int, int, int]], Path, Path]:
            vertices, faces = make_icosphere(subdivision)
            vertex_file, face_file = write_mesh_files(mesh_dir, subdivision, vertices, faces)
            return vertices, faces, vertex_file, face_file

        start = time.perf_counter()
        vertices, faces, vertex_file, face_file = mesh_generation()
        wall = time.perf_counter() - start
        edge_count = edge_count_from_faces(faces)
        if (len(vertices), len(faces), edge_count) != expected:
            raise ValueError(
                f"unexpected counts for subdivision {subdivision}: "
                f"got {(len(vertices), len(faces), edge_count)}, expected {expected}"
            )

        rows.append(
            BenchmarkRow(
                subdivision=subdivision,
                vertices=len(vertices),
                faces=len(faces),
                edges=edge_count,
                workflow="mesh_generation",
                operations=1,
                wall_s=wall,
                rate=(1.0 / wall) if wall > 0.0 else float("inf"),
                rate_units="meshes/s",
            )
        )

        def mc_workflow() -> None:
            system, avg_edge_length = _build_system(vertex_file, face_file)
            evolver = _build_common_evolver(system, avg_edge_length)
            evolver.add_integrator("Mesh>MonteCarlo>vertex>move", {"dr": "0.008"})
            evolver.set_global_temperature("1e-3")
            evolver.evolveMC(steps=steps)

        rows.append(
            _benchmark_workflow(
                subdivision,
                len(vertices),
                len(faces),
                edge_count,
                "mc_vertex_move",
                steps,
                "steps/s",
                mc_workflow,
            )
        )

        def bd_workflow() -> None:
            system, avg_edge_length = _build_system(vertex_file, face_file)
            evolver = _build_common_evolver(system, avg_edge_length)
            evolver.add_integrator("Mesh>Brownian>vertex>move", {"seed": "202208"})
            evolver.set_time_step("2e-3")
            evolver.set_global_temperature("1e-4")
            evolver.evolveMD(steps=steps)

        rows.append(
            _benchmark_workflow(
                subdivision,
                len(vertices),
                len(faces),
                edge_count,
                "brownian_dynamics",
                steps,
                "steps/s",
                bd_workflow,
            )
        )
        return rows
    finally:
        if tempdir_cm is not None:
            tempdir_cm.cleanup()


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Size-scaling example for PyMembrane.")
    parser.add_argument("--quick", action="store_true", help="run a small smoke example")
    parser.add_argument("--subdivisions", nargs="+", type=int, default=None, help="icosphere subdivision levels")
    parser.add_argument("--steps", type=int, default=None, help="workflow step count")
    parser.add_argument("--json", default=None, help="write results to JSON")
    parser.add_argument("--keep-meshes", action="store_true", help="keep generated mesh files on disk")
    parser.add_argument("--output-dir", default="size_scaling_meshes", help="directory for kept mesh files")
    return parser.parse_args()


def format_table(rows: list[BenchmarkRow]) -> str:
    header = (
        f"{'subdiv':>6} {'vertices':>8} {'faces':>8} {'edges':>8} "
        f"{'workflow':<18} {'operations':>10} {'wall_s':>12} {'rate':>12} {'rate_units':<10}"
    )
    lines = [header]
    for row in rows:
        lines.append(
            f"{row.subdivision:>6} {row.vertices:>8} {row.faces:>8} {row.edges:>8} "
            f"{row.workflow:<18} {row.operations:>10} {row.wall_s:>12.6f} {row.rate:>12.3f} {row.rate_units:<10}"
        )
    return "\n".join(lines)


def json_payload(args: argparse.Namespace, rows: list[BenchmarkRow]) -> dict[str, object]:
    return {
        "environment": {
            "platform": platform.platform(),
            "python": sys.version.replace("\n", " "),
            "pymembrane_path": str(Path(mb.__file__).resolve()),
            "timestamp": datetime.now(timezone.utc).isoformat(),
        },
        "parameters": {
            "subdivisions": args.subdivisions,
            "steps": args.steps,
        },
        "results": [asdict(row) for row in rows],
    }


def main() -> int:
    args = parse_args()
    if args.quick:
        print("Running in quick smoke-test mode")

    args.subdivisions = args.subdivisions if args.subdivisions is not None else ([0, 1, 2] if args.quick else [0, 1, 2, 3])
    args.steps = args.steps if args.steps is not None else (10 if args.quick else 100)

    mesh_root = None
    if args.keep_meshes:
        mesh_root = Path(args.output_dir)
        mesh_root.mkdir(parents=True, exist_ok=True)

    rows: list[BenchmarkRow] = []
    for subdivision in args.subdivisions:
        rows.extend(run_one_subdivision(subdivision, args.steps, args.keep_meshes, mesh_root))

    print(format_table(rows))

    if args.json:
        json_path = Path(args.json)
        json_path.parent.mkdir(parents=True, exist_ok=True)
        json_path.write_text(json.dumps(json_payload(args, rows), indent=2), encoding="utf-8")
        print(f"\nJSON written to {json_path}")

    return 0
