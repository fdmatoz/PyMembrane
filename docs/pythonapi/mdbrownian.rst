Brownian
========

.. contents::
   :local:
   :depth: 2
   
Definition
-----------
.. list-table::
   :widths: 25 75
   :header-rows: 0

   * - **Name**
     - Mesh>brownian
   * - **Type**
     - Brownian
   * - **Defined in**
     - Vertices

Brownian vertex integrator moves vertices following brownian dynamics.

Brownian dynamics assumes overdamped Langevin dynamics and solves the set of first-order equations for the position of each vertex :math:`\mathbf{r}_i`:

.. centered::
   :class: no-mathjax

   $\gamma \dot{\mathbf{r}}_{i} = \mathbf{F} + \mathbf{\eta}_{i} \left( t \right)$

Where :math:`\mathbf{\eta}_{i} \left( t \right) \in \mathbb{R}^{3}` is a weak random noise obeying:

- $\langle \mathbf{\eta}_{i} \rangle = 0$
- $\langle \eta_{i}^{m} \left( t \right) \eta_{j}^{n} \left( t^{\prime} \right) \rangle = \sqrt{2\gamma k_{B}T} \delta_{ij} \delta_{mn} \delta \left( t - t^{\prime} \right)$

Where :math:`m, n \in \{x, y, z\}`.

Here, :math:`T` is the temperature and :math:`\gamma` is the friction coefficient.

Python calling
--------------
**add**
.. code-block:: python

   evolver.add_integrator("Mesh>brownian", {"T": "0.2"})

**run**
.. code-block:: python

   evolver.evolveMD()

**set**
.. code-block:: python

   evolver.set("Mesh>brownian", {"gamma": "1.0"})

**delete**
.. code-block:: python

   evolver.delete_integrator("Mesh>brownian")

Property
--------
**T**
Temperature used in simulation.
   type: double
   default: 0.0

**gamma**
Friction coefficient.
   type: double
   default: 1.0

**seed**
Seed used to generate the random number.
   type: int
   default: 123456
