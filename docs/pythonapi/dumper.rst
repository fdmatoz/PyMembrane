Dumper
=======

.. contents::
   :local:
   :depth: 2

Description
-----------

`Dumper` is a Python class interfacing with the underlying C++ `DumperClass`. It enables exporting mesh data in various formats and handling custom user data.

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

- **setvtkLegacyFormat()**:
  Set the vtk legacy format flag.

- **user_data_vertex(data_name: str, data: List[real])**:
  Dump user data on vertices. Overloaded to handle both a list of real values and real tensors.

- **user_data_face(data_name: str, data: List[real])**:
  Dump user data on faces. Overloaded to handle both a list of real values and real tensors.

- **user_data_edge(data_name: str, data: List[real])**:
  Dump user data on edges. Overloaded to handle both a list of real values and real tensors.

