Hybrid Brownian + Monte Carlo Example
=====================================

This example demonstrates a conservative hybrid workflow built entirely from
existing public PyMembrane APIs. It uses the documented open ``+1``
disclination setup and alternates Brownian dynamics vertex relaxation with
Monte Carlo vertex moves.

What This Example Demonstrates
------------------------------

The packaged module reuses the same disclination mesh from
``InputFiles.zip``, the same box size, and the same elastic forces as the
existing Brownian and Monte Carlo disclination examples:

- harmonic stretching,
- limit protection,
- dihedral bending.

Within each cycle it performs:

1. Brownian dynamics with ``Mesh>Brownian>vertex>move``.
2. Monte Carlo sampling with ``Mesh>MonteCarlo>vertex>move``.
3. VTK output with ``system.dumper.vtk(...)``.

``--quick`` reduces only the number of cycles and the MD/MC step counts. It
does not change the model, mesh, force constants, or integrator names.

How to Run
----------

.. code-block:: bash

   python -m pymembrane.examples.hybrid_mc_bd --quick

To write files into a separate directory:

.. code-block:: bash

   python -m pymembrane.examples.hybrid_mc_bd --quick --output-dir results

Inputs
------

- packaged disclination mesh from ``InputFiles.zip``
- default mesh size parameter: ``N=14``
- output directory: current working directory unless ``--output-dir`` is used

Model Ingredients
-----------------

- mesh: open ``+1`` disclination reused from the documented disclination examples
- forces: ``Mesh>Harmonic``, ``Mesh>Limit``, ``Mesh>Bending>Dihedral``
- integrators: ``Mesh>Brownian>vertex>move`` and ``Mesh>MonteCarlo>vertex>move``
- boundary condition: non-periodic

Expected Output
---------------

- ``initial_mesh.vtk``
- ``hybrid_t0.vtk``
- additional ``hybrid_t*.vtk`` snapshots
- output format: legacy ASCII ``.vtk``

Quick Mode
----------

Quick mode keeps the same mesh, forces, and hybrid order, but reduces the
number of cycles and the MD/MC step counts.

How to visualize the result
---------------------------

Open the generated VTK files in ParaView to inspect how the same disclination
relaxes under alternating Brownian dynamics and Monte Carlo updates.

Source and packaged versions
----------------------------

The mirrored documentation source for this workflow is
``docs/examples/hybrid_mc_bd/__main__.py``. The installed runnable module is
``pymembrane.examples.hybrid_mc_bd``.
