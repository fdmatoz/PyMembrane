.. _Limit:

Limit
=====

.. contents::
   :local:
   :depth: 2

Definition
----------
.. list-table::
   :widths: 25 75
   :header-rows: 0

   * - **Name**
     - Mesh>Limit
   * - **Type**
     - Limit
   * - **Defined in**
     - Edges

If the edge is longer or shorter than given values, the energy is infinity, otherwise it is zero :cite:`nelson2004statistical`:

.. math::
   E =
   \begin{cases}
   \infty & \text{if} \quad l < l_{min}\\
   \infty & \text{if} \quad l > l_{max}\\
   0 & \text{otherwise}
   \end{cases}
   
there :math:`l` is the current edge length. This is useful to avoid edges that are too long and lead to nonphysical shapes.

Python calling
--------------

.. code-block:: python

   evolver.add_force("Mesh>Limit", {
       "lmin": {"1": str(min_constant_value)},
       "lmax": {"1": str(max_bond_value)}
   })
