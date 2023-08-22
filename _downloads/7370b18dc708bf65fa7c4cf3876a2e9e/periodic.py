#import the code
import pymembrane as mb
from math import sqrt
import numpy as np
from pprint import pprint
import argparse


### Parse arguments
## Now we want to have X snapshots every X steps each
parser = argparse.ArgumentParser(description="Please provide: snapshots and run_steps")
## Add arguments for snapshots and run_steps
parser.add_argument("--snapshots", type=int, required=True, help="Number of snapshots")
parser.add_argument("--run_steps", type=int, required=True, help="Number of run steps")
parser.add_argument("--epsilon", type=float, required=True, help="strain in x direction")



user_args = parser.parse_args()
# Access the parsed arguments
snapshots = user_args.snapshots
run_steps = user_args.run_steps
epsilon = user_args.epsilon


vertex_file = 'vertices.dat'
face_file = 'faces.dat'
box = mb.Box(sqrt(3.0)*29, 50.0, 50.0, True, True, True)

print(box)

#create a system 
system = mb.System(box)

#read the mesh
system.read_mesh_from_files(files={'vertices':vertex_file, 'faces':face_file})
#warpup all the vertices inside of the box for periodic boundary conditions 
system.enforce_boundaries()

"""
we create a dumper that it does what it says, dump the visualization 
"""
#create dumper
dump = system.dumper
dump.vtk(filename="initial_mesh", periodic=True)


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

#add the Brownian integrator
evolver.add_integrator("Mesh>Brownian>vertex>move", {"seed":"202208"})

dt = str(2e-3)
evolver.set_time_step(dt)


## then we want to run the simulation for a temperature 1e-3
temperature = str(1e-4)
evolver.set_global_temperature(temperature)


"""
In this simulation we will compress the box in the x direction by a factor epsilon
"""

def compress_box(_epsilon):
    old_box = system.box
    new_box = mb.Box(old_box.L.x*(1-_epsilon), old_box.L.y, old_box.L.z, True, True, True)
    system.box = new_box
    return new_box

## Compute the initial energy
energy = compute.energy(evolver)
print("[Initial] energy = ", energy)

dump.vtk("periodic_t0", periodic=True)
for snapshot in range(1, snapshots):
    evolver.evolveMD(steps=run_steps)
    dump.vtk("periodic_t" + str(snapshot*run_steps), periodic=True)
    #compress the box
    if snapshot<50:
        system.box = compress_box(epsilon)
        system.enforce_boundaries()

edge_lengths = compute.edge_lengths()
avg_edge_length= np.mean(edge_lengths)
print("[Final] avg_edge_length = ", avg_edge_length)

energy = compute.energy(evolver)
print("[Final] energy = ", energy)