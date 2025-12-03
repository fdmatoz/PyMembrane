import pymembrane as mb
import numpy as np
import random

vertex_file = './Input/Vertices_ico2562.txt'
faces_file = './Input/Edges_ico2562.txt'

# System
system = mb.System()
system.read_mesh_from_files(files={'vertices':vertex_file, 'faces':faces_file})

faces = system.faces
for face in faces:
    face.type = 0
system.setFaces(faces)
edges = system.edges

rad_particle=0.1
overl_factor=2
random.seed(0)  
tot_hole_vertices=0
vertices = system.vertices

for vertex in vertices:
    vertex.type=-1  #Initialize the system with no particles, then add particles in.
    vertex.mass=-1  #Mass attribute is used to point to edge vertices in a set of vertices covered by a particle. 
                    #Initialize it at -1.

for vertex in vertices:
    if random.random()<0.2:    # This factor varies the area fraction of droplet-attached particles. Note, that the 
                               # generated particle area fraction depends on the random number sequence and will
                               # slightly vary between runs (provided that the random.seed number is changed each time).
        overlaps=0
        for vertex_j in vertices:
            if vertex_j!=vertex and vertex_j.type!=-1:
                dist_ij=((vertex.r.x-vertex_j.r.x)**2+(vertex.r.y-vertex_j.r.y)**2+(vertex.r.z-vertex_j.r.z)**2)**0.5;
                if dist_ij<rad_particle*overl_factor:
                    overlaps=1
        if overlaps==0:    #If no overlap with other particles, "vertex" is chosen as a center of a new droplet-attached particle.
            vertex.type = vertex.id   # The central vertex in a set of vertices covered by a particle has its type value 
                                      # equal to its id. This "flag" allows the central vertices of each particle to be 
                                      # distinguished from all other particles.
            for vertex_j in vertices:
                if vertex_j!=vertex and vertex_j.type==-1:
                    dist_ij=((vertex.r.x-vertex_j.r.x)**2+(vertex.r.y-vertex_j.r.y)**2+(vertex.r.z-vertex_j.r.z)**2)**0.5;
                    if dist_ij<rad_particle:    # vertex_j is covered by the particle centered at "vertex".
                        vertex_j.type=vertex.id  # The type field of a vertex covered by a particle holds the id of the vertex which is at the center of the relevant particle.     
            last_bndry_prt=-1                       
            bndry_part_i=0
            tot_hole_vertices+=1
            for vertex_j in vertices:           # This loop is used to specially mark all edge vertices belonging to the same particle.
                                                # These vertices typically interact with particle-free vertices, so that they are particularly important.
                if vertex_j!=vertex and vertex_j.type==vertex.id:
                    he_first=vertex_j._hedge
                    he=he_first
                    condition = True
                    hole_bndry_vertex = False
                    while condition:
                        edge_index=system.halfedges[he].edge
                        v1 = edges[edge_index].i
                        v2 = edges[edge_index].j
                        t1 = vertices[v1].type
                        t2 = vertices[v2].type
                        he_pair = system.halfedges[he].pair
                        he = system.halfedges[he_pair].next
                        condition = (he!=he_first)
                        if ((t1<0 and t2>=0) or (t2<0 and t1>=0)):
                            hole_bndry_vertex=True
                    if hole_bndry_vertex :
                        bndry_part_i=bndry_part_i+1
                        if bndry_part_i==1:
                            vertex.mass=vertex_j.id   # Use .mass attribute to point to "the first" edge vertex of the hole.
                                                      # Then, each edge vertex points to the next edge vertex, using its .mass attribute.
                                                      # The last edge vertex has mass=-1 (yet, it's type is equal to vertex.id, so that it is easily
                                                      # distinguished from either free vertices or vertices covered by other particles.
                        else:
                            vertices[last_bndry_prt].mass=vertex_j.id
                        last_bndry_prt=vertex_j.id
system.setVertices(vertices)
print(f"Particles created = {tot_hole_vertices} Area Fraction ={tot_hole_vertices*rad_particle**2/4}")   #Display the actual area fraction of surface-piercing particles.

for edge in edges:
    edge.type = 0

system.setEdges(edges)

compute = system.compute_mesh()

edge_lengths = compute.edge_lengths()
avg_edge_length= np.mean(edge_lengths)
print(f"avg = {np.mean(edge_lengths)} max ={np.max(edge_lengths)} min = {np.min(edge_lengths)}")

evolver = mb.Evolver(system)

parameters = {"k" : {"0": str(2)},"l0": {"0": str(0.06955)},"J": {"0": str(0)},"Ea": {"0": str(0)}}

evolver.add_force("Mesh>Limit", {"lmax": {"0": str(1.8*np.max(edge_lengths))},\
                                  "lmin": {"0": str(0.7*np.min(edge_lengths))}})

evolver.add_force("Mesh>HarmonicDye", parameters)
evolver.add_force("Mesh>Bending", {"kappa":{"0":str(0.0001)}})

evolver.add_integrator("Mesh>MonteCarlo>vertex>move", {"T":str(1e-5),"dr": str(0.001),"seed": str(123949)})
evolver.add_integrator("Mesh>MonteCarlo>edge>flip", {"T":str(3.5e-4),"every step": "1"})

#No global temperature is defined here. Two separate temperatures were defined for the two evolvers.
#evolver.set_global_temperature(str(1e-5))   

snapshots = 10
run_steps = 2000
dump = system.dump() 
dump.vtk("initial_mesh")
for snapshot in range(1, snapshots):
    acceptance = evolver.evolveMC(steps=run_steps)
    compute = system.compute_mesh()
    print(compute.energy(evolver)["edges"])
    dump.vtk(f"Particle_on_Sphere_t{str(snapshot*run_steps)}")

for snapshot in range(11, 50):
    acceptance = evolver.evolveMC(steps=run_steps)
    compute = system.compute_mesh()
    print(compute.energy(evolver)["edges"])
    dump.vtk(f"Particle_on_Sphere_t{str(snapshot*run_steps)}")

Tinit=3.5e-4
Tfin=1e-5
Tstep=1e-5
Nsteps=round((Tinit-Tfin)/Tstep+1)
for Tcur_step in range(0,Nsteps):
    Tcur=Tinit-Tstep*Tcur_step
    evolver.delete_integrator("Mesh>MonteCarlo>edge>flip")
    evolver.add_integrator("Mesh>MonteCarlo>edge>flip", {"T":str(Tcur),"every step": "1"})
    for snapshot in range(151+(Tcur_step-1)*10, 151+Tcur_step*10):
        acceptance = evolver.evolveMC(steps=run_steps)
        compute = system.compute_mesh()
        print(compute.energy(evolver)["edges"])
        dump.vtk(f"Particle_on_Sphere_t{str(snapshot*run_steps)}")


