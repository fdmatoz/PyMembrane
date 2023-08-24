Constant Face Area
==================

.. contents::
   :local:
   :depth: 2

Definition
----------
.. list-table::
   :widths: 25 75
   :header-rows: 0

   * - **Name**
     - Mesh>Constant Area
   * - **Type**
     - Stretching
   * - **Defined in**
     - Faces

For a given triangle \( t \), compute the contribution from the surface tension due to changes in the surface area.

.. math::
   U_t = \frac{1}{2} \sigma \left(A - A_0\right)^2

Where:

- \( A \) is the current area of the triangle
- \( A_0 \) is the native (given) triangle area
- \( \sigma \) is the surface tension

Python calling
--------------

.. code-block:: python

   evolver.add_force("Mesh>Constant Area", {
       "sigma": {"1": str(E_value)}
   })
