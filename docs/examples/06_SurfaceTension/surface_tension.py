import math
import pymembrane as mb
import numpy as np

np.finfo(np.dtype("float64"))


# Simulation of a droplet of water between two orthogonal cylindrical objects
# contact angle defines wettability of the solids
# radius defines cylinder radius relative to droplet radius
# spacing defines spacing between the two cylinder (if too far from each other, nothing appends)
#
# saves mesh in a .ply file every snapshot and end mesh
# return energy of the system at the end
def orthogonal_cylinders(vertex_file, face_file, contact_angle=27, run_steps=4000, radius=15, spacing=1, max_snapshots=10):
    
    delta_gamma = math.cos(math.radians(contact_angle))
    # Create a system 
    box = mb.Box(10,10,10)
    system = mb.System(box)
    system.read_mesh_from_files(files={'vertices':vertex_file, 'faces':face_file})

    # Create a dumper to export mesh
    dump = system.dumper

    # Create the evolver to run simulation
    evolver = mb.Evolver(system)
    compute = system.compute

    # Add surface tension
    evolver.add_force("Mesh>SurfaceTension", {"gamma": {"1": str(1.0), "2":str(delta_gamma)}})
        
    position = radius + spacing/2.0
    
    # Define how many cylinders in the simulation
    # Then each is defined by an axis, a point on their axis, and their radius
    evolver.add_force("Mesh>SubstrateCylinder", {"nb_cylinder": {"0": str(2.0)},
                                                 "axis": {"0": str(1.0),"1": str(0.0), "2": str(0.0), "3": str(0), "4": str(0.0),"5": str(1.0)},
                                                 "position": {"0": str(0.0), "1": str(-position), "2": str(0.0), "3": str(0.0), "4": str(position), "5": str(0.0)},
                                                 "radius": {"0": str(radius), "1": str(radius)}})

    initial_volume = compute.volume()
    print("volume initial : {}".format(initial_volume))
    # Add volume constraint (constant volume)
    evolver.add_constraint("Mesh>Volume", {"V": str(initial_volume), "tol":str(initial_volume/100.0), "max_iter":str(1000)})

    evolver.add_integrator("Mesh>Brownian>vertex>move", {"seed":"202208"})
    
    
    #Note for the velocity brownian integrator we need to set the mass of each vertex
    vertices = system.vertices
    for vertex in vertices:
        vertex.mass = 1.0
        
    system.vertices = vertices

    dt = str(1e-3)
    evolver.set_time_step(dt)

    evolver.evolveMD(steps=1)
    dump.ply("cyl_t0")
    print("[start]-------------------------------------------")
    not_converged = True
    snapshot = 1
    previous_contact_area = 0
    previous_total_area = 0
    while (not_converged):
        evolver.evolveMD(steps=run_steps)
        dump.ply("cyl_t" + str(snapshot))
        contact_area = compute.area_typed(2)
        total_area = compute.area()
        
        print("[snapshot_{}]-------------------------------------------(max {})".format(snapshot, max_snapshots))
        if snapshot > max_snapshots or (abs(previous_contact_area - contact_area) < 0.1 and abs(previous_total_area - total_area) < 0.1):
            not_converged = False
        snapshot += 1
        previous_contact_area = contact_area
        previous_total_area = total_area
        
    name_type = "Results/"
    
    dump.ply(name_type+"r_{}_a_{}_s_{:.2f}".format(radius, contact_angle, spacing))
    energy = compute.energy(evolver)['faces']
    return energy

#####################################################################################


# for a faster simulation replace 'medium' by 'simple' (simpler inital mesh)
# for a more precise simulation replace 'medium' by 'refined' (more complex inital mesh)
vertex_file = 'initial_mesh_medium_vertices.inp'
face_file = 'initial_mesh_medium_faces.inp'
contact_angle = 27
radius_cyl = 10
fiber_spacing = 1.0

energy = orthogonal_cylinders(vertex_file=vertex_file, face_file=face_file, contact_angle=contact_angle, radius=radius_cyl, spacing=fiber_spacing)
print('energy = {:.3f}'.format(energy))