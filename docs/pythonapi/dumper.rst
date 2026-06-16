Dumper
=======

.. contents::
   :local:
   :depth: 2

Description
-----------

`Dumper` is the default Python dumper attached to `System.dumper`. Normal
dumping does not require VTK. The default `vtk()` and `edge_vtk()` methods
write legacy ASCII VTK files directly from Python, and `obj()` writes
Wavefront OBJ geometry.

The standard user-facing methods are:

.. code-block:: python

   system.dumper.vtk("mesh")
   system.dumper.edge_vtk("edges")
   system.dumper.json("mesh")
   system.dumper.ply("mesh")
   system.dumper.txt("mesh")
   system.dumper.obj("mesh")

For normal dumping:

- ``vtk(...)`` writes legacy ASCII VTK directly from Python.
- ``edge_vtk(...)`` writes edge geometry as legacy ASCII VTK lines.
- ``obj(...)`` writes Wavefront OBJ geometry.
- the VTK Python bindings are not required.

Constructor
-----------

- **Dumper(SystemClass)**: Constructor that requires an instance of the `SystemClass`.
  Example:
  .. code-block:: python

     dumper = Dumper(system_instance)

Methods
-------

- **vtk()**:
  Dump the mesh (vertices and faces) in vtk format.

- **edge_vtk()**:
  Dump the mesh (edges) in vtk format.

- **json()**:
  Dump the mesh (vertices and faces) in JSON format.

- **ply()**:
  Dump the mesh (vertices and faces) in PLY format.

- **txt()**:
  Dump the mesh (vertices and faces) in plain-text format.

- **obj()**:
  Dump the mesh (vertices and faces) in Wavefront OBJ format.

- **setvtkLegacyFormat()**:
  Backward-compatible no-op retained for older user scripts.

- **user_data_vertex(data_name: str, data: List[real])**:
  Dump user data on vertices. Overloaded to handle both a list of real values and real tensors.

- **user_data_face(data_name: str, data: List[real])**:
  Dump user data on faces. Overloaded to handle both a list of real values and real tensors.

- **user_data_edge(data_name: str, data: List[real])**:
  Dump user data on edges. Overloaded to handle both a list of real values and real tensors.
