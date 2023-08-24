#!/usr/bin/env python
# coding: utf-8

# # Multicomponent elastic membranes
# 
# 
# In this example we reproduce the results presented in [paper](). The model describes a bacteria microcomparment with three different types of proteins, which is characterized with hetereogeneous bending rigidities. Different from the previous [example](edgeswap.md), the heterogeneous properties are assigned to the vertices other than edges.
# 
# ## Table of contents
# 
# <dl>
#   <dt>By</dt>
#   <dd>Siyu Li</dd>
#   <dt>Type</dt>
#   <dd>Monte Carlo</dd>
# </dl>

# ### Three Component Vertex Swap
# 
# To begin we first import the necessary modulus and load the icosahedral mesh. The triangulation number is T=192 with (h,k)=(8,8).

# In[ ]:


#First we need to import the module for the simulations
import pymembrane as mb
#numpy
import numpy as np


# In[ ]:


#create a system 
system = mb.System()
#read the mesh
vertex_file = '03_vertexswap/vertices_T192.inp'
face_file = '03_vertexswap/faces_T192.inp'
system.read_mesh_from_files(files = {'vertices': vertex_file, 'faces': face_file})


# Then we assign different types to the vertices by using 'getVertices' and 'setVertices' functions:

# In[ ]:


fac=[0.6,0.3]
vertices = system.getVertices()
Nv=len(vertices)
id0s=np.random.choice(range(Nv),round(fac[0] * Nv),replace=False)
id1s=np.random.choice(list(set(range(Nv))-set(id0s)),round(fac[1] * Nv),replace=False)
id2s=list(set(range(Nv))-set(id0s)-set(id1s))

for vi in id0s: vertices[vi].type=0
for vi in id1s: vertices[vi].type=1
for vi in id2s: vertices[vi].type=2

system.setVertices(vertices)


# **Dump** initial triangulation mesh

# In[ ]:


#save the mesh to display
#create dumper
dump = system.dump() 
dump.vtk("initial mesh")


# Next we setup the potentials. The elastic property of the shell is homogeneous and is defined with **harmonic bonds** potential $E_{bond}=\sum_i k~(l_i-l_0)^2$, $i$ is the edge index.

# In[ ]:


#add the evolver class where the potentials and integrators are added
evolver = mb.Evolver(system)


# In[ ]:


evolver.add_force("Mesh>Harmonic", {"k": {"0": str(10.), "1": str(10.), "2": str(10.)},
                                    "l0": {"0": str(1.0), "1": str(1.0), "2": str(1.0)}})


# The **bending energy** is defined on the vertices based on Itzykson's discretization and [paper](https://journals.aps.org/pre/abstract/10.1103/PhysRevE.68.061905), where $E_{bend}=\sum_i^N \big(2\kappa_i (H_i-H_0 )^2+\kappa^g_i G_i\big) A_i$, $H$ and $G$ are the mean curvature and gauss curvature respectively. 
# This bending potential can be added through "bendingGK", with $H_0$, $\kappa$, $\kappa^g$ the spontaneous curvature, bending rigidity and gaussian rigidity.
# <!---The discretized formula $H_i = \frac{1}{2 A_i}\sum_{j\in N(i)}\big(\cot (\alpha_{ij})+\cot(\beta_{ij})\big)(\mathbf{x}_i-\mathbf{x}_j)$ and $G_i = 2\pi-\sum_i \theta_i$. ---> 

# In[ ]:


evolver.add_force("Mesh>BendingGK", {"H0": {"0": "0.", "1": "0.", "2": "0."},
                                     "kappaH": {"0": "4.", "1": "1.", "2": "0.8"},
                                     "kappaG": {"0": str(-0.66667 * 4.), "1": str(-0.66667 * 1.), "2": str(-0.66667 * 0.8)}})


# In addition, **Line Tension potential** is used to define the affinity between different types of vertices. The potential is defined as $E_{bend}=\sum_{<v_i,v_j>}\gamma~(1-\delta(m(v_i),m(v_j)))$ where $\gamma$ is the line tension coefficient which can be assigned:

# In[ ]:


evolver.add_force("Mesh>Linetension", {"gamma": {"0": "0.0004", "1": "0.0004", "2": "0.0004"}}) 


# Finally, same **limit edge** potential is added

# In[ ]:


evolver.add_force("Mesh>Limit", {"lmax": {"0": "1.3", "1": "1.3"},
                                 "lmin": {"0": "0.7", "1": "0.7"}})


# After assign all the potentials we add the **Monte Carlo** integrator
# 

# In[ ]:


evolver.add_integrator("Mesh>MonteCarlo>vertex>move", {"dr": "0.05"})
evolver.add_integrator("Mesh>MonteCarlo>vertex>swap", {"every step": "1"})


# In[ ]:


print(evolver.get_force_info())


# and use the annealing method to integrate

# In[ ]:


## Now we want to run 20000 steps and take snapshot every 10000 steps each
frame = 0
run_steps = 20000
snapshots = 10
anneal_steps = 5
templist = np.linspace(0.1, 10**-7, 10)

for anneal in range(anneal_steps):
    for tempindex, temp in enumerate(templist):
        ## we step the temperature first
        evolver.set_global_temperature(str(temp))
        for frame in range(snapshots):
            evolver.evolveMC(run_steps // snapshots)
            dump.vtk("vertex_swap_t" + str(frame))


# Finally we dump the state to json file
# 

# Download the [**script**](../../attached/vertexswap.py), [**initial vertice file**](../../attached/vertices_T192.inp), [**initial face file**](../../attached/faces_T192.inp).
