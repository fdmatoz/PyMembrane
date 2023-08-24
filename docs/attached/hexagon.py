#!/usr/bin/env python
# coding: utf-8

# In[ ]:


#import the code
import pymembrane as mb
import numpy as np


# In[ ]:


#Create a system
system = mb.System()
#read the mesh
vertex_file = 'vertices.inp'
face_file = 'faces.inp'
system.read_mesh_from_files(files={'vertices':vertex_file, 'faces':face_file})


# In[ ]:


#save the mesh to d
#create dumper
dump = system.dump() 
dump.vtk("initial_mesh")


# # Halfedge structures
# 
# ![halfedge](./examples/images/he.gif "Halfedge")

# In[ ]:


# If you want to extract the vertices we do the following
vertices = system.getVertices()
#help(system)
#lets take a look at them
for vertex in vertices:
    print(vertex.r)


# In[ ]:


# no much information there since is a complex data structure
# lets get some help
help(vertices[0])


# In[ ]:


#so now try to print the positions of the vertices
for vertex in vertices:
    print(vertex.r , "or a list ", vertex.r.list())


# In[ ]:


#for the faces we do the same
faces = system.getFaces()
for face in faces:
    print(face.v1, face.v2, face.v3, face.refmetric())
help(faces[0])


# In[ ]:


#other methods
# first we need to know the edge length to move it appropiate:
compute = system.compute_mesh()
help(compute)


# In[ ]:


edge_lengths = compute.compute_edge_lengths()
avg_edge_length= np.mean(edge_lengths)
print("avg_edge_length = ", avg_edge_length)

