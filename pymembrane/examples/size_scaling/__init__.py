from __future__ import annotations

# Packaged runnable version of the spherical scaling benchmark used in the docs.
# The example keeps the physics model representative while focusing on timing
# core operations and documenting the size dependence of the public API.

import argparse
import csv
import json
import math
import os
import platform
import statistics
import subprocess
import sys
import tempfile
import time
from dataclasses import asdict, dataclass
from datetime import datetime, timezone
from pathlib import Path
from typing import Any

import pymembrane as mb


@dataclass
class MeshSpec:
    requested_vertices: int | None
    subdivision: int
    vertices: int
    faces: int
    edges: int
    label: str
    vertex_file: Path
    face_file: Path


@dataclass
class BenchmarkRow:
    requested_vertices: int | None
    subdivision: int
    vertices: int
    faces: int
    edges: int
    benchmark: str
    attempted_operations: int
    repeat: int
    warmup: int
    wall_s: float
    seconds_per_million: float | None
    rate: float
    rate_units: str
    acceptance_ratio: float | None
    peak_memory_mb: float | None
    all_wall_s: list[float]
    all_accepted: list[int] | None = None


def _expected_counts(subdivision: int) -> tuple[int, int, int]:
    scale = 4**subdivision
    return 10 * scale + 2, 20 * scale, 30 * scale


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

    expected_vertices, expected_faces, _ = _expected_counts(subdivision)
    if len(vertices) != expected_vertices or len(faces) != expected_faces:
        raise ValueError(
            f"unexpected icosphere size at subdivision {subdivision}: "
            f"got {(len(vertices), len(faces))}, expected {(expected_vertices, expected_faces)}"
        )
    return vertices, faces


def _subdivision_for_target_vertices(target_vertices: int) -> int:
    if target_vertices <= 0:
        raise ValueError("target vertex counts must be positive")
    best_subdivision = 0
    best_diff = float("inf")
    for subdivision in range(0, 9):
        vertices, _, _ = _expected_counts(subdivision)
        diff = abs(vertices - target_vertices)
        if diff < best_diff:
            best_diff = diff
            best_subdivision = subdivision
    return best_subdivision


def _format_size_label(spec: MeshSpec) -> str:
    if spec.requested_vertices is None:
        return f"s{spec.subdivision}"
    return f"v{spec.requested_vertices}"


def _label_for(requested_vertices: int | None, subdivision: int) -> str:
    if requested_vertices is None:
        return f"s{subdivision}"
    return f"v{requested_vertices}"


def edge_count_from_faces(faces: list[tuple[int, int, int]]) -> int:
    edges: set[tuple[int, int]] = set()
    for a, b, c in faces:
        for i, j in ((a, b), (b, c), (c, a)):
            edge = (i, j) if i < j else (j, i)
            edges.add(edge)
    return len(edges)


def write_mesh_files(
    directory: Path,
    label: str,
    vertices: list[tuple[float, float, float]],
    faces: list[tuple[int, int, int]],
) -> tuple[Path, Path]:
    vertex_file = directory / f"vertices_{label}.inp"
    face_file = directory / f"faces_{label}.inp"
    with vertex_file.open("w", encoding="utf-8") as handle:
        for index, (x, y, z) in enumerate(vertices):
            handle.write(f"{index} {x:.17g} {y:.17g} {z:.17g} 1\n")
    with face_file.open("w", encoding="utf-8") as handle:
        for index, (a, b, c) in enumerate(faces):
            handle.write(f"{index} {a} {b} {c} 1 1\n")
    return vertex_file, face_file


def _peak_memory_mb() -> float | None:
    try:
        import resource
    except Exception:
        return None
    usage = resource.getrusage(resource.RUSAGE_SELF).ru_maxrss
    if usage <= 0:
        return None
    if sys.platform == "darwin":
        return usage / (1024.0 * 1024.0)
    return usage / 1024.0


def _cpu_model() -> str | None:
    if sys.platform == "darwin":
        try:
            result = subprocess.run(
                ["sysctl", "-n", "machdep.cpu.brand_string"],
                check=True,
                capture_output=True,
                text=True,
            )
            value = result.stdout.strip()
            return value or None
        except Exception:
            return platform.processor() or None
    processor = platform.processor()
    if processor:
        return processor
    return platform.machine() or None


