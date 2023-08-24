Monte Carlo Edge Swap
=====================

.. contents::
   :local:
   :depth: 2

Definition
----------
.. list-table::
   :widths: 25 75
   :header-rows: 0

   * - **Name**
     - Mesh>MonteCarlo>edge>swap
   * - **Type**
     - Monte Carlo
   * - **Defined in**
     - Edges

Monte Carlo edge swap integrator randomly picks two edges and swaps their type ID. 
At each runtime, :math:`N_e` swaps are attempted. This is useful for simulations of multicomponent system where components can mix.

Python calling
--------------
**add**
.. code-block:: python

   evolver.add_integrator("Mesh>MonteCarlo>edge>swap", {"every step": "1"})

**evolveMC(MCS)**
.. code-block:: python

   evolver.evolveMC(1000)

**set**
.. code-block:: python

   evolver.set("Mesh>MonteCarlo>edge>swap", {"every step": "2"})

**delete**
.. code-block:: python

   evolver.delete_integrator("Mesh>MonteCarlo>edge>swap")

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
