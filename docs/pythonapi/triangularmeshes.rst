Triangular Meshes
=================

.. contents::
   :local:
   :depth: 3

Triangular meshes are cornerstone structures widely utilized in computational geometry, computer graphics, and several scientific disciplines. They offer a discrete representation of surfaces, whether they're simple flat shapes or intricate curved forms, spanning both two-dimensional (2D) and three-dimensional (3D) spaces. Such a representation proves indispensable for tasks like modeling, simulation, and visualization. Within PyMembrane, triangular meshes assume a critical function in simulating coarse-grained elastic systems. Their primary applications include:


* **Coarse-Grained Simulations**: PyMembrane leverages triangular meshes to model and simulate elastic behaviors at a macroscopic scale, abstracting away microscopic details.
* **Elastic System Dynamics**: The mesh aids in simulating how the system responds to external forces, constraints, and other dynamic factors.
* **Visualization**: Through PyMembrane's tools, users can visualize the mesh, offering insights into the system's behavior and properties.


Components of Triangular Meshes
------------------------------

1. **Vertices (V)**:
    - *Definition*: Vertices are the fundamental points in the mesh, acting as the corner points of triangles.
    - *Role*: They serve as the primary positional data for the mesh. Each vertex contains coordinates in 3D space, and other attributes like **mass**, **type**, and **normals**.

2. **Edges (E)**:
    - *Definition*: Edges are the straight lines connecting two vertices.
    - *Role*: Edges define the structure and connectivity of the mesh. An edge in a triangular mesh always belongs to either one or two triangles.

3. **Faces (F)**:
    - *Definition*: A face, in the context of a triangular mesh, is a triangle formed by connecting three vertices with three edges.
    - *Role*: Faces define the surface of the mesh. They represent the a triangular structure with vertices **v1**, **v2**, and **v3**. Each face has normal vector, **normal**, **type**.

In **PyMembrane**, understanding the structure and interplay of components within a triangular mesh is crucial. The accompanying figure offers a clear visualization of these elements - vertices, edges, and faces - highlighting their significance in driving the simulations. Please, refer to the illustrated figure:

.. figure:: ../images/triangular_mesh.svg
   :width: 500
   :align: center
   :alt: Triangular Mesh


PyMembrane's Triangular Mesh Classes
------------------------------------

.. note::
    The following explain in brief each of the methods for Face, Vertex, and Edge classes. Note that not all of this method can be modefied by the user. Some of them are read-only. For more details, please refer to the contextual help as so:

    .. code-block:: python
       
       import pymembrane as mb
       help(mb.Vertex) # for Vertex class
       help(mb.Face) # for Face class
       help(mb.Edge) # for Edge class 


Looping over vertices
----------------------

As an example, let's say that we want to deform the mesh that we just loaded. To do this, we need to access the vertices in the `System` class as follows:

.. code-block:: python

   # Retrieve the vertices
   vertices = system.vertices
   for index in range(len(vertices)):
       vertex = vertices[index]
       vertex.r.x += 0.1
       vertex.r.y -= 0.3
       vertices[index] = vertex

   # Set the new vertices
   system.vertices = vertices

   # Now we can visualize the new mesh
   dump.vtk("hexagon_vertex_move")      #for vtk

Vertex Attributes
^^^^^^^^^^^^^^^^^

- `boundary`: Vertex True if it is at the boundary
- `coordination`: Vertex Coordination
- `energy`: Conservative Energy
- `forceC`: Conservative Force
- `forceD`: Dissipative Force
- `id`: Vertex id
- `mass`: mass
- `normal`: normal
- `property`: Properties
- `r`: Vertex position
- `type`: type
- `v`: Vertex velocity

Looping over faces
------------------

As an example, let's say that we want to change the face type in the mesh that we just loaded. To do this, we need to access the faces in the :doc:`System<system>` class as follows:

.. code-block:: python

   # Retrieve the faces
   faces = system.faces
   for index in range(len(faces)):
       face = faces[index]
       face.type = 3
       faces[index] = face

   # Set the new faces
   system.faces = faces

   # Now we can visualize the new mesh
   dump.vtk("hexagon_face_type")      #for vtk

Face Attributes
^^^^^^^^^^^^^^^

- `area`: area of the face
- `boundary`: True if it is at the boundary
- `energy`: Conservative Energy
- `id`: face id
- `normal`: normal to that face
- `nsides`: number of sides the face has
- `outer`: True if face is a ghost outer face
- `property`: properties
- `reference_normal`: reference normal to that face
- `type`: types
- `v1`: one of vertices that define the face
- `v2`: one of vertices that define the face
- `v3`: one of vertices that define the face

Looping over edges
------------------

.. code-block:: python

   edges = system.edges
   for index in range(len(edges)):
       edge = edges[index]
       edge.type = 3
       edge[index] = edge

   # Set the new faces
   system.edge = edge

Edge Attributes
^^^^^^^^^^^^^^^

- `id`: The unique identifier for the edge.

- `i`: One of the connected vertices.

- `j`: One of the connected vertices.

- `boundary`: A boolean indicating if the edge is at the boundary of the mesh.

- `face_k`: The index to one of the faces shared by this edge. Returns `-1` if it's an outer face.

- `face_l`: The index to one of the faces shared by this edge. Returns `-1` if it's an outer face.

- `v0`: A vertex around the edge.

- `v1`: A vertex around the edge.

- `v2`: A vertex around the edge.

- `v3`: A vertex around the edge.

- `_hedge`: The halfedge index.

- `type`:  The type of the edge.

- `energy`: The conservative energy associated with the edge.