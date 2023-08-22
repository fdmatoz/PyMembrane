#!/usr/bin/env python
# coding: utf-8


#import the code
import pymembrane as mb
import numpy as np
from pprint import pprint


#create a system 
box = mb.Box(100.0, 100.0, 100.0)

system = mb.System(box)

#check if the box is loaded correctly
print(system.box)

#read the mesh
N = 14 #pentagon size
vertex_file = '../vertices_N' + str(N) + '.inp'
vertex_file = 'pentagon_vertices.dat'
face_file = '../faces_N' + str(N) + '.inp'
system.read_mesh_from_files(files={'vertices':vertex_file, 'faces':face_file})


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

#add the Brownian integrator
evolver.add_integrator("Mesh>Brownian>vertex>move", {"seed":"202208"})

dt = str(2e-3)
evolver.set_time_step(dt)

## Now we want to have 100 snapshots every 5000 steps each
snapshots = 200
run_steps = 10000

## then we want to run the simulation for a temperature 1e-4
temperature = str(1e-4)
evolver.set_global_temperature(temperature)


## Compute the initial energy
energy = compute.energy(evolver)
print("[Initial] energy = ", energy)

dump.vtk("pentagon_t0")
for snapshot in range(1, snapshots):
    for temperature in [1e-4, 1e-5, 1e-6, 0.0]:
        evolver.set_global_temperature(str(temperature))
        evolver.evolveMD(steps=run_steps)
    dump.vtk("pentagon_t" + str(snapshot*run_steps))

edge_lengths = compute.edge_lengths()
avg_edge_length= np.mean(edge_lengths)
print("[Final] avg_edge_length = ", avg_edge_length)

energy = compute.energy(evolver)
print("[Final] energy = ", energy)

dump.txt(f"final_pentagon_N={N}")