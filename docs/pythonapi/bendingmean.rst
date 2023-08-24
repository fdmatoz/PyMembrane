.. _HelfrichElasticEnergy:

Helfrich Bending
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
     - Mesh>BendingHelfrich
   * - **Type**
     - Bending
   * - **Defined in**
     - vertex/edges/faces

The Helfrich bending energy for membranes is described by:

.. math::
   U = \int \kappa (H - H_0)^2 + \kappa_G K dA

Where:
- :math:`\kappa` is the bending rigidity.
- :math:`H` is the mean curvature of the membrane.
- :math:`H_0` is the spontaneous curvature.
- :math:`\kappa_G` is the Gaussian rigidity.
- :math:`K` is the Gaussian curvature.
- :math:`dA` denotes integration over the membrane surface area.

In the code we follow the Itzykson discretization of the curvature energy :cite:`kohyama2003budding, LiMatoz2021` we have the bending term

.. math::
    U =  [2\kappa_i (H_i-H_0 )^2+\kappa_{G,i} K_i] A_i

where :math:`\kappa (\kappa_G)` is the mean (Gaussian) rigidity of the component, :math:`A_i` is the dual-lattice area of vertex :math:`i`, and :math:`H_i (K_i)` is the mean (Gaussian) curvature at the vertex :math:`i`:

.. math::
    H_i = \frac{1}{2 A_i}\sum_{j\in N(i)}[\cot (\alpha_{ij})+\cot(\beta_{ij})](\mathbf{x}_i-\mathbf{x}_j) 

.. math::
    K_i = 2\pi-\sum_i \theta_i

Python calling
--------------

.. code-block:: python

   evolver.add_force("Mesh>Bending>Helfrich", {"kappaH": {"1": str(mean_curvature_rigidity)},
                                                "H0": {"1": str(spontaneous_curvature)},
                                                "kappaG": {"1": str(gaussian_curvature_rigidity)}})
