#!/usr/bin/env python
# coding: utf-8

# # Multicomponent elastic membranes

# In this example we reproduce the simulation of [Vernizzi, et al](https://www.pnas.org/content/108/11/4292)
# The system contains a spherical shell with two different type of edges.

# ## Table of contents

# <dl>
#   <dt>By</dt>
#   <dd>Siyu Li</dd>
#   <dt>Type</dt>
#   <dd>Monte Carlo</dd>
# </dl>

# In[ ]:


#First we need to import the module for the simulations
import pymembrane as mb
#numpy
import numpy as np


# #### Loading Mesh
# We first load the icosahedral mesh, the triangulation number used in the paper is T=189 with (h, k)=(12, 3). The input file can be found in the end of this section.

# In[ ]:


#create a system 
system = mb.System()
#read the mesh
vertex_file = 'vertices_T189.inp'
face_file = 'faces_T189.inp'
system.read_mesh_from_files(files = {'vertices': vertex_file, 'faces': face_file})


# #### Set up system
# We then set up the edge IDs. One way to set up edge ids is use the 'meterial' object, here we present an alternative way by using the 'getEdges' and 'setEdges' functions. Below we assign $30\%$ edges as type 0 and $70\%$ edges as type 1. 

# In[ ]:


fac = 0.7
edges = system.getEdges()
Ne = len(edges)
id0s = np.random.choice(range(Ne),round(fac * Ne), replace = False)
id1s = list(set(range(Ne)) - set(id0s))
for li in id1s: edges[li].type = 1
for li in id0s: edges[li].type = 0
system.setEdges(edges)


# At this stage we could export the initial mesh to vtk format by 'dump' function,  one could then visualize the mesh by open 'mesh_edges.vtp' in **Paraview** and select the 'edge type'

# In[ ]:


#save the mesh to display
#create dumper
dump = system.dump() 
dump.vtk("initial mesh")


# <img src="../../images/twoedge-3-7.png" width="200">

# Now we setup the hetereogeneous properties assigned to different edge type. The total energy of the shell is a sum of harmonic bond energy and bending potential, defined as $E_{bond}=\sum_i k~(l_i-l_0)^2$ and $E_{bend}=\sum_{<i,j>}\frac{\kappa}{2}~(\hat{n}_i-\hat{n}_j)^2$, where the $k$, $l_0$ and $\kappa$ are the spring constant, equilibrium length and bending rigidity respectively. The potentials are added in the 'add_potential' function, in the third argument two types of edges are defined with same stretching property but different bending rigidities.

# In[ ]:


#add the evolver class where the potentials and integrators are added
evolver = mb.Evolver(system)


# In[ ]:


evolver.add_force("Mesh>Harmonic", {"k":{"0": "5.77", "1": "5.77"}, 
                                    "l0":{"0": "1.0", "1": "1.0"}})

evolver.add_force("Mesh>Bending", {"kappa":{"0": "0.06", "1": "28.9"}})


# The limit edge potential
# $E_{limit}=\infty$, $l>l_{max}$ or $l<l_{min}$
# is also added to prevent the large deformation

# In[ ]:


evolver.add_force("Mesh>Limit", {"lmax":{"0": "1.3", "1": "1.3"},
                                 "lmin":{"0": "0.7", "1": "0.7"},})


# After assign all the potentials to the system we now need to add the ***Monte Carlo*** integrator. In the following example, each Monte Carlo run will contain $N_v$ attempts move of vertice and $N_v$ attempts of edge swaps.

# In[ ]:


evolver.add_integrator("Mesh>MonteCarlo>vertex>move", {"dr":"0.05"})
evolver.add_integrator("Mesh>MonteCarlo>edge>swap", {"every step": "1"})


# In[ ]:


print(evolver.get_force_info())


# Finally we use annealing method to integrate the monte carlo simulation. Five cooling cycles are used below where each cycle containes a linear decreasing of temperature.

# In[ ]:


## Now we want to run 100000 steps and take snapshot every 10000 steps each
frame = 0
run_steps = 200
snapshots = 10
anneal_steps = 5
templist = np.linspace(0.1, 10**-7, 10)

for anneal in range(anneal_steps):
    for tempindex, temp in enumerate(templist):
        ## we step the temperature first
        evolver.set_global_temperature(str(temp))
        for frame in range(snapshots):
            evolver.evolveMC(run_steps // snapshots)
            dump.edge_vtk("edge_swap_t" + str(frame))
            dump.vtk("edge_swap_t" + str(frame))


# #### Visualization in Paraview

# ![twocomp_fac](../images/twocomp_fac.pdf)
