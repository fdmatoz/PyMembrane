.. _evolver-class:

Evolver Class
==================

.. contents::
   :local:
   :depth: 2


The `Evolver` class stands as a central component in the design and implementation of the PyMembrane framework. Its primary role is to oversee the dynamics of the simulated system. To achieve this, the `Evolver` class maintains a comprehensive list of Forces, Integrators, Minimizers, and Constraints. 

By capturing the essence of system dynamics through these elements, `Evolver` ensures that the behavior of the `System`.  The Evolver class is a key part of PyMembrane in short:

 1. It keeps track of Forces, Integrators, Minimizers, and Constraints.
 2. It controls how the system changes over time by applying forces, integrators, minimizers, and constraints.


Creating an evolver class
-------------------------

.. code-block:: python

   import pymembrane as mb
   # create a new system instance
   system = mb.System()
   vertex_file = 'hexagon_vertices.inp'
   face_file = 'hexagon_faces.inp'
   system.read_mesh_from_files(files={'vertices':vertex_file, 'faces':face_file})

   # create the evolver class
   evolver = mb.Evolver(system)

The Python interface of the Evolver class define the following methods that the user needs to get familiar with:

Evolver methods
---------------

Main methods
^^^^^^^^^^^^

- `add_force(...)`

  1. 
    .. code-block:: python

       add_force(self: pymembrane.cppmodule.cppmodule.Evolver, arg0: str, arg1: Dict[str, Dict[str, str]]) -> None
  2.
    .. code-block:: python

       add_force(self: pymembrane.cppmodule.cppmodule.Evolver, arg0: str, arg1: Dict[str, Dict[Tuple[str, str], str]]) -> None

- `delete_force(...)`: delete force method

  .. code-block:: python

     delete_force(self: pymembrane.cppmodule.cppmodule.Evolver, arg0: str) -> None

- `add_integrator(...)`: 

  .. code-block:: python

     add_integrator(self: pymembrane.cppmodule.cppmodule.Evolver, arg0: str, arg1: Dict[str, str]) -> None


- `delete_integrator(...)`: delete integrator method

  .. code-block:: python

     delete_integrator(self: pymembrane.cppmodule.cppmodule.Evolver, arg0: str) -> None

- `add_constraint(...)`: add constraint method

  .. code-block:: python

     add_constraint(self: pymembrane.cppmodule.cppmodule.Evolver, arg0: str, arg1: Dict[str, str]) -> None

- `delete_constraint(...)`: delete constraint method

  .. code-block:: python

     delete_constraint(self: pymembrane.cppmodule.cppmodule.Evolver, arg0: str) -> None

- `set(...)`: set/modify properties for integrators/forces/minimizers/constraints

  1. 
    .. code-block:: python

       set(self: pymembrane.cppmodule.cppmodule.Evolver, arg0: str, arg1: Dict[str, Dict[Tuple[str, str], str]]) -> None
  2.
    .. code-block:: python

       set(self: pymembrane.cppmodule.cppmodule.Evolver, arg0: str, arg1: Dict[str, Dict[str, str]]) -> None
  3.
    .. code-block:: python

       set(self: pymembrane.cppmodule.cppmodule.Evolver, arg0: str, arg1: Dict[str, str]) -> None


Temperature and time methods
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In addition to the methods defined above, the Evolver class also provides a way to propagate important information into the all of the integrators at once. The latter is important when more than one integrators is present in the system. In the same way for molecular dynamics the time step needs to be set globally. 


- `set_global_temperature(...)`: set the temperature in the whole system

  .. code-block:: python

     set_global_temperature(self: pymembrane.cppmodule.cppmodule.Evolver, arg0: str) -> None

- `set_time_step(...)`: set the time step for Molecular Dynamics methods

  .. code-block:: python

     set_time_step(self: pymembrane.cppmodule.cppmodule.Evolver, arg0: str) -> None

Dynamics methods
^^^^^^^^^^^^^^^^

As we mentioned earlier, the Evolver class is responsible for the dynamics of the system. The following methods are used to propagate the system forward in time.


- `evolveMC(...)`: Evolve the mesh following Monte Carlos methods loaded. Arg: steps Return: accepted moves

  .. code-block:: python

     evolveMC(self: pymembrane.cppmodule.cppmodule.Evolver, arg0: int) -> int

- `evolveMD(...)`: evolve the mesh following Molecular Dynamics methods loaded

  .. code-block:: python

     evolveMD(self: pymembrane.cppmodule.cppmodule.Evolver, arg0: int) -> None

- `enforce_mesh_constraints(...)`: enforce constraint method

  .. code-block:: python

     enforce_mesh_constraints(self: pymembrane.cppmodule.cppmodule.Evolver) -> None

Other methods
^^^^^^^^^^^^^^

Other methods that are not required to be used by the user are unless debugging or testing, for example, the following methods are used to compute the mesh energy and forces.

- `compute_mesh_energy(...)`: compute the mesh energy and store in the mesh

  .. code-block:: python

     compute_mesh_energy(self: pymembrane.cppmodule.cppmodule.Evolver) -> None

- `compute_mesh_forces(...)`: compute the mesh forces and store in the mesh

  .. code-block:: python

     compute_mesh_forces(self: pymembrane.cppmodule.cppmodule.Evolver) -> None


- `reset_mesh_energy(...)`: reset the mesh energy

  .. code-block:: python

     reset_mesh_energy(self: pymembrane.cppmodule.cppmodule.Evolver) -> None

- `reset_mesh_forces(...)`: reset the mesh forces

  .. code-block:: python

     reset_mesh_forces(self: pymembrane.cppmodule.cppmodule.Evolver) -> None

To know more about the Evolver class, please refer to contextual help in python:

.. code-block:: python
  
   help(Evolver)