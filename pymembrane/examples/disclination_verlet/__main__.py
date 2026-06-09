from __future__ import annotations

# Packaged runnable version of docs/examples/01_disclination/Verlet/disclination.py.
# Physics parameters are intentionally kept identical to the documentation example.

import argparse
import os
from pathlib import Path
import tempfile
import zipfile

import numpy as np
from pprint import pprint

import pymembrane as mb
from .._resources import example_data_path


def _mesh_files(n: int) -> tuple[Path, Path]:
    cache_dir = Path(tempfile.gettempdir()) / f"pymembrane_disclination_{n}"
    vertex_file = cache_dir / "InputFiles" / f"vertices_N{n}.inp"
    face_file = cache_dir / "InputFiles" / f"faces_N{n}.inp"
    if not vertex_file.exists() or not face_file.exists():
        cache_dir.mkdir(parents=True, exist_ok=True)
        with example_data_path(__package__, "InputFiles.zip") as archive_path:
            with zipfile.ZipFile(archive_path) as archive:
                archive.extract(f"InputFiles/vertices_N{n}.inp", path=cache_dir)
                archive.extract(f"InputFiles/faces_N{n}.inp", path=cache_dir)
    return vertex_file, face_file


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Velocity-Verlet relaxation of an open +1 disclination.",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
        epilog="Use --quick for a short smoke-test run.",
    )
    parser.add_argument(
        "--quick",
        action="store_true",
        help="Run a short version of the example for testing the installation.",
    )
    parser.add_argument(
        "--snapshots",
        type=int,
        default=None,
        help="Number of output snapshots written during the run.",
    )
    parser.add_argument(
        "--run_steps",
        type=int,
        default=None,
        help="Number of velocity-Verlet steps between output snapshots.",
    )
    parser.add_argument(
        "--N",
        type=int,
        default=14,
        help="Pentagon mesh size used to select the bundled input mesh.",
    )
    parser.add_argument(
        "--output-dir",
        default=".",
        help="Directory where output mesh files are written.",
    )
    user_args = parser.parse_args()

    if user_args.quick:
        print("Running in quick smoke-test mode")

    output_dir = Path(user_args.output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    os.chdir(output_dir)

    snapshots = user_args.snapshots if user_args.snapshots is not None else (3 if user_args.quick else 4)
    run_steps = user_args.run_steps if user_args.run_steps is not None else (10 if user_args.quick else 25)
    mesh_size = user_args.N

    box = mb.Box(100.0, 100.0, 100.0)
    system = mb.System(box)
    print(system.box)

    vertex_file, face_file = _mesh_files(mesh_size)
    system.read_mesh_from_files(files={"vertices": str(vertex_file), "faces": str(face_file)})

    dump = system.dumper
    dump.vtk("initial mesh", False)

    evolver = mb.Evolver(system)
    evolver.add_force("Mesh>Harmonic", {"k": {"0": "100.0"}, "l0": {"0": "1.0"}})
    evolver.add_force("Mesh>Limit", {"lmin": {"0": "0.7"}, "lmax": {"0": "1.3"}})
    evolver.add_force("Mesh>Bending>Dihedral", {"kappa": {"0": "1.0"}})
    pprint(evolver.get_force_info())

    compute = system.compute
    edge_lengths = compute.edge_lengths()
    avg_edge_length = np.mean(edge_lengths)
    print("[Initial] avg_edge_length = ", avg_edge_length)

    evolver.add_integrator("Mesh>VelocityVerlet>vertex>move", {"limit": "True", "limit_val": "0.008"})

    vertices = system.vertices
    for vertex in vertices:
        vertex.mass = 1.0
    system.vertices = vertices

    for i, vertex in enumerate(system.vertices):
        print(f"vertex[{i}] = {vertex.mass}")
        if i > 10:
            break

    evolver.set_time_step(str(1e-3))
    evolver.set_global_temperature(str(1e-6))

    energy = compute.energy(evolver)
    print("[Initial] energy = ", energy)

    dump.vtk("pentagon_t0")
    for snapshot in range(1, snapshots):
        evolver.evolveMD(steps=run_steps)
        dump.vtk("pentagon_t" + str(snapshot * run_steps))

    edge_lengths = compute.edge_lengths()
    avg_edge_length = np.mean(edge_lengths)
    print("[Final] avg_edge_length = ", avg_edge_length)

    energy = compute.energy(evolver)
    print("[Final] energy = ", energy)


if __name__ == "__main__":
    main()
