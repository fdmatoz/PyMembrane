.. _Substrate:

Substrate
=========

.. contents::
   :local:
   :depth: 2

Definition
----------
.. list-table::
   :widths: 25 75
   :header-rows: 0

   * - **Name**
     - Mesh>Substrate
   * - **Type**
     - Substrate
   * - **Defined in**
     - Vertices

This potential is defined for vertices in relation to their z-position. If the z-coordinate of a vertex is positive, a force is applied downwards; if it is negative, the force is applied upwards. The magnitude of the force is proportional to the z-coordinate and a type-specific constant, either :math:`k_{z1}` or :math:`k_{z2}`.

.. math::
   f_{z} =
   \begin{cases}
   -k_{z1} \cdot z & \text{if} \quad z \geq 0 \\
   -k_{z2} \cdot z & \text{if} \quad z < 0
   \end{cases}

Python calling
--------------

.. code-block:: python

   evolver.add_force("Mesh>Substrate", {
       "kz1": {"1": str(kz1_value)},
       "kz2": {"1": str(kz2_value)}
   })
