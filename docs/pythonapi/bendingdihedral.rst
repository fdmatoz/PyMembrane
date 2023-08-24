.. _DihedralBend:

Dihedral Bending
================

.. contents::
   :local:
   :depth: 2

Definition
----------
.. list-table::
   :widths: 25 75
   :header-rows: 0

   * - **Name**
     - Mesh>Dihedral
   * - **Type**
     - Bending Dihedral 
   * - **Defined in**
     - Edges

The bending energy using dihedral harmonics is described as:

.. math::
   U = \frac{1}{2} k_b (1 - \cos(\theta - \theta_0))

Where:
- :math:`k_b` is the bending rigidity.
- :math:`\theta` is the angle formed by the triangles meeting at the edge.
- :math:`\theta_0` is the preferred dihedral angle.

.. note::
   The dihedral potential is equivalent to :doc:`Bending Potential <bending>` but is faster to compute. Note this potential is the same as the one used in LAMMPS and HOODMD-Blue.

Python calling
--------------

.. code-block:: python

   evolver.add_force("Mesh>Bending>Dihedral", {"kappa": {"1": str(kappa_value)},
                                               "theta0": {"1": str(theta0_value)}})