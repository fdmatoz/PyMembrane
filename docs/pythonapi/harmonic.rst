.. _Harmonic:

Harmonic
========

.. contents::
   :local:
   :depth: 2

Definition
----------
.. list-table::
   :widths: 25 75
   :header-rows: 0

   * - **Name**
     - Mesh>Harmonic
   * - **Type**
     - Harmonic
   * - **Defined in**
     - Edges

Calculate stretching energy by assuming that edges are Hookean harmonic springs :cite:`Seung88`:

.. math::

   U = \frac{1}{2} k \left( l - l_0 \right)^2

where :math:`k` is the spring constant, :math:`l_0` is the length of the unstretched (reference) edge, and :math:`l` is the current edge length.

Python calling
--------------

.. code-block:: python

   evolver.add_force("Mesh>Harmonic", {
       "k": {"1": str(spring_constant_value)}, # spring constant
       "l0": {"1": str(unstretched_bond_value)} # unstretched bond length
   })
