Benchmarks
==========

Size-Scaling Example
--------------------

The sphere size-scaling workflow is now available as an installed example:

.. code-block:: bash

   pip install -e .
   python -m pymembrane.examples.size_scaling --quick
   python -m pymembrane.examples.size_scaling --subdivisions 1 2 3 4 --steps 1000 --repeat 3 --json size_scaling.json

This example generates spherical meshes of increasing resolution and reports
three timings for each size:

- ``mesh_generation``
- ``mc_vertex_move``
- ``brownian_dynamics``

The legacy command ``python -m pymembrane.benchmarks.sphere`` remains as a
compatibility wrapper and prints a message pointing to the installed example.

The ``--quick`` option is intended as a quick check. For more stable timings,
increase ``--steps`` and keep ``--repeat 3`` or larger.

When quoting timings, include the CPU model, operating system, Python version,
compiler/build mode if known, and the exact command used.
