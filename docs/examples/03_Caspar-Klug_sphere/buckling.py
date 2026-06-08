from pathlib import Path

import pymembrane as mb
import numpy as np
from pprint import pprint
import argparse

HERE = Path(__file__).resolve().parent


### Parse arguments
## Now we want to have X snapshots every X steps each
parser = argparse.ArgumentParser(description="Please provide: snapshots and run_steps")
parser.add_argument("--quick", action="store_true")
parser.add_argument("--snapshots", type=int, default=None, help="Number of snapshots")
parser.add_argument("--run_steps", type=int, default=None, help="Number of run steps")

user_args = parser.parse_args()
snapshots = user_args.snapshots if user_args.snapshots is not None else (3 if user_args.quick else 100)
run_steps = user_args.run_steps if user_args.run_steps is not None else (10 if user_args.quick else 5000)

#create a system 
box = mb.Box(4,4,4)

system = mb.System(box)

#check if the box is loaded correctly
print(system.box)

#read the mesh
vertex_file = HERE / 'vertices.dat'
face_file = HERE / 'faces.dat'
system.read_mesh_from_files(files={'vertices': str(vertex_file), 'faces': str(face_file)})


#save the mesh to display
#create dumper
dump = system.dumper
dump.vtk("initial mesh", False)


#add the evolver class where the potentials and integrators are added
evolver = mb.Evolver(system)


#add the potentials in this case we will add bending and stretching 
# stretching 
k = str(350.0)
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

dump.vtk("sphere_t0")
for snapshot in range(1, snapshots):
    for temperature in [1e-3, 1e-5, 1e-7, 0.0]: #using heating and cooling cycles
        evolver.set_global_temperature(str(temperature))
        evolver.evolveMC(steps=run_steps)
    dump.vtk("sphere_t" + str(snapshot*run_steps))
    mc_energy[snapshot] =  100.0*compute.energy(evolver)['edges']/system.Numedges
    print("[{}] energy = {} x 10^-2".format(snapshot,  mc_energy[snapshot]))

dump.vtk('final_mesh')

edge_lengths = compute.edge_lengths()
avg_edge_length= np.mean(edge_lengths)
print("[Final] avg_edge_length = ", avg_edge_length)

energy = compute.energy(evolver)
print("[Final] energy = ", mc_energy[snapshots-1])
