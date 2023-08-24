Core Components
===============

PyMembrane operates mainly through two user-facing components: `System` and `Evolver`. These classes form the backbone of PyMembrane, and gaining a clear understanding of their operations is vital for users.

System Class
------------

The `System` class encompasses five specialized sub-classes:

- Three classes that construct the mesh: vertices, edges, and faces.
- Two classes that calculate the mesh's geometrical properties.
  
Additionally, this class offers functions that facilitate data exportation, enabling visualization and analysis of results.

Evolver Class
-------------

`Evolver` plays a central role within PyMembrane. It is responsible for controlling the system's dynamics, coordination of various forces, integrators, minimizers, and constraints. The primary duties of the Evolver class encompass:

- Keeping a structured record of `Forces`, `Integrators`, `Minimizers`, and `Constraints`.
- Controlling the system's evolution by computing energies, applying forces on vertices, executing integrators, and ensuring that all constraints are satisfied.

List of Core components 
------------------------

.. toctree::
   :maxdepth: 1
   
   system
   evolver
   triangularmeshes
   halfedge
   box
   dumper
   compute