def _compiler_info() -> tuple[str | None, str | None]:
    compiler = os.environ.get("CXX") or os.environ.get("CC")
    flags = os.environ.get("CXXFLAGS") or os.environ.get("CFLAGS")
    return compiler, flags


def _git_commit() -> str | None:
    start = Path(mb.__file__).resolve()
    for parent in [start.parent, *start.parents]:
        git_dir = parent / ".git"
        if git_dir.exists():
            try:
                result = subprocess.run(
                    ["git", "-C", str(parent), "rev-parse", "HEAD"],
                    check=True,
                    capture_output=True,
                    text=True,
                )
            except Exception:
                return None
            commit = result.stdout.strip()
            return commit or None
    return None


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


def _build_mesh_spec(requested_vertices: int | None, subdivision: int, mesh_dir: Path) -> MeshSpec:
    actual_subdivision = subdivision
    if requested_vertices is not None:
        actual_subdivision = _subdivision_for_target_vertices(requested_vertices)
    vertices, faces = make_icosphere(actual_subdivision)
    label = _label_for(requested_vertices, actual_subdivision)
    vertex_file, face_file = write_mesh_files(mesh_dir, label, vertices, faces)
    edge_count = edge_count_from_faces(faces)
    return MeshSpec(
        requested_vertices=requested_vertices,
        subdivision=actual_subdivision,
        vertices=len(vertices),
        faces=len(faces),
        edges=edge_count,
        label=label,
        vertex_file=vertex_file,
        face_file=face_file,
    )


def _time_repeats(repeat: int, warmup: int, prepare, run) -> tuple[list[float], list[int] | None]:
    all_wall_s: list[float] = []
    all_accepted: list[int] = []

    def _accepted_total(value: Any) -> int | None:
        if value is None:
            return None
        if isinstance(value, dict):
            total = 0
            found = False
            for item in value.values():
                if isinstance(item, (int, float)):
                    total += int(item)
                    found = True
            return total if found else None
        if isinstance(value, (list, tuple)):
            total = 0
            found = False
            for item in value:
                if isinstance(item, (int, float)):
                    total += int(item)
                    found = True
            return total if found else None
        if isinstance(value, (int, float)):
            return int(value)
        return None

    for _ in range(warmup):
        state = prepare()
        run(state)
    for _ in range(repeat):
        state = prepare()
        start = time.perf_counter()
        accepted = run(state)
        all_wall_s.append(time.perf_counter() - start)
        accepted_total = _accepted_total(accepted)
        if accepted_total is not None:
            all_accepted.append(accepted_total)
    return all_wall_s, all_accepted or None


def _benchmark_row(
    spec: MeshSpec,
    benchmark: str,
    attempted_operations: int,
    repeat: int,
    warmup: int,
    wall_s_values: list[float],
    accepted_values: list[int] | None,
    rate_units: str,
) -> BenchmarkRow:
    if not wall_s_values:
        raise ValueError("at least one timed repeat is required")
    wall_s = statistics.median(wall_s_values)
    rate = float(attempted_operations) / wall_s if wall_s > 0.0 else float("inf")
    seconds_per_million = wall_s / float(attempted_operations) * 1_000_000.0 if attempted_operations > 0 else None
    acceptance_ratio = None
    if accepted_values is not None and attempted_operations > 0:
        total_attempted = attempted_operations * len(wall_s_values)
        acceptance_ratio = sum(accepted_values) / float(total_attempted)
    return BenchmarkRow(
        requested_vertices=spec.requested_vertices,
        subdivision=spec.subdivision,
        vertices=spec.vertices,
        faces=spec.faces,
        edges=spec.edges,
        benchmark=benchmark,
        attempted_operations=attempted_operations,
        repeat=repeat,
        warmup=warmup,
        wall_s=wall_s,
        seconds_per_million=seconds_per_million,
        rate=rate,
        rate_units=rate_units,
        acceptance_ratio=acceptance_ratio,
        peak_memory_mb=_peak_memory_mb(),
        all_wall_s=wall_s_values,
        all_accepted=accepted_values,
    )


def _prepare_mesh_directory(keep_meshes: bool, output_dir: Path | None, label: str):
    if keep_meshes:
        assert output_dir is not None
        mesh_dir = output_dir / label
        mesh_dir.mkdir(parents=True, exist_ok=True)
        return mesh_dir, None
    tempdir = tempfile.TemporaryDirectory(prefix=f"pymembrane-size-scaling-{label}-")
    return Path(tempdir.name), tempdir


