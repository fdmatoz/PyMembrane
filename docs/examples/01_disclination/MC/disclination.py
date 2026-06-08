from pathlib import Path
import tempfile
import zipfile

import pymembrane as mb
import numpy as np
from pprint import pprint
import argparse

HERE = Path(__file__).resolve().parent


def mesh_files(n: int) -> tuple[Path, Path]:
    cache_dir = Path(tempfile.gettempdir()) / f"pymembrane_disclination_{n}"
    vertex_file = cache_dir / f"InputFiles/vertices_N{n}.inp"
    face_file = cache_dir / f"InputFiles/faces_N{n}.inp"
    if not vertex_file.exists() or not face_file.exists():
        cache_dir.mkdir(parents=True, exist_ok=True)
        with zipfile.ZipFile(HERE.parent / "InputFiles.zip") as archive:
            archive.extract(f"InputFiles/vertices_N{n}.inp", path=cache_dir)
            archive.extract(f"InputFiles/faces_N{n}.inp", path=cache_dir)
    return vertex_file, face_file


### Parse arguments
## Now we want to have X snapshots every X steps each
parser = argparse.ArgumentParser(description="Please provide: snapshots and run_steps")
parser.add_argument("--quick", action="store_true")
parser.add_argument("--snapshots", type=int, default=None, help="Number of snapshots")
parser.add_argument("--run_steps", type=int, default=None, help="Number of run steps")
parser.add_argument("--N", type=int, default=14, help="Pentagon Number size")

user_args = parser.parse_args()
snapshots = user_args.snapshots if user_args.snapshots is not None else (3 if user_args.quick else 200)
run_steps = user_args.run_steps if user_args.run_steps is not None else (10 if user_args.quick else 5000)
N = user_args.N

#create a system 
box = mb.Box(40.0, 40.0, 40.0)

system = mb.System(box)

#check if the box is loaded correctly
print(system.box)

# read the mesh
vertex_file, face_file = mesh_files(N)
system.read_mesh_from_files(files={'vertices': str(vertex_file), 'faces': str(face_file)})


#save the mesh to display
#create dumper
dump = system.dumper
dump.vtk("initial mesh", False)


#add the evolver class where the potentials and integrators are added
evolver = mb.Evolver(system)


#add the potentials in this case we will add bending and stretching 
# stretching 
k = str(100.0)
l0 = str(1.0)
evolver.add_force("Mesh>Harmonic", {"k":{"0":k}, 
                                    "l0":{"0":l0}})

# limit potential
lmin = str(0.7)
lmax = str(1.3)
evolver.add_force("Mesh>Limit", {"lmin":{"0":lmin}, 
                                 "lmax":{"0":lmax}})
# bending potential
kappa = str(1.0)
evolver.add_force("Mesh>Bending>Dihedral", {"kappa":{"0":kappa}})

#Print the loaded forces
pprint(evolver.get_force_info())


# vertex move:
# first we need to know the edge length to move it appropriate:
compute = system.compute
edge_lengths = compute.edge_lengths()
avg_edge_length= np.mean(edge_lengths)
print("[Initial] avg_edge_length = ", avg_edge_length)

#add the monte carlo integrator
evolver.add_integrator("Mesh>MonteCarlo>vertex>move", {"dr":"0.008"})


## Compute the initial energy
mc_energy = snapshots*[None]
mc_energy[0] = 100.0*compute.energy(evolver)['edges']/system.Numedges
print("[Initial] energy = {} x 10^-2".format(mc_energy[0]))

dump.vtk("pentagon_t0")
for snapshot in range(1, snapshots):
    for temperature in [1e-3, 1e-5, 1e-7, 0.0]: #using heating and cooling cycles
        evolver.set_global_temperature(str(temperature))
        evolver.evolveMC(steps=run_steps)
    dump.vtk("pentagon_t" + str(snapshot*run_steps))
    mc_energy[snapshot] =  100.0*compute.energy(evolver)['edges']/system.Numedges
    print("[{}] energy = {} x 10^-2".format(snapshot,  mc_energy[snapshot]))

dump.vtk('final_mesh')

edge_lengths = compute.edge_lengths()
avg_edge_length= np.mean(edge_lengths)
print("[Final] avg_edge_length = ", avg_edge_length)

energy = compute.energy(evolver)
print("[Final] energy = ", mc_energy[snapshots-1])
