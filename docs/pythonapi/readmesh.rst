ReadMesh
========

Description
-----------

`ReadMesh` is a Python class providing functionalities to read mesh data from files. It offers methods to fetch vertices, edges, faces, and half-edges, and also to get their count.

Constructors
------------

- **ReadMesh(faces_file_path: str, vertices_file_path: str)**:
  Constructor that initializes the reader using paths to face and vertex files.
  Example:
  .. code-block:: python

     reader = ReadMesh("path_to_faces_file", "path_to_vertices_file")

- **ReadMesh(paths_map: Dict[str, str])**:
  Constructor that accepts a dictionary with keys 'faces' and 'vertices' mapped to their respective file paths.
  Example:
  .. code-block:: python

     reader = ReadMesh({"faces": "path_to_faces_file", "vertices": "path_to_vertices_file"})

Methods
-------

- **clear()**:
  Clears vectors used in reading. Note that this method clears the vectors in the `ReadMesh` class but not the mesh itself.

- **getVertices()** -> List[Vertex]:
  Retrieves the vertices. Returns a reference to the vertices.

- **getEdges()** -> List[Edge]:
  Retrieves the edges. Returns a reference to the edges.

- **getFaces()** -> List[Face]:
  Retrieves the faces. Returns a reference to the faces.

- **getHalfedges()** -> List[HalfEdge]:
  Retrieves the half-edges. Returns a reference to the half-edges.

- **getNumVertices()** -> int:
  Returns the number of vertices in the mesh.

- **getNumEdges()** -> int:
  Returns the number of edges in the mesh.

- **getNumFaces()** -> int:
  Returns the number of faces in the mesh.

- **getNumHalfedges()** -> int:
  Returns the number of