def run_one_spec(
    spec: MeshSpec,
    steps: int,
    repeat: int,
    warmup: int,
    dump_output: bool,
) -> list[BenchmarkRow]:
    rows: list[BenchmarkRow] = []

    def make_state():
        system, avg_edge_length = _build_system(spec.vertex_file, spec.face_file)
        evolver = _build_common_evolver(system, avg_edge_length)
        return {"system": system, "evolver": evolver, "avg_edge_length": avg_edge_length}

    def energy_run(state):
        system = state["system"]
        evolver = state["evolver"]
        for _ in range(steps):
            system.compute.energy(evolver)
        return None

    wall_s_values, accepted_values = _time_repeats(repeat, warmup, make_state, energy_run)
    rows.append(
        _benchmark_row(
            spec,
            "energy_eval_only",
            steps,
            repeat,
            warmup,
            wall_s_values,
            accepted_values,
            "evals/s",
        )
    )

    def mc_vertex_run(state):
        evolver = state["evolver"]
        evolver.add_integrator("Mesh>MonteCarlo>vertex>move", {"dr": "0.008"})
        evolver.set_global_temperature("1e-3")
        return evolver.evolveMC(steps=steps)

    wall_s_values, accepted_values = _time_repeats(repeat, warmup, make_state, mc_vertex_run)
    rows.append(
        _benchmark_row(
            spec,
            "mc_vertex_move",
            steps * spec.vertices,
            repeat,
            warmup,
            wall_s_values,
            accepted_values,
            "vertex_moves/s",
        )
    )

    def mc_edge_run(state):
        evolver = state["evolver"]
        evolver.add_integrator("Mesh>MonteCarlo>edge>flip", {"every step": "1"})
        evolver.set_global_temperature("1e-3")
        return evolver.evolveMC(steps=steps)

    wall_s_values, accepted_values = _time_repeats(repeat, warmup, make_state, mc_edge_run)
    rows.append(
        _benchmark_row(
            spec,
            "mc_edge_flip",
            steps * spec.edges,
            repeat,
            warmup,
            wall_s_values,
            accepted_values,
            "edge_flips/s",
        )
    )

    def bd_run(state):
        evolver = state["evolver"]
        evolver.add_integrator("Mesh>Brownian>vertex>move", {"seed": "202208"})
        evolver.set_time_step("2e-3")
        evolver.set_global_temperature("1e-4")
        return evolver.evolveMD(steps=steps)

    wall_s_values, accepted_values = _time_repeats(repeat, warmup, make_state, bd_run)
    rows.append(
        _benchmark_row(
            spec,
            "brownian_dynamics",
            steps * spec.vertices,
            repeat,
            warmup,
            wall_s_values,
            accepted_values,
            "vertex_updates/s",
        )
    )

    if dump_output:
        def dump_run(state):
            system = state["system"]
            dump_dir = state["dump_dir"]
            output_base = dump_dir / "benchmark_dump"
            system.dumper.vtk(str(output_base))
            return None

        def make_dump_state():
            system, avg_edge_length = _build_system(spec.vertex_file, spec.face_file)
            _ = avg_edge_length  # kept for symmetry with the compute benchmarks
            dump_dir = spec.vertex_file.parent
            return {"system": system, "dump_dir": dump_dir}

        wall_s_values, accepted_values = _time_repeats(repeat, warmup, make_dump_state, dump_run)
        rows.append(
            _benchmark_row(
                spec,
                "dump_vtk",
                1,
                repeat,
                warmup,
                wall_s_values,
                accepted_values,
                "files/s",
            )
        )

    return rows


def _row_to_csv(row: BenchmarkRow) -> dict[str, str]:
    data = asdict(row)
    csv_row: dict[str, str] = {}
    for key, value in data.items():
        if isinstance(value, list):
            csv_row[key] = json.dumps(value)
        elif value is None:
            csv_row[key] = ""
        else:
            csv_row[key] = str(value)
    return csv_row


