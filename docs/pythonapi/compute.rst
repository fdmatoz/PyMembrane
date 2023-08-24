Compute Mesh Properties
=======================

.. contents::
   :local:
   :depth: 2

Description
-----------

`ComputeMesh` is a Python class interfacing with the C++ backend. It provides various computational functions for a mesh.

Constructor
-----------

- **ComputeMesh(SystemClass)**: Constructor that requires an instance of the `SystemClass`.
  Example:
  .. code-block:: python

     compute = ComputeMesh(system_instance)

Methods
-------

- **face_normals()**:
  Compute face normals and store it in the face.normal.

- **vertex_normals(vertex_normal_angle_weight=False)**:
  Compute vertex normals and store it in the vertex.normal. Optional argument to define vertex normal angle weight.

- **edge_lengths()**:
  Compute the edge lengths.

- **gaussian_curvature()**:
  Compute the Gaussian curvature.

- **mean_curvature()**:
  Compute the mean curvature.

- **volume()**:
  Compute the mesh volume. It requires closed surfaces.

- **face_area()**:
  Compute the face area.

- **area()**:
  Compute the mesh area.

- **face_metric()**:
  Compute the face metric.

- **vertex_area()**:
  Compute the vertex area.

- **energy(Evolver)**:
  Compute the energy of the mesh. It requires an instance of the `EvolverClass`. Returns the sum of the energy defined for vertices, edges, and faces.

- **forces(Evolver)**:
  Compute the forces on the mesh vertices. It requires an instance of the `EvolverClass`.

- **stresses(Evolver, IncludeVel=False)**:
  Compute the forces on the mesh. It requires an instance of the `EvolverClass` and an optional boolean argument to include the velocities. Returns a 9 components tensor.

- **kinetic_energy_tensor()**:
  Compute the kinetic energy tensor on the mesh. Returns a 9 components tensor.

- **kinetic_energy()**:
  Compute the kinetic energy of the mesh.

- **temperature()**:
  Compute the temperature in the system.

- **pressure(Evolver)**:
  Compute the pressure in the system. It requires `EvolverClass`.

- **stresses_virial(Evolver, IncludeVel=False)**:
  Compute the stresses virial on the mesh. It requires `EvolverClass` and an optional boolean argument to include the velocities. Returns a 9 components tensor.

- **stresses_atom(Evolver, IncludeVel=False)**:
  Compute the stresses on individual atoms in the mesh. It requires an instance of the `EvolverClass` and an optional boolean argument to include the velocities. Returns a 9 components tensor of `Numvertices` size.

