from __future__ import annotations

# Packaged runnable version composed from the documented disclination examples.
# Physics parameters are intentionally reused from the Brownian and Monte Carlo
# documentation examples; only the scheduling is hybridized at the Python level.

import argparse
import os
from pathlib import Path
import tempfile
import zipfile

from pprint import pprint

import numpy as np

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
        description="Hybrid Brownian dynamics plus Monte Carlo disclination example.",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
        epilog="Use --quick for a short smoke-test run.",
    )
    parser.add_argument(
        "--quick",
        action="store_true",
        help="Run a short version of the example for testing the installation.",
    )
    parser.add_argument(
        "--cycles",
        type=int,
        default=None,
        help="Number of alternating Brownian/Monte Carlo cycles to run.",
    )
    parser.add_argument(
        "--md-steps",
        type=int,
        default=None,
        help="Brownian-dynamics steps per hybrid cycle.",
    )
    parser.add_argument(
        "--mc-steps",
        type=int,
        default=None,
        help="Monte Carlo steps per temperature value in each hybrid cycle.",
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

    cycles = user_args.cycles if user_args.cycles is not None else (3 if user_args.quick else 8)
    md_steps = user_args.md_steps if user_args.md_steps is not None else (10 if user_args.quick else 25)
    mc_steps = user_args.mc_steps if user_args.mc_steps is not None else (10 if user_args.quick else 25)
    mesh_size = user_args.N

    box = mb.Box(100.0, 100.0, 100.0)
    system = mb.System(box)
    vertex_file, face_file = _mesh_files(mesh_size)
    system.read_mesh_from_files(files={"vertices": str(vertex_file), "faces": str(face_file)})

    dump = system.dumper
    dump.vtk("initial_mesh", False)

    evolver = mb.Evolver(system)
    evolver.add_force("Mesh>Harmonic", {"k": {"0": "100.0"}, "l0": {"0": "1.0"}})
    evolver.add_force("Mesh>Limit", {"lmin": {"0": "0.7"}, "lmax": {"0": "1.3"}})
    evolver.add_force("Mesh>Bending>Dihedral", {"kappa": {"0": "1.0"}})
    pprint(evolver.get_force_info())

    compute = system.compute
    edge_lengths = compute.edge_lengths()
    avg_edge_length = np.mean(edge_lengths)
    print("[Initial] avg_edge_length = ", avg_edge_length)

    evolver.add_integrator("Mesh>Brownian>vertex>move", {"seed": "202208"})
    evolver.add_integrator("Mesh>MonteCarlo>vertex>move", {"dr": "0.008"})
    evolver.set_time_step("2e-3")

    mc_temperatures = [1e-3, 1e-5, 1e-7, 0.0]
    bd_temperature = 1e-4

    initial_energy = compute.energy(evolver)
    print("Hybrid MC+BD example")
    print(f"vertices: {system.Numvertices}")
    print(f"faces: {system.Numfaces}")
    print(f"cycles: {cycles}")
    print(f"MD steps per cycle: {md_steps}")
    print(f"MC steps per cycle: {mc_steps}")
    print(f"initial energy: {initial_energy}")

    written_files = ["initial_mesh.vtk"]
    dump.vtk("hybrid_t0")
    written_files.append("hybrid_t0.vtk")

    for cycle in range(1, cycles + 1):
        evolver.set_global_temperature(str(bd_temperature))
        evolver.evolveMD(steps=md_steps)
        for temperature in mc_temperatures:
            evolver.set_global_temperature(str(temperature))
            evolver.evolveMC(steps=mc_steps)
        filename = f"hybrid_t{cycle}"
        dump.vtk(filename)
        written_files.append(f"{filename}.vtk")

    final_energy = compute.energy(evolver)
    print(f"final energy: {final_energy}")
    print("wrote:")
    for name in written_files:
        print(name)


if __name__ == "__main__":
    main()