def _environment_metadata() -> dict[str, object]:
    compiler, compiler_flags = _compiler_info()
    return {
        "platform": platform.platform(),
        "os": platform.system(),
        "cpu_model": _cpu_model(),
        "python": sys.version.replace("\n", " "),
        "pymembrane_version": getattr(mb, "__version__", None),
        "pymembrane_path": str(Path(mb.__file__).resolve()),
        "git_commit": _git_commit(),
        "threads": int(os.environ.get("OMP_NUM_THREADS") or os.environ.get("MKL_NUM_THREADS") or os.cpu_count() or 1),
        "compiler": compiler,
        "compiler_flags": compiler_flags,
        "peak_memory_mb": _peak_memory_mb(),
    }


def _resolve_specs(args: argparse.Namespace) -> tuple[list[MeshSpec], list[str]]:
    requested_vertices = args.vertices if args.vertices is not None else None
    subdivisions = args.subdivisions if args.subdivisions is not None else None
    if requested_vertices is not None and subdivisions is not None:
        raise ValueError("Specify either --vertices or --subdivisions, not both")
    if requested_vertices is None and subdivisions is None:
        subdivisions = [0, 1, 2] if args.quick else [0, 1, 2, 3]
    if subdivisions is None:
        specs = []
        notes = []
        for target in requested_vertices or []:
            subdivision = _subdivision_for_target_vertices(target)
            actual_vertices, actual_faces, actual_edges = _expected_counts(subdivision)
            notes.append(f"{target} -> subdivision {subdivision} ({actual_vertices} vertices)")
            specs.append(
                MeshSpec(
                    requested_vertices=target,
                    subdivision=subdivision,
                    vertices=actual_vertices,
                    faces=actual_faces,
                    edges=actual_edges,
                    label=f"v{target}",
                    vertex_file=Path(),
                    face_file=Path(),
                )
            )
        return specs, notes
    specs = []
    for subdivision in subdivisions:
        vertices, faces, edges = _expected_counts(subdivision)
        specs.append(
            MeshSpec(
                requested_vertices=None,
                subdivision=subdivision,
                vertices=vertices,
                faces=faces,
                edges=edges,
                label=f"s{subdivision}",
                vertex_file=Path(),
                face_file=Path(),
            )
        )
    return specs, []


def _print_table(rows: list[BenchmarkRow]) -> str:
    header = (
        f"{'requested':>10} {'subdiv':>6} {'vertices':>8} {'faces':>8} {'edges':>8} "
        f"{'benchmark':<20} {'attempted':>12} {'repeats':>8} {'warmups':>8} "
        f"{'wall_s':>12} {'sec/1e6':>12} {'rate':>14} {'acceptance':>12} {'peak_MB':>10}"
    )
    lines = [header]
    for row in rows:
        requested = "" if row.requested_vertices is None else str(row.requested_vertices)
        acceptance = "" if row.acceptance_ratio is None else f"{row.acceptance_ratio:.3f}"
        peak = "" if row.peak_memory_mb is None else f"{row.peak_memory_mb:.1f}"
        sec_per = "" if row.seconds_per_million is None else f"{row.seconds_per_million:.3f}"
        lines.append(
            f"{requested:>10} {row.subdivision:>6} {row.vertices:>8} {row.faces:>8} {row.edges:>8} "
            f"{row.benchmark:<20} {row.attempted_operations:>12} {row.repeat:>8} {row.warmup:>8} "
            f"{row.wall_s:>12.6f} {sec_per:>12} {row.rate:>14.3f} {acceptance:>12} {peak:>10}"
        )
    return "\n".join(lines)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Performance benchmark and internal size-scaling example for PyMembrane.",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
        epilog="Use --quick for a short version.",
    )
    parser.add_argument(
        "--quick",
        action="store_true",
        help="Run a short version of the example for checking the installation and workflow.",
    )
    parser.add_argument(
        "--subdivisions",
        nargs="+",
        type=int,
        default=None,
        help="Icosphere subdivision levels used to generate the benchmark meshes.",
    )
    parser.add_argument(
        "--vertices",
        nargs="+",
        type=int,
        default=None,
        help="Target vertex counts used to choose the nearest available icosphere mesh size.",
    )
    parser.add_argument(
        "--steps",
        type=int,
        default=None,
        help="Number of evolution steps or energy-evaluation repeats used in each benchmark row.",
    )
    parser.add_argument(
        "--repeat",
        type=int,
        default=None,
        help="Number of timed repeats collected for each benchmark row.",
    )
    parser.add_argument(
        "--warmup",
        type=int,
        default=None,
        help="Number of untimed warmup runs collected before timing each benchmark row.",
    )
    parser.add_argument(
        "--json",
        default=None,
        help="Write the timing summary to a JSON file.",
    )
    parser.add_argument(
        "--csv",
        default=None,
        help="Write the timing summary to a CSV file.",
    )
    parser.add_argument(
        "--dump-output",
        action="store_true",
        help="Time a representative VTK output write in addition to the compute benchmarks.",
    )
    parser.add_argument(
        "--keep-meshes",
        action="store_true",
        help="Keep the generated temporary mesh files on disk.",
    )
    parser.add_argument(
        "--output-dir",
        default="size_scaling_meshes",
        help="Directory used when keeping generated mesh files.",
    )
    return parser.parse_args()


