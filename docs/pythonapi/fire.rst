Fire Minimizer
==============

.. contents::
   :local:
   :depth: 2

Definition
----------
.. list-table::
   :widths: 25 75
   :header-rows: 0

   * - **Name**
     - Mesh>Fire
   * - **Type**
     - Monte Carlo
   * - **Defined in**
     - Vertex

The minimizer uses the FIRE algorithm to minimize energy :cite:`bitzek2006fire`.

Python calling
--------------
**add**
.. code-block:: python

   evolver.add_minimizer("Mesh>Fire", {"dt": "0.01", "max_iter": "100000", "ftol": "0.0001", "etol": "0.0000001"})

**run**
.. code-block:: python

   evolver.minimize()


**get minimizer information**

.. code-block:: python

   evolver.get_minimizer_info()

**set**

.. code-block:: python

   evolver.set("Mesh>Fire", {"dt": "0.1"})

**delete**

.. code-block:: python

   evolver.delete_minimizer("Mesh>Fire")

Property
--------
**max_iter**
Maximum iteration number in each run.
   type: int
   default: 10000

**dt**
Time step. dt_max = 10*dt.
   type: double
   default: 0.001

**ftol**
Force criteria used for convergence.
   type: double
   default: 0.1

**etol**
Energy criteria used for convergence.
   type: double
   default: 0.001

**Finc**
Fractional increase in timestep.
   type: double >1
   default: 1.1

**Fdec**
Fractional decrease in timestep.
   type: double between 0-1
   default: 0.5

**AlphaStart**
Initial alpha.
   type: double between 0-1
   default: 0.1

**Falpha**
Fractional decrease in alpha.
   type: double between 0-1
   default: 0.99
