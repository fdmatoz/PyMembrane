Size Scaling Example
====================

This example generates spherical meshes of increasing resolution and times
three workflows for each size:

- mesh generation and file writing
- Monte Carlo vertex moves
- Brownian dynamics

How to run
----------

.. code-block:: bash

   python -m pymembrane.examples.size_scaling --quick
   python -m pymembrane.examples.size_scaling --subdivisions 1 2 3 4 --steps 1000 --repeat 3 --json size_scaling.json

If you want to keep the generated mesh files, add ``--keep-meshes`` and
``--output-dir``:

.. code-block:: bash

   python -m pymembrane.examples.size_scaling --keep-meshes --output-dir size_scaling_meshes

Model ingredients
-----------------

- mesh: recursively subdivided icosphere
- Monte Carlo force model: harmonic stretching, length limits, dihedral bending
- Monte Carlo integrator: ``Mesh>MonteCarlo>vertex>move``
- Brownian dynamics integrator: ``Mesh>Brownian>vertex>move``

Expected output
---------------

The example prints a compact table with one row each for ``mesh_generation``,
``mc_vertex_move``, and ``brownian_dynamics``. If ``--json`` is supplied, the
results are written to a JSON file.

Quick mode
----------

``--quick`` reduces the default subdivision list to ``0 1 2`` and runs a short
smoke-test length of 100 steps for the Monte Carlo and Brownian dynamics
workflows with ``--repeat 3`` and ``--warmup 1``. For more stable timings,
increase ``--steps`` and keep ``--repeat 3`` or larger.
