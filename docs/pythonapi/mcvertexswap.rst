Monte Carlo Vertex Swap
=======================

.. contents::
   :local:
   :depth: 2

Definition
-----------
.. list-table::
   :widths: 25 75
   :header-rows: 0

   * - **Name**
     - Mesh>MonteCarlo>vertex>swap
   * - **Type**
     - Monte Carlo
   * - **Defined in**
     - Vertices

Monte Carlo vertex swap integrator randomly picks two vertices and swaps their type ID. 
At each runtime, :math:`N_v` swaps are attempted. This is useful for simulations of multicomponent system where components can mix.

Python calling
--------------
**add**
.. code-block:: python

   evolver.add_integrator("Mesh>MonteCarlo>vertex>swap", {"every step": "1"})

**evolveMC(MCS)**
.. code-block:: python

   evolver.evolveMC(1000)

**set**
Set property separately.
.. code-block:: python

   evolver.set("Mesh>MonteCarlo>vertex>swap", {"every step": "5"})

**delete**
.. code-block:: python

   evolver.delete_integrator("Mesh>MonteCarlo>vertex>swap")

Property
--------
**T**
Temperature used in simulation.
   type: double
   default: 0.0

**seed**
Seed used to generate the random number.
   type: int
   default: 123456

**every step**
Perform the Monte Carlo movement every n steps.
   type: int

**stochastic_tunnelling**
   type: bool
   default: false

**gamma**
Stochastic gamma used for stochastic_tunnelling.
   type: double
   default: 0.0
