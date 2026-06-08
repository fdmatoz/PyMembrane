from __future__ import annotations

import argparse
import cProfile
import io
import json
import math
import pstats
import random
import sys
import tempfile
import time
from pathlib import Path
from typing import Any


PRESETS = {
    "tiny": 0,
    "small": 1,
    "medium": 2,
}


def _normalize(point: tuple[float, float, float]) -> tuple[float, float, float]:
    norm = math.sqrt(sum(value * value for value in point))
    return tuple(value / norm for value in point)


def _midpoint(
    vertices: list[tuple[float, float, float]],
    cache: dict[tuple[int, int], int],
    i: int,
    j: int,
) -> int:
    key = tuple(sorted((i, j)))
    if key in cache:
        return cache[key]
    p = vertices[i]
    q = vertices[j]
    vertices.append(_normalize(((p[0] + q[0]) * 0.5, (p[1] + q[1]) * 0.5, (p[2] + q[2]) * 0.5)))
    cache[key] = len(vertices) - 1
    return cache[key]


def make_icosphere(subdivisions: int) -> tuple[list[tuple[float, float, float]], list[tuple[int, int, int]]]:
    phi = (1.0 + math.sqrt(5.0)) / 2.0
    vertices = [
        _normalize((-1.0, phi, 0.0)),
        _normalize((1.0, phi, 0.0)),
        _normalize((-1.0, -phi, 0.0)),
        _normalize((1.0, -phi, 0.0)),
        _normalize((0.0, -1.0, phi)),
        _normalize((0.0, 1.0, phi)),
        _normalize((0.0, -1.0, -phi)),
        _normalize((0.0, 1.0, -phi)),
        _normalize((phi, 0.0, -1.0)),
        _normalize((phi, 0.0, 1.0)),
        _normalize((-phi, 0.0, -1.0)),
        _normalize((-phi, 0.0, 1.0)),
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

    for _ in range(subdivisions):
        cache: dict[tuple[int, int], int] = {}
        new_faces = []
        for a, b, c in faces:
            ab = _midpoint(vertices, cache, a, b)
            bc = _midpoint(vertices, cache, b, c)
            ca = _midpoint(vertices, cache, c, a)
            new_faces.extend([(a, ab, ca), (b, bc, ab), (c, ca, bc), (ab, bc, ca)])
        faces = new_faces

    return vertices, faces


def write_mesh_files(
    directory: Path,
    vertices: list[tuple[float, float, float]],
    faces: list[tuple[int, int, int]],
) -> tuple[Path, Path]:
    vertices_file = directory / "vertices.dat"
    faces_file = directory / "faces.dat"
    with vertices_file.open("w", encoding="utf-8") as out:
        for index, (x, y, z) in enumerate(vertices):
            out.write(f"{index} {x:.17g} {y:.17g} {z:.17g} 1\n")
    with faces_file.open("w", encoding="utf-8") as out:
        for index, (a, b, c) in enumerate(faces):
            out.write(f"{index} {a} {b} {c} 1 1\n")
    return vertices_file, faces_file


def import_pymembrane():
    try:
        import pymembrane as mb
    except Exception as exc:  # pragma: no cover - diagnostic script
        raise RuntimeError(f"Could not import pymembrane: {exc}") from exc
    return mb


def build_system(mb: Any, preset: str):
    vertices, faces = make_icosphere(PRESETS[preset])
    tempdir = tempfile.TemporaryDirectory(prefix=f"pymembrane-edge-flip-{preset}-")
    vertices_file, faces_file = write_mesh_files(Path(tempdir.name), vertices, faces)

    box = mb.box(4.0, 4.0, 4.0, False, False, False)
    system = mb.System(box)
    system.read_mesh_from_files({"vertices": str(vertices_file), "faces": str(faces_file)}, False)
    mesh_ops = mb.MeshOperations(system)
    return tempdir, system, mesh_ops


def run_benchmark(args: argparse.Namespace) -> dict[str, Any]:
    mb = import_pymembrane()
    tempdir, system, mesh_ops = build_system(mb, args.preset)
    rng = random.Random(args.seed)

    num_vertices = int(system.Numvertices)
    num_edges = int(system.Numedges)
    num_faces = len(system.faces)
    attempts = int(args.steps)

    def attempt_flips() -> None:
        for _ in range(attempts):
            edge_index = rng.randrange(num_edges)
            flip_face_up = bool(rng.randrange(2))
            mesh_ops.flip_edge(edge_index, flip_face_up)
        # Copy final topology back to Python once, outside the attempted-flip loop.
        _ = system.edges

    profile_text = None
    start = time.perf_counter()
    if args.profile:
        profiler = cProfile.Profile()
        profiler.enable()
        attempt_flips()
        profiler.disable()
        stream = io.StringIO()
        pstats.Stats(profiler, stream=stream).sort_stats("cumulative").print_stats(30)
        profile_text = stream.getvalue()
    else:
        attempt_flips()
    elapsed = time.perf_counter() - start

    tempdir.cleanup()

    result = {
        "status": "ok",
        "preset": args.preset,
        "mesh": f"icosphere_subdivisions_{PRESETS[args.preset]}",
        "vertices": num_vertices,
        "edges": num_edges,
        "faces": num_faces,
        "steps": attempts,
        "attempted_flips": attempts,
        "accepted_flips": None,
        "rejected_flips": None,
        "acceptance_ratio": None,
        "total_wall_time_s": elapsed,
        "steps_per_second": attempts / elapsed if elapsed > 0.0 else None,
        "attempted_flips_per_second": attempts / elapsed if elapsed > 0.0 else None,
        "time_per_step_s": elapsed / attempts if attempts else None,
        "package_file": str(Path(mb.__file__).resolve()),
        "profile": profile_text,
        "notes": [
            "Accepted/rejected counters are not exposed by the current C++ API.",
            "This benchmark measures MeshOperations.flip_edge(), not a registered MC integrator.",
        ],
    }
    return result


def print_result(result: dict[str, Any]) -> None:
    print(f"status: {result['status']}")
    print(f"preset: {result.get('preset')}")
    print(f"mesh: {result.get('mesh')}")
    print(f"vertices: {result.get('vertices')}")
    print(f"edges: {result.get('edges')}")
    print(f"faces: {result.get('faces')}")
    print(f"steps: {result.get('steps')}")
    print(f"attempted_flips: {result.get('attempted_flips')}")
    print(f"accepted_flips: {result.get('accepted_flips')}")
    print(f"rejected_flips: {result.get('rejected_flips')}")
    print(f"acceptance_ratio: {result.get('acceptance_ratio')}")
    print(f"total_wall_time_s: {result.get('total_wall_time_s')}")
    print(f"steps_per_second: {result.get('steps_per_second')}")
    print(f"attempted_flips_per_second: {result.get('attempted_flips_per_second')}")
    print(f"time_per_step_s: {result.get('time_per_step_s')}")
    if result.get("error"):
        print(f"error: {result['error']}")
    if result.get("profile"):
        print("\nProfile:")
        print(result["profile"])
    for note in result.get("notes", []):
        print(f"note: {note}")


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description="Benchmark PyMembrane edge-flip attempts.")
    parser.add_argument("--preset", choices=sorted(PRESETS), default="tiny")
    parser.add_argument("--steps", type=int, default=10)
    parser.add_argument("--seed", type=int, default=1)
    parser.add_argument("--json", type=Path, default=None, help="Write benchmark result JSON.")
    parser.add_argument("--profile", action="store_true", help="Run Python-level cProfile.")
    args = parser.parse_args(argv)

    try:
        result = run_benchmark(args)
    except Exception as exc:
        result = {
            "status": "error",
            "preset": args.preset,
            "steps": args.steps,
            "error": str(exc),
            "notes": [
                "Install/build pymembrane before running the edge-flip benchmark.",
                "The native extension is required before this benchmark can measure edge flips.",
            ],
        }
        if args.json:
            args.json.write_text(json.dumps(result, indent=2), encoding="utf-8")
        print_result(result)
        return 2

    if args.json:
        args.json.write_text(json.dumps(result, indent=2), encoding="utf-8")
    print_result(result)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
