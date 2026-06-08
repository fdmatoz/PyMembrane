Quick Start and Examples
========================

.. image:: ../_static/logo-guide-examples.png
   :alt: logo-python-api
   :align: center

Packaged Example Overview
-------------------------

All reviewer-facing examples run from the installed Python namespace with
``python -m``. The input files are bundled with the package, so no manual
downloads or path edits are required.

.. list-table::
   :header-rows: 1
   :widths: 18 28 28 12 24

   * - Example
     - Demonstrates
     - Command
     - Quick-mode runtime
     - Output files
   * - ``periodic``
     - Periodic sheet wrinkling under uniaxial compression
     - ``python -m pymembrane.examples.periodic --quick``
     - few seconds
     - ``initial_mesh.vtk``, ``periodic_t*.vtk``; ``--quick`` reduces snapshots and MD steps
   * - ``buckling``
     - Caspar-Klug shell buckling with Monte Carlo vertex moves
     - ``python -m pymembrane.examples.buckling --quick``
     - few seconds
     - ``initial mesh.vtk``, ``sphere_t*.vtk``, ``final_mesh.vtk``; ``--quick`` reduces snapshots and MC steps
   * - ``minimizer``
     - Constant-volume vesicle minimization with FIRE
     - ``python -m pymembrane.examples.minimizer --quick``
     - few seconds
     - ``initial mesh.vtk``, ``minimization_t*.vtk``; ``--quick`` reduces snapshots and minimizer iterations
   * - ``disclination``
     - Brownian relaxation of an open ``+1`` disclination
     - ``python -m pymembrane.examples.disclination --quick``
     - few seconds
     - ``initial mesh.vtk``, ``pentagon_t*.vtk``; ``--quick`` reduces snapshots and BD steps
   * - ``disclination_mc``
     - Monte Carlo relaxation of the same disclination
     - ``python -m pymembrane.examples.disclination_mc --quick``
     - few seconds
     - ``initial mesh.vtk``, ``pentagon_t*.vtk``, ``final_mesh.vtk``; ``--quick`` reduces snapshots and MC steps
   * - ``disclination_verlet``
     - Velocity-Verlet relaxation of the same disclination
     - ``python -m pymembrane.examples.disclination_verlet --quick``
     - few seconds
     - ``initial mesh.vtk``, ``pentagon_t*.vtk``; ``--quick`` reduces snapshots and MD steps
   * - ``hybrid_mc_bd``
     - Alternating Brownian dynamics and Monte Carlo vertex moves
     - ``python -m pymembrane.examples.hybrid_mc_bd --quick``
     - few seconds
     - ``initial_mesh.vtk``, ``hybrid_t*.vtk``; ``--quick`` reduces cycles and MD/MC step counts

All of the examples above write legacy ASCII ``.vtk`` output through the
default dumper. None requires plotting packages or the VTK Python bindings to
run.

Reproducibility Checklist
-------------------------

- [ ] package imports
- [ ] example runs from outside repository
- [ ] input files are bundled
- [ ] output files are written
- [ ] no import-path changes needed
- [ ] no plotting packages needed

.. toctree::
   :maxdepth: 1
   
   tutorial
   examples
