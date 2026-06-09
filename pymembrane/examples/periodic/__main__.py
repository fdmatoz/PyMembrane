from __future__ import annotations

# Packaged runnable version of docs/examples/02_periodic/periodic.py.
# Physics parameters are intentionally kept identical to the documentation example.

import argparse
import os
from math import sqrt
from pathlib import Path
from pprint import pprint

import numpy as np

import pymembrane as mb
from .._resources import example_data_dir


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Periodic wrinkling example for a triangulated sheet.",
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
        help="Number of Brownian-dynamics steps between output snapshots.",
    )
    parser.add_argument(
        "--epsilon",
        type=float,
        default=0.01,
        help="Fractional compression applied to the x box length after each snapshot.",
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

    snapshots = user_args.snapshots if user_args.snapshots is not None else (3 if user_args.quick else 100)
    run_steps = user_args.run_steps if user_args.run_steps is not None else (10 if user_args.quick else 5000)
    epsilon = user_args.epsilon

    with example_data_dir(__package__) as data_dir:
        vertex_file = data_dir / "vertices.dat"
        face_file = data_dir / "faces.dat"

        box = mb.Box(sqrt(3.0) * 29, 50.0, 50.0, True, True, True)

        print(box)

        system = mb.System(box)
        system.read_mesh_from_files(files={"vertices": str(vertex_file), "faces": str(face_file)})
        system.enforce_boundaries()

        dump = system.dumper
        dump.vtk(filename="initial_mesh", periodic=True)

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
        evolver.set_time_step("2e-3")
        evolver.set_global_temperature("1e-4")

        def compress_box(_epsilon):
            old_box = system.box
            new_box = mb.Box(old_box.L.x * (1 - _epsilon), old_box.L.y, old_box.L.z, True, True, True)
            return new_box

        energy = compute.energy(evolver)
        print("[Initial] energy = ", energy)

        dump.vtk("periodic_t0", periodic=True)
        for snapshot in range(1, snapshots):
            evolver.evolveMD(steps=run_steps)
            dump.vtk("periodic_t" + str(snapshot * run_steps), periodic=True)
            if snapshot < 50:
                system.box = compress_box(epsilon)
                system.enforce_boundaries()

        edge_lengths = compute.edge_lengths()
        avg_edge_length = np.mean(edge_lengths)
        print("[Final] avg_edge_length = ", avg_edge_length)

        energy = compute.energy(evolver)
        print("[Final] energy = ", energy)


if __name__ == "__main__":
    main()
