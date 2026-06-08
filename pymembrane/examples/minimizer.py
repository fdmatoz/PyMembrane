from __future__ import annotations

# Packaged runnable version of docs/examples/04_minimization/minimizer.py.
# Physics parameters are intentionally kept identical to the documentation example.

import argparse
import os
from pprint import pprint

import numpy as np

import pymembrane as mb
from ._resources import example_data_path


def main() -> None:
    parser = argparse.ArgumentParser(description="Please provide: snapshots and run_steps")
    parser.add_argument("--quick", action="store_true")
    parser.add_argument("--snapshots", type=int, default=None, help="Number of snapshots")
    parser.add_argument("--max_iter", type=int, default=None, help="Number of iteration steps")
    parser.add_argument("--output-dir", default=".", help="directory for output files")
    user_args = parser.parse_args()

    if user_args.quick:
        print("Running in quick smoke-test mode")

    os.makedirs(user_args.output_dir, exist_ok=True)
    os.chdir(user_args.output_dir)

    snapshots = user_args.snapshots if user_args.snapshots is not None else (2 if user_args.quick else 50)
    max_iter = user_args.max_iter if user_args.max_iter is not None else (25 if user_args.quick else 2000)

    with example_data_path("04_minimization/vertices_R1.0_l01.inp") as vertex_file, example_data_path("04_minimization/faces_R1.0_l01.inp") as face_file:
        box = mb.Box(40, 40, 40)
        system = mb.System(box)
        system.read_mesh_from_files(files={"vertices": str(vertex_file), "faces": str(face_file)})

        dump = system.dumper
        dump.vtk("initial mesh", False)

        evolver = mb.Evolver(system)

        compute = system.compute
        edge_lengths = compute.edge_lengths()
        avg_edge_length = np.mean(edge_lengths)
        print("[Initial] avg_edge_length = ", avg_edge_length)

        k = str(400.0)
        l0 = str(1.1 * avg_edge_length)
        evolver.add_force("Mesh>Harmonic", {"k": {"0": k}, "l0": {"0": l0}})

        lmin = str(0.5 * np.min(edge_lengths))
        lmax = str(2.0 * np.max(edge_lengths))
        evolver.add_force("Mesh>Limit", {"lmin": {"0": lmin}, "lmax": {"0": lmax}})
        evolver.add_force("Mesh>Bending>Dihedral", {"kappa": {"0": "1.0"}})

        evolver.add_minimizer(
            "Mesh>Fire",
            {"dt": str(1e-2), "max_iter": str(max_iter), "ftol": str(1e-2), "etol": str(1e-4)},
        )
        pprint(evolver.get_minimizer_info())

        initial_volume = compute.volume()
        evolver.add_constraint("Mesh>Volume", {"V": str(initial_volume), "max_iter": str(10000), "tol": str(1e-5)})

        rnd_move = 0.5 * np.std(edge_lengths) / 6.0
        vertices = system.vertices
        for vertex in vertices:
            vertex.r.x += np.random.uniform(-rnd_move, rnd_move)
            vertex.r.y += np.random.uniform(-rnd_move, rnd_move)
            vertex.r.z += np.random.uniform(-rnd_move, rnd_move)
        system.vertices = vertices

        min_energy = snapshots * [None]
        min_energy[0] = 100.0 * compute.energy(evolver)["edges"] / system.Numedges
        print("[Initial] energy:{} x 10^-2 volume:{}".format(min_energy[0], compute.volume()))

        dump.vtk("minimization_t0")
        for snapshot in range(1, snapshots):
            evolver.minimize()
            dump.vtk("minimization_t{}".format(snapshot))
            min_energy[snapshot] = 100.0 * compute.energy(evolver)["edges"] / system.Numedges
            print("[{}] energy:{} x 10^-2 volume:{}".format(snapshot, min_energy[snapshot], compute.volume()))

        energy = compute.energy(evolver)
        print("[Final] energy:{} x 10^-2 volume:{}".format(min_energy[snapshots - 1], compute.volume()))
        final_volume = compute.volume()
        print("volume difference: {}".format(final_volume - initial_volume))
        pprint(evolver.get_minimizer_info())


if __name__ == "__main__":
    main()
