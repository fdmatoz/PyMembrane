.. _system-class:

System Class
============

.. contents::
   :local:
   :depth: 2

**PyMembrane**  uses a modular design where different components act as autonomously as possible. The System class encompasses five independent classes, which include three that form the :doc:`mesh<triangularmeshes>` (vertices, edges, and faces), two supplementary classes to :doc:`compute geometrical properties <compute>`  over the mesh, and functions for `data export for visualization and analysis<dumper>`.


Before you start
-----------------

.. code-block:: python

   # import pymembrane
   import pymembrane as mb

   # create a new system instance
   system = mb.System()

Loading my first mesh
---------------------

**Pymembrane** works with meshes in _inp_ format. For example, a vertex file for a hexagon looks like:

::

   Vertex File (id x y z type):

   0 0.0 0.0 0.0 1
   1 1.0 0.0 0.0 1
   2 0.5 0.8660254038 0.0 1
   3 -0.5 0.8660254038 0.0 1
   4 -1.0 0.0 0.0 1
   5 -0.5 -0.8660254038 0.0 1
   6 0.5 -0.8660254038 0.0 1

and the corresponding face file:

::

   Face File (id v1 v2 v3 normal):

   0 0 1 2 1
   1 0 2 3 1
   2 0 3 4 1
   3 0 4 5 1
   4 0 5 6 1
   5 0 6 1 1

Now that we have the faces and vertices files we can create a mesh as follows:

.. code-block:: python

   vertex_file = 'vertices.inp'
   face_file = 'faces.inp'
   system.read_mesh_from_files(files={'vertices':vertex_file, 'faces':face_file})

Visualization
-------------

**PyMembrane** can export meshes to `vtk <https://en.wikipedia.org/wiki/VTK>`_ and `ply <https://en.wikipedia.org/wiki/PLY_(file_format)>`_ files. To create a dumper for a given mesh, we need to do:

.. code-block:: python

   dump = system.dump()
   dump.ply("hexagon")      #for ply
   dump.vtk("hexagon")      #for vtk

.. image:: ../images/00_hexagon.png
   :alt: hexagon
   :align: center

In addition, the _vtk_ format can be set to legacy format by:

.. code-block:: python

   dump.setvtkLegacyFormat(True)

.. note::

   _VTK_ and _json_ format is desirable since all the properties of the mesh are fully exported.

Visualization
-------------

PyMembrane offers various mesh-related computations using the compute class. To execute computations for a given mesh, follow the steps:

.. code-block:: python

   mesh_compute = system.compute()
   edge_lengths = mesh_compute.edge_lengths()
   face_areas = mesh_compute.face_areas()
   face_normals = mesh_compute.face_normals() #to be stored in the face class

Additionally, `compute` class can be used to compute thermodynamic properties of the mesh. For example, the following code computes the pressure of a given mesh:

.. code-block:: python

   mesh_compute.pressure(evolver)

.. note::

   To calculate thermodynamic quantities, one must define an :doc:`Evolver<evolver>` class. This necessity arises because thermodynamic properties are intrinsically linked to the force potentials in the system. Although the `compute<compute>` class operates independently and isn't a subset of the `system` class, it has been integrated with the `system` class for user convenience. This setup facilitates the computation of various properties for a specified mesh.