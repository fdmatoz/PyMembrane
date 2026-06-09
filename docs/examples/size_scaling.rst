Size Scaling Example
====================

This example generates spherical meshes of increasing resolution and reports
the cost of a small set of core PyMembrane operations on each mesh.

What this example demonstrates
------------------------------

- sphere generation from a recursive icosphere construction
- energy evaluation only
- Monte Carlo vertex-displacement attempts
- Monte Carlo edge-flip attempts
- Brownian-dynamics vertex updates
- optional output timing, reported separately when ``--dump-output`` is used

The benchmark also records environment information such as the CPU model, the
operating system, the Python version, the PyMembrane version or git commit,
and the number of threads when that information is available.

How to run
----------

.. code-block:: bash

   python -m pymembrane.examples.size_scaling --quick
   python -m pymembrane.examples.size_scaling --vertices 1000 5000 10000 50000 --steps 10000 --repeat 3 --json size_scaling.json

If you want to keep the generated temporary mesh files on disk, add
``--keep-meshes`` and ``--output-dir``:

.. code-block:: bash

   python -m pymembrane.examples.size_scaling --keep-meshes --output-dir size_scaling_meshes

Command-line arguments
----------------------

.. list-table::
   :header-rows: 1
   :widths: 18 16 66

   * - Argument
     - Default
     - Description
   * - ``--quick``
     - ``False``
     - Run a short version suitable for checking the installation and the
       benchmark workflow.
   * - ``--subdivisions``
     - ``None``
     - Icosphere subdivision levels used to generate the benchmark meshes.
       Runtime default: ``[0, 1, 2]`` with ``--quick`` or ``[0, 1, 2, 3]``
       otherwise. This option is mutually exclusive with ``--vertices``.
   * - ``--vertices``
     - ``None``
     - Target vertex counts. Each requested size is mapped to the nearest
       available icosphere subdivision level. This option is mutually
       exclusive with ``--subdivisions``.
   * - ``--steps``
     - ``None``
     - Number of evolution steps or repeated energy evaluations used in each
       benchmark row. Runtime default: ``100`` with ``--quick`` or ``1000``
       otherwise.
   * - ``--repeat``
     - ``None``
     - Number of timed repeats collected for each benchmark row. Runtime
       default: ``3``.
   * - ``--warmup``
     - ``None``
     - Number of untimed warmup runs collected before timing each benchmark
       row. Runtime default: ``1``.
   * - ``--json``
     - ``None``
     - Write the timing summary to a JSON file.
   * - ``--csv``
     - ``None``
     - Write the timing summary to a CSV file.
   * - ``--dump-output``
     - ``False``
     - Time a representative VTK output write in addition to the compute
       benchmarks.
   * - ``--keep-meshes``
     - ``False``
     - Keep the generated temporary mesh files on disk.
   * - ``--output-dir``
     - ``size_scaling_meshes``
     - Directory used when keeping generated mesh files.

Expected output
---------------

The example prints a compact table with rows for
``energy_eval_only``, ``mc_vertex_move``, ``mc_edge_flip``,
``brownian_dynamics``, and, when requested, ``dump_vtk``. If ``--json`` or
``--csv`` is supplied, the results are written to the chosen file.

Quick mode
----------

``--quick`` reduces the default subdivision list to ``0 1 2`` and uses a short
run of ``100`` steps, ``--repeat 3``, and ``--warmup 1``. For more stable
timings, increase ``--steps`` and keep ``--repeat 3`` or larger.

