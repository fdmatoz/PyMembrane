PyMembrane Examples
===================

Here we demonstrate various applications of PyMembrane to study common
problems in the physics of membranes. The examples below are kept in two
forms:

- The source versions live under ``docs/examples`` and are useful for
  developers who want to inspect the documented scripts directly.
- The installed versions live under ``pymembrane.examples`` and can be run
  with ``python -m`` after installation. Their runtime input files are
  bundled inside the installed package, so they do not depend on
  ``docs/examples`` at runtime.

Primary interface
-----------------

Install PyMembrane from the repository root:

.. code-block:: bash

   pip install -e .

Then run the packaged examples from any working directory:

.. code-block:: bash

   python -m pymembrane.examples.periodic --quick
   python -m pymembrane.examples.buckling --quick
   python -m pymembrane.examples.minimizer --quick
   python -m pymembrane.examples.disclination --quick
   python -m pymembrane.examples.disclination_mc --quick
   python -m pymembrane.examples.disclination_verlet --quick
   python -m pymembrane.examples.size_scaling --quick

Most examples also support ``--output-dir`` so that generated files are kept in
one directory:

.. code-block:: bash

   python -m pymembrane.examples.periodic --quick --output-dir results

Packaged examples
-----------------

``pymembrane.examples.periodic``
   Demonstrates wrinkling of a periodic elastic sheet under uniaxial
   compression. Quick mode writes ``initial_mesh.vtk`` and ``periodic_t*.vtk``.
   Typical quick-mode runtime is a few seconds on a laptop.

``pymembrane.examples.buckling``
   Demonstrates Caspar-Klug shell buckling of a closed triangulated shell using
   a Monte Carlo vertex move. Quick mode writes ``initial mesh.vtk``,
   ``sphere_t*.vtk`` and ``final_mesh.vtk``. Typical quick-mode runtime is a
   few seconds.

``pymembrane.examples.minimizer``
   Demonstrates constant-volume energy minimization of a strained vesicle using
   the FIRE minimizer. Quick mode writes ``initial mesh.vtk`` and
   ``minimization_t*.vtk``. Typical quick-mode runtime is a few seconds.

``pymembrane.examples.disclination``
   Demonstrates Brownian relaxation of an open ``+1`` disclination. Quick mode
   writes ``initial mesh.vtk`` and ``pentagon_t*.vtk``. Typical quick-mode
   runtime is a few seconds.

``pymembrane.examples.disclination_mc``
   Demonstrates the same disclination problem using a Monte Carlo vertex-move
   integrator. Quick mode writes ``initial mesh.vtk``, ``pentagon_t*.vtk`` and
   ``final_mesh.vtk``. Typical quick-mode runtime is a few seconds.

``pymembrane.examples.disclination_verlet``
   Demonstrates the same disclination problem using a velocity-Verlet
   integrator. Quick mode writes ``initial mesh.vtk`` and ``pentagon_t*.vtk``.
   Typical quick-mode runtime is a few seconds.

``pymembrane.examples.hybrid_mc_bd``
   Demonstrates a conservative hybrid workflow for the open ``+1``
   disclination problem by alternating Brownian dynamics vertex relaxation with
   Monte Carlo vertex moves. Quick mode writes ``initial_mesh.vtk`` and
   ``hybrid_t*.vtk``. Typical quick-mode runtime is a few seconds.

``pymembrane.examples.size_scaling``
   Generates spherical meshes of increasing resolution and times three
   workflows for each size: mesh generation, Monte Carlo vertex moves, and
   Brownian dynamics. Quick mode runs subdivisions ``0 1 2`` with short Monte
   Carlo and Brownian workflows.

Dependency note
---------------

- The examples require PyMembrane and its runtime dependencies.
- No plotting packages are required to run the examples.
- Output geometry is written with dumper methods such as
  ``system.dumper.vtk(...)``.

.. toctree::
   :maxdepth: 1
   
   disclination
   hybrid_mc_bd
   shell_buckling
   minimization
   size_scaling
   periodic
