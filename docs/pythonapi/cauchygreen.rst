.. _CauchyGreen:

Cauchy-Green
============

.. contents::
   :local:
   :depth: 2

Definition
----------
.. list-table::
   :widths: 25 75
   :header-rows: 0

   * - **Name**
     - Mesh>Cauchy-Green
   * - **Type**
     - Stretching
   * - **Defined in**
     - Faces

Stretching energy density is calculated as :cite:`brakke1992surface, sknepnek2012nonlinear, Matoz2020`:

.. math::

   U = \frac{E\,h}{2}\frac{1}{1+\nu}\left[Tr\left(C^2\right) + \frac{\nu}{1-\nu}\left(Tr C\right)^2\right]

where :math:`E` is the 3D Young modulus constant, :math:`h` is the membrane thickness, :math:`\nu` is the Poisson ratio, and 

.. math::

   C = \frac{1}{2}\left(FQ-I\right)

with :math:`F` being the Gram matrix for the strained triangle, :math:`Q` being the inverse of the Gram matrix for the unstretched triangle, and :math:`I` is the identity matrix.

Python calling
--------------

.. code-block:: python

   evolver.add_force("Mesh>Cauchy-Green", {
       "E": {"1": str(E_value)},  # Elastic constant
       "h": {"1": str(h_value)},  # Thickness 
       "nu": {"1": str(nu_value)} # Poisson ratio
   })