def _json_payload(args: argparse.Namespace, rows: list[BenchmarkRow], notes: list[str]) -> dict[str, object]:
    return {
        "environment": _environment_metadata(),
        "parameters": {
            "subdivisions": args.subdivisions,
            "vertices": args.vertices,
            "steps": args.steps,
            "repeat": args.repeat,
            "warmup": args.warmup,
            "dump_output": args.dump_output,
            "statistic": "median",
        },
        "notes": notes,
        "results": [asdict(row) for row in rows],
    }


def _write_csv(path: Path, rows: list[BenchmarkRow]) -> None:
    if not rows:
        path.write_text("", encoding="utf-8")
        return
    with path.open("w", encoding="utf-8", newline="") as handle:
        writer = csv.DictWriter(handle, fieldnames=list(_row_to_csv(rows[0]).keys()))
        writer.writeheader()
        for row in rows:
            writer.writerow(_row_to_csv(row))


def main() -> int:
    args = parse_args()
    if args.quick:
        print("Running in quick-check mode")

    args.steps = args.steps if args.steps is not None else (100 if args.quick else 1000)
    args.repeat = args.repeat if args.repeat is not None else 3
    args.warmup = args.warmup if args.warmup is not None else 1

    if args.repeat < 1:
        raise ValueError("--repeat must be at least 1")
    if args.warmup < 0:
        raise ValueError("--warmup must be at least 0")
    if args.steps < 1:
        raise ValueError("--steps must be at least 1")

    mesh_root = None
    if args.keep_meshes:
        mesh_root = Path(args.output_dir)
        mesh_root.mkdir(parents=True, exist_ok=True)

    specs, notes = _resolve_specs(args)
    rows: list[BenchmarkRow] = []

    for spec in specs:
        if mesh_root is None:
            mesh_dir_cm = tempfile.TemporaryDirectory(prefix=f"pymembrane-size-scaling-{spec.label}-")
            mesh_dir = Path(mesh_dir_cm.name)
            cleanup = mesh_dir_cm.cleanup
        else:
            mesh_dir = mesh_root / spec.label
            mesh_dir.mkdir(parents=True, exist_ok=True)
            cleanup = None
        try:
            vertices, faces = make_icosphere(spec.subdivision)
            vertex_file, face_file = write_mesh_files(mesh_dir, spec.label, vertices, faces)
            actual_spec = MeshSpec(
                requested_vertices=spec.requested_vertices,
                subdivision=spec.subdivision,
                vertices=len(vertices),
                faces=len(faces),
                edges=edge_count_from_faces(faces),
                label=spec.label,
                vertex_file=vertex_file,
                face_file=face_file,
            )
            if (actual_spec.vertices, actual_spec.faces, actual_spec.edges) != _expected_counts(spec.subdivision):
                raise ValueError(f"unexpected mesh size at subdivision {spec.subdivision}")
            rows.extend(run_one_spec(actual_spec, args.steps, args.repeat, args.warmup, args.dump_output))
        finally:
            if cleanup is not None:
                cleanup()

    print(_print_table(rows))

    if args.json:
        json_path = Path(args.json)
        json_path.parent.mkdir(parents=True, exist_ok=True)
        json_path.write_text(json.dumps(_json_payload(args, rows, notes), indent=2), encoding="utf-8")
        print(f"\nJSON written to {json_path}")

    if args.csv:
        csv_path = Path(args.csv)
        csv_path.parent.mkdir(parents=True, exist_ok=True)
        _write_csv(csv_path, rows)
        print(f"CSV written to {csv_path}")

    if notes:
        print("\nRequested vertex counts mapped to the nearest available icosphere sizes:")
        for note in notes:
            print(f"  {note}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
