from __future__ import annotations

# Packaged runnable example for a simple dynamically triangulated liquid membrane.

import argparse
import json
import math
import platform
import sys
import tempfile
from contextlib import nullcontext
from dataclasses import asdict, dataclass
from datetime import datetime, timezone
from pathlib import Path
from typing import Any

import pymembrane as mb


EXPECTED_COUNTS = {
    0: (12, 20, 30),
    1: (42, 80, 120),
    2: (162, 320, 480),
    3: (642, 1280, 1920),
    4: (2562, 5120, 7680),
}


@dataclass
class Summary:
    subdivision: int
    vertices: int
    faces: int
    edges: int
    steps: int
    temperature: float
    seed: int
    initial_energy: dict[str, Any]
    final_energy: dict[str, Any]
    accepted_moves: dict[str, int]
    output_files: list[str]


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


def _build_evolver(system: mb.System, avg_edge_length: float, seed: int) -> mb.Evolver:
    evolver = mb.Evolver(system)
    evolver.add_force("Mesh>Harmonic", {"k": {"0": "1.0"}, "l0": {"0": str(avg_edge_length)}})
    evolver.add_force(
        "Mesh>Limit",
        {"lmin": {"0": str(0.7 * avg_edge_length)}, "lmax": {"0": str(1.3 * avg_edge_length)}},
    )
    evolver.add_force("Mesh>Bending>Dihedral", {"kappa": {"0": "1.0"}})
    evolver.add_integrator(
        "Mesh>MonteCarlo>vertex>move",
        {"dr": "0.008", "spherical_move": "true", "seed": str(seed)},
    )
    evolver.add_integrator("Mesh>MonteCarlo>edge>flip", {"every step": "1", "seed": str(seed + 1)})
    return evolver


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Simple liquid-membrane example with Monte Carlo edge flips.",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
        epilog="Use --quick for a short smoke-test run.",
    )
    parser.add_argument(
        "--quick",
        action="store_true",
        help="Run a short version of the example for testing the installation.",
    )
    parser.add_argument(
        "--steps",
        type=int,
        default=None,
        help="Number of Monte Carlo steps used in the main simulation run.",
    )
    parser.add_argument(
        "--subdivision",
        type=int,
        default=None,
        help="Icosphere subdivision level used to build the initial spherical mesh.",
    )
    parser.add_argument(
        "--temperature",
        type=float,
        default=1e-1,
        help="Monte Carlo temperature used for the liquid-membrane workflow.",
    )
    parser.add_argument(
        "--output-dir",
        default=".",
        help="Directory where the VTK, OBJ, and summary files are written.",
    )
    parser.add_argument(
        "--json",
        default="summary.json",
        help="Path to the summary JSON file.",
    )
    parser.add_argument(
        "--seed",
        type=int,
        default=202208,
        help="Base random seed used for the Monte Carlo integrators.",
    )
    parser.add_argument(
        "--keep-meshes",
        action="store_true",
        help="Keep the generated temporary mesh input files on disk.",
    )
    return parser.parse_args()


def _resolve_path(base: Path, path: str) -> Path:
    candidate = Path(path)
    if candidate.is_absolute():
        return candidate
    return base / candidate


def main() -> int:
    args = parse_args()
    if args.quick:
        print("Running in quick smoke-test mode")

    subdivision = args.subdivision if args.subdivision is not None else (1 if args.quick else 2)
    steps = args.steps if args.steps is not None else (100 if args.quick else 1000)
    if subdivision not in EXPECTED_COUNTS:
        raise ValueError(f"unsupported subdivision level {subdivision}")
    if steps < 1:
        raise ValueError("--steps must be at least 1")

    output_dir = Path(args.output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)

    mesh_root: Path | None = None
    if args.keep_meshes or output_dir != Path("."):
        mesh_root = output_dir / "mesh_inputs"
        mesh_root.mkdir(parents=True, exist_ok=True)

    mesh_context = nullcontext(mesh_root) if mesh_root is not None else tempfile.TemporaryDirectory(prefix="pymembrane-liquid-membrane-")
    with mesh_context as mesh_path:
        mesh_dir = Path(mesh_path)
        vertices, faces = make_icosphere(subdivision)
        vertex_file, face_file = write_mesh_files(mesh_dir, subdivision, vertices, faces)

        system, avg_edge_length = _build_system(vertex_file, face_file)
        edges = edge_count_from_faces(faces)
        initial_evolver = _build_evolver(system, avg_edge_length, args.seed)
        initial_energy = system.compute.energy(initial_evolver)

        initial_vtk = output_dir / "initial"
        final_vtk = output_dir / "final"
        final_obj = output_dir / "final"
        system.dumper.vtk(str(initial_vtk))

        system, avg_edge_length = _build_system(vertex_file, face_file)
        evolver = _build_evolver(system, avg_edge_length, args.seed)
        evolver.set_global_temperature(str(args.temperature))
        accepted_moves = evolver.evolveMC(steps=steps)

        final_energy = system.compute.energy(evolver)
        system.dumper.vtk(str(final_vtk))
        system.dumper.obj(str(final_obj))

        json_path = _resolve_path(output_dir, args.json)
        json_path.parent.mkdir(parents=True, exist_ok=True)

        summary = Summary(
            subdivision=subdivision,
            vertices=len(vertices),
            faces=len(faces),
            edges=edges,
            steps=steps,
            temperature=args.temperature,
            seed=args.seed,
            initial_energy=initial_energy,
            final_energy=final_energy,
            accepted_moves=dict(accepted_moves),
            output_files=[
                str(initial_vtk.with_suffix(".vtk")),
                str(final_vtk.with_suffix(".vtk")),
                str(final_obj.with_suffix(".obj")),
                str(json_path),
            ],
        )

        json_path.write_text(
            json.dumps(
                {
                    "environment": {
                        "platform": platform.platform(),
                        "python": sys.version.replace("\n", " "),
                        "pymembrane_path": str(Path(mb.__file__).resolve()),
                        "timestamp": datetime.now(timezone.utc).isoformat(),
                    },
                    "parameters": {
                        "subdivision": subdivision,
                        "steps": steps,
                        "temperature": args.temperature,
                        "seed": args.seed,
                        "quick": bool(args.quick),
                    },
                    "summary": asdict(summary),
                },
                indent=2,
            ),
            encoding="utf-8",
        )

        print(
            "Liquid membrane example "
            f"vertices: {len(vertices)} faces: {len(faces)} edges: {edges} "
            "integrators: Mesh>MonteCarlo>vertex>move, Mesh>MonteCarlo>edge>flip "
            f"steps: {steps} accepted moves: {dict(accepted_moves)} "
            f"output directory: {output_dir}"
        )
        print(f"initial energy: {initial_energy}")
        print(f"final energy: {final_energy}")
        print("wrote:")
        for name in summary.output_files:
            print(name)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
