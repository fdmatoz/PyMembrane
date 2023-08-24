#!/usr/bin/env python
# coding: utf-8

# In[ ]:


#import the code
import pymembrane as mb
import numpy as np


# In[ ]:


#create a system 
system = mb.System()
#read the mesh
N = 10 #pentagon size
vertex_file = 'vertices_N' + str(N) + '.inp'
face_file = 'faces_N' + str(N) + '.inp'
system.read_mesh_from_files(files={'vertices':vertex_file, 'faces':face_file})


# In[ ]:


#save the mesh to display
#create dumper
dump = system.dump() 
dump.vtk("initial mesh")


# In[ ]:


#add the evolver class where the potentials and integrators are added
evolver = mb.Evolver(system)


# In[ ]:


# first we need to know the edge length to move it appropiate:
compute = system.compute_mesh()
edge_lengths = compute.compute_edge_lengths()
avg_edge_length= np.mean(edge_lengths)
print("avg_edge_length = ", avg_edge_length)

#add the potentials in this case we will add bending and stretching 
# stretching potential
evolver.add_force("Mesh>Harmonic", {"k":{"0":str(50.0)}, 
                                    "l0":{"0":str(1.0)}})

# limit potential
evolver.add_force("Mesh>Limit", {"lmin":{"0":str(0.7)}, 
                                 "lmax":{"0":str(1.3)}})
# bending potential
evolver.add_force("Mesh>Bending", {"kappa":{"1":str(1.0)}})


#edges = system.getEdges()
#for edge in edges:
#    print(edge.type)


# In[ ]:


#add the monte carlo integrator
# vertex move:


evolver.add_integrator("Mesh>MonteCarlo>vertex>move", {"dr":str(0.005),
                                                       "seed":str(123949)})


# In[ ]:


## Now we want to have 100 snapshots every 1000 steps each
snapshots = 100
run_steps = 2000

## then we want to run the simulation for a temperature 1e-6
evolver.set_global_temperature(str(1e-3))

dump.vtk("pentagon_t0")
for snapshot in range(1, snapshots):
    acceptance = evolver.evolveMC(MCS=run_steps)
    print(acceptance)
    dump.vtk("pentagon_t" + str(snapshot*run_steps))

