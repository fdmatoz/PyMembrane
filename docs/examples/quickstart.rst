Quick Start and Examples
========================

.. image:: ../_static/logo-guide-examples.png
   :alt: logo-python-api
   :align: center

Packaged Example Overview
-------------------------

The examples run from the installed Python namespace with ``python -m``. The
input files are bundled with the package.

.. list-table::
   :header-rows: 1
   :widths: 18 28 28 12 24

   * - Example
     - Demonstrates
     - Command
     - Quick-mode runtime
     - Output files
   * - :doc:`periodic <examples/periodic>`
     - Periodic sheet wrinkling under uniaxial compression
     - ``python -m pymembrane.examples.periodic --quick``
     - few seconds
     - ``initial_mesh.vtk``, ``periodic_t*.vtk``; ``--quick`` reduces snapshots and MD steps
   * - :doc:`buckling <examples/shell_buckling>`
     - Caspar-Klug shell buckling with Monte Carlo vertex moves
     - ``python -m pymembrane.examples.buckling --quick``
     - few seconds
     - ``initial mesh.vtk``, ``sphere_t*.vtk``, ``final_mesh.vtk``; ``--quick`` reduces snapshots and MC steps
   * - :doc:`minimizer <examples/minimization>`
     - Constant-volume vesicle minimization with FIRE
     - ``python -m pymembrane.examples.minimizer --quick``
     - few seconds
     - ``initial mesh.vtk``, ``minimization_t*.vtk``; ``--quick`` reduces snapshots and minimizer iterations
   * - :doc:`disclination <examples/disclination>`
     - Brownian relaxation of an open ``+1`` disclination
     - ``python -m pymembrane.examples.disclination --quick``
     - few seconds
     - ``initial mesh.vtk``, ``pentagon_t*.vtk``; ``--quick`` reduces snapshots and BD steps
   * - :doc:`disclination_mc <examples/disclination>`
     - Monte Carlo relaxation of the same disclination
     - ``python -m pymembrane.examples.disclination_mc --quick``
     - few seconds
     - ``initial mesh.vtk``, ``pentagon_t*.vtk``, ``final_mesh.vtk``; ``--quick`` reduces snapshots and MC steps
   * - :doc:`disclination_verlet <examples/disclination>`
     - Velocity-Verlet relaxation of the same disclination
     - ``python -m pymembrane.examples.disclination_verlet --quick``
     - few seconds
     - ``initial mesh.vtk``, ``pentagon_t*.vtk``; ``--quick`` reduces snapshots and MD steps
   * - :doc:`hybrid_mc_bd <examples/hybrid_mc_bd>`
     - Alternating Brownian dynamics and Monte Carlo vertex moves
     - ``python -m pymembrane.examples.hybrid_mc_bd --quick``
     - few seconds
     - ``initial_mesh.vtk``, ``hybrid_t*.vtk``; ``--quick`` reduces cycles and MD/MC step counts
   * - :doc:`liquid_membrane <examples/liquid_membrane>`
     - Bending-dominated dynamic triangulation with edge flips and vertex moves
     - ``python -m pymembrane.examples.liquid_membrane --quick``
     - few seconds
     - ``initial.vtk``, ``final.vtk``, ``final.obj``, ``summary.json``; ``--quick`` reduces Monte Carlo steps
   * - :doc:`size_scaling <examples/size_scaling>`
     - Spherical size scaling with mesh generation, Monte Carlo vertex moves,
       and Brownian dynamics
     - ``python -m pymembrane.examples.size_scaling --quick``
     - few seconds
     - ``--json`` writes a timing summary; ``--keep-meshes`` preserves the generated ``.inp`` files

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
   size_scaling
   liquid_membrane
