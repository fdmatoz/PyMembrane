.. _Bending:

Bending
=======

.. contents::
   :local:
   :depth: 2

Definition
----------

.. list-table::
   :widths: 25 75
   :header-rows: 0

   * - **Name**
     - Mesh>Bending
   * - **Type**
     - Bending
   * - **Defined in**
     - Edges

Use normals that belong to the two triangles that share an edge and calculate bending energy as :cite:`Seung88`:

.. math::

   U = \frac{1}{2} \kappa \vert \hat{\mathbf{n}}_k- \hat{\mathbf{n}}_l \vert^2

where :math:`( \kappa )` is the bending rigidity and :math:`( \hat{\mathbf{n}}_i, \hat{\mathbf{n}}_j )` are the normal unit vectors of the triangles meeting at the edge.

Python calling
--------------

.. code-block:: python

   evolver.add_force("Mesh>Bending", {"kappa": {"1": str(kappa_value)}})


