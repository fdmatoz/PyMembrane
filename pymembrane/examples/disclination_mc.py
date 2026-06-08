from __future__ import annotations

# Packaged runnable version of docs/examples/01_disclination/MC/disclination.py.
# Physics parameters are intentionally kept identical to the documentation example.

import argparse
import os
from pathlib import Path
import tempfile
import zipfile

import numpy as np
from pprint import pprint

import pymembrane as mb
from ._resources import example_data_path


def _mesh_files(n: int) -> tuple[Path, Path]:
    cache_dir = Path(tempfile.gettempdir()) / f"pymembrane_disclination_{n}"
    vertex_file = cache_dir / f"InputFiles/vertices_N{n}.inp"
    face_file = cache_dir / f"InputFiles/faces_N{n}.inp"
    if not vertex_file.exists() or not face_file.exists():
        cache_dir.mkdir(parents=True, exist_ok=True)
        with example_data_path("01_disclination/InputFiles.zip") as archive_path:
            with zipfile.ZipFile(archive_path) as archive:
                archive.extract(f"InputFiles/vertices_N{n}.inp", path=cache_dir)
                archive.extract(f"InputFiles/faces_N{n}.inp", path=cache_dir)
    return vertex_file, face_file


def main() -> None:
    parser = argparse.ArgumentParser(description="Please provide: snapshots and run_steps")
    parser.add_argument("--quick", action="store_true")
    parser.add_argument("--snapshots", type=int, default=None, help="Number of snapshots")
    parser.add_argument("--run_steps", type=int, default=None, help="Number of run steps")
    parser.add_argument("--N", type=int, default=14, help="Pentagon Number size")
    parser.add_argument("--output-dir", default=".", help="directory for output files")
    user_args = parser.parse_args()

    if user_args.quick:
        print("Running in quick smoke-test mode")

    os.makedirs(user_args.output_dir, exist_ok=True)
    os.chdir(user_args.output_dir)

    snapshots = user_args.snapshots if user_args.snapshots is not None else (3 if user_args.quick else 4)
    run_steps = user_args.run_steps if user_args.run_steps is not None else (10 if user_args.quick else 25)
    N = user_args.N

    box = mb.Box(100.0, 100.0, 100.0)
    system = mb.System(box)
    print(system.box)

    vertex_file, face_file = _mesh_files(N)
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

    evolver.add_integrator("Mesh>MonteCarlo>vertex>move", {"dr": "0.008"})

    mc_energy = snapshots * [None]
    mc_energy[0] = 100.0 * compute.energy(evolver)["edges"] / system.Numedges
    print("[Initial] energy = {} x 10^-2".format(mc_energy[0]))

    dump.vtk("pentagon_t0")
    for snapshot in range(1, snapshots):
        for temperature in [1e-3, 1e-5, 1e-7, 0.0]:
            evolver.set_global_temperature(str(temperature))
            evolver.evolveMC(steps=run_steps)
        dump.vtk("pentagon_t" + str(snapshot * run_steps))
        mc_energy[snapshot] = 100.0 * compute.energy(evolver)["edges"] / system.Numedges
        print("[{}] energy = {} x 10^-2".format(snapshot, mc_energy[snapshot]))

    dump.vtk("final_mesh")

    edge_lengths = compute.edge_lengths()
    avg_edge_length = np.mean(edge_lengths)
    print("[Final] avg_edge_length = ", avg_edge_length)

    energy = compute.energy(evolver)
    print("[Final] energy = ", mc_energy[snapshots - 1])


if __name__ == "__main__":
    main()
