Command-Line Arguments
======================

The packaged examples provide command-line options for controlling runtime,
output location, and the amount of work done in each example. The arguments
documented below are taken directly from the installed example parsers.

Periodic
--------

Command
~~~~~~~

.. code-block:: bash

   python -m pymembrane.examples.periodic --quick

Arguments
~~~~~~~~~

.. list-table::
   :header-rows: 1
   :widths: 18 16 66

   * - Argument
     - Default
     - Description
   * - ``--quick``
     - ``False``
     - Run a short version of the example for testing the installation.
   * - ``--snapshots``
     - ``None``
     - Number of output snapshots. Runtime default: ``3`` with ``--quick`` or
       ``100`` otherwise.
   * - ``--run_steps``
     - ``None``
     - Number of Brownian-dynamics steps between output snapshots. Runtime
       default: ``10`` with ``--quick`` or ``5000`` otherwise.
   * - ``--epsilon``
     - ``0.01``
     - Fractional compression applied to the x box length after each snapshot.
   * - ``--output-dir``
     - ``.``
     - Directory where output mesh files are written.

Recommended commands
~~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

   python -m pymembrane.examples.periodic --quick
   python -m pymembrane.examples.periodic --snapshots 5 --run_steps 1000 --output-dir periodic_output

Expected output
~~~~~~~~~~~~~~~

``initial_mesh.vtk`` and ``periodic_t*.vtk`` are written to the chosen output
directory.

Quick mode
~~~~~~~~~~

``--quick`` reduces the run to a short quick check with fewer snapshots and
fewer Brownian-dynamics steps.

Buckling
--------

Command
~~~~~~~

.. code-block:: bash

   python -m pymembrane.examples.buckling --quick

Arguments
~~~~~~~~~

.. list-table::
   :header-rows: 1
   :widths: 18 16 66

   * - Argument
     - Default
     - Description
   * - ``--quick``
     - ``False``
     - Run a short version of the example for testing the installation.
   * - ``--snapshots``
     - ``None``
     - Number of output snapshots. Runtime default: ``3`` with ``--quick`` or
       ``100`` otherwise.
   * - ``--run_steps``
     - ``None``
     - Number of Monte Carlo steps between output snapshots. Runtime default:
       ``10`` with ``--quick`` or ``5000`` otherwise.
   * - ``--output-dir``
     - ``.``
     - Directory where output mesh files are written.

Recommended commands
~~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

   python -m pymembrane.examples.buckling --quick
   python -m pymembrane.examples.buckling --snapshots 5 --run_steps 1000 --output-dir buckling_output

Expected output
~~~~~~~~~~~~~~~

``initial mesh.vtk``, ``sphere_t*.vtk``, and ``final_mesh.vtk`` are written to
the chosen output directory.

Quick mode
~~~~~~~~~~

``--quick`` reduces the run to a short quick check with fewer snapshots and
fewer Monte Carlo steps.

Minimizer
---------

Command
~~~~~~~

.. code-block:: bash

   python -m pymembrane.examples.minimizer --quick

Arguments
~~~~~~~~~

.. list-table::
   :header-rows: 1
   :widths: 18 16 66

   * - Argument
     - Default
     - Description
   * - ``--quick``
     - ``False``
     - Run a short version of the example for testing the installation.
   * - ``--snapshots``
     - ``None``
     - Number of output snapshots. Runtime default: ``2`` with ``--quick`` or
       ``50`` otherwise.
   * - ``--max_iter``
     - ``None``
     - Maximum number of FIRE iterations used for each minimization call.
       Runtime default: ``25`` with ``--quick`` or ``2000`` otherwise.
   * - ``--output-dir``
     - ``.``
     - Directory where output mesh files are written.

Recommended commands
~~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

   python -m pymembrane.examples.minimizer --quick
   python -m pymembrane.examples.minimizer --snapshots 5 --max_iter 2000 --output-dir minimizer_output

Expected output
~~~~~~~~~~~~~~~

``initial mesh.vtk`` and ``minimization_t*.vtk`` are written to the chosen
output directory.

Quick mode
~~~~~~~~~~

``--quick`` reduces the run to a short quick check with fewer snapshots and
fewer minimizer iterations.

Disclination
------------

Command
~~~~~~~

.. code-block:: bash

   python -m pymembrane.examples.disclination --quick

Arguments
~~~~~~~~~

.. list-table::
   :header-rows: 1
   :widths: 18 16 66

   * - Argument
     - Default
     - Description
   * - ``--quick``
     - ``False``
     - Run a short version of the example for testing the installation.
   * - ``--snapshots``
     - ``None``
     - Number of output snapshots. Runtime default: ``3`` with ``--quick`` or
       ``4`` otherwise.
   * - ``--run_steps``
     - ``None``
     - Number of Brownian-dynamics steps between output snapshots. Runtime
       default: ``10`` with ``--quick`` or ``25`` otherwise.
   * - ``--N``
     - ``14``
     - Pentagon mesh size used to select the bundled input mesh.
   * - ``--output-dir``
     - ``.``
     - Directory where output mesh files are written.

Recommended commands
~~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

   python -m pymembrane.examples.disclination --quick
   python -m pymembrane.examples.disclination --N 18 --snapshots 4 --run_steps 25 --output-dir disclination_output

Expected output
~~~~~~~~~~~~~~~

``initial mesh.vtk`` and ``pentagon_t*.vtk`` are written to the chosen output
directory.

Quick mode
~~~~~~~~~~

``--quick`` reduces the run to a short quick check with fewer snapshots and
fewer Brownian-dynamics steps.

Monte Carlo disclination
------------------------

Command
~~~~~~~

.. code-block:: bash

   python -m pymembrane.examples.disclination_mc --quick

Arguments
~~~~~~~~~

.. list-table::
   :header-rows: 1
   :widths: 18 16 66

   * - Argument
     - Default
     - Description
   * - ``--quick``
     - ``False``
     - Run a short version of the example for testing the installation.
   * - ``--snapshots``
     - ``None``
     - Number of output snapshots. Runtime default: ``3`` with ``--quick`` or
       ``4`` otherwise.
   * - ``--run_steps``
     - ``None``
     - Number of Monte Carlo steps between output snapshots. Runtime default:
       ``10`` with ``--quick`` or ``25`` otherwise.
   * - ``--N``
     - ``14``
     - Pentagon mesh size used to select the bundled input mesh.
   * - ``--output-dir``
     - ``.``
     - Directory where output mesh files are written.

Recommended commands
~~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

   python -m pymembrane.examples.disclination_mc --quick
   python -m pymembrane.examples.disclination_mc --N 18 --snapshots 4 --run_steps 25 --output-dir disclination_mc_output

Expected output
~~~~~~~~~~~~~~~

``initial mesh.vtk``, ``pentagon_t*.vtk``, and ``final_mesh.vtk`` are written
to the chosen output directory.

Quick mode
~~~~~~~~~~

``--quick`` reduces the run to a short quick check with fewer snapshots and
fewer Monte Carlo steps.

Velocity-Verlet disclination
----------------------------

Command
~~~~~~~

.. code-block:: bash

   python -m pymembrane.examples.disclination_verlet --quick

Arguments
~~~~~~~~~

.. list-table::
   :header-rows: 1
   :widths: 18 16 66

   * - Argument
     - Default
     - Description
   * - ``--quick``
     - ``False``
     - Run a short version of the example for testing the installation.
   * - ``--snapshots``
     - ``None``
     - Number of output snapshots. Runtime default: ``3`` with ``--quick`` or
       ``4`` otherwise.
   * - ``--run_steps``
     - ``None``
     - Number of velocity-Verlet steps between output snapshots. Runtime
       default: ``10`` with ``--quick`` or ``25`` otherwise.
   * - ``--N``
     - ``14``
     - Pentagon mesh size used to select the bundled input mesh.
   * - ``--output-dir``
     - ``.``
     - Directory where output mesh files are written.

Recommended commands
~~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

   python -m pymembrane.examples.disclination_verlet --quick
   python -m pymembrane.examples.disclination_verlet --N 18 --snapshots 4 --run_steps 25 --output-dir disclination_verlet_output

Expected output
~~~~~~~~~~~~~~~

``initial mesh.vtk`` and ``pentagon_t*.vtk`` are written to the chosen output
directory.

Quick mode
~~~~~~~~~~

``--quick`` reduces the run to a short quick check with fewer snapshots and
fewer velocity-Verlet steps.

Hybrid MC + BD
--------------

Command
~~~~~~~

.. code-block:: bash

   python -m pymembrane.examples.hybrid_mc_bd --quick

Arguments
~~~~~~~~~

.. list-table::
   :header-rows: 1
   :widths: 18 16 66

   * - Argument
     - Default
     - Description
   * - ``--quick``
     - ``False``
     - Run a short version of the example for testing the installation.
   * - ``--cycles``
     - ``None``
     - Number of alternating Brownian-dynamics and Monte Carlo cycles.
       Runtime default: ``3`` with ``--quick`` or ``8`` otherwise.
   * - ``--md-steps``
     - ``None``
     - Brownian-dynamics steps per hybrid cycle. Runtime default: ``10`` with
       ``--quick`` or ``25`` otherwise.
   * - ``--mc-steps``
     - ``None``
     - Monte Carlo steps per temperature value in each hybrid cycle. Runtime
       default: ``10`` with ``--quick`` or ``25`` otherwise.
   * - ``--N``
     - ``14``
     - Pentagon mesh size used to select the bundled input mesh.
   * - ``--output-dir``
     - ``.``
     - Directory where output mesh files are written.

Recommended commands
~~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

   python -m pymembrane.examples.hybrid_mc_bd --quick
   python -m pymembrane.examples.hybrid_mc_bd --cycles 5 --md-steps 25 --mc-steps 25 --output-dir hybrid_output

Expected output
~~~~~~~~~~~~~~~

``initial_mesh.vtk`` and ``hybrid_t*.vtk`` are written to the chosen output
directory.

Quick mode
~~~~~~~~~~

``--quick`` reduces the run to a short quick check with fewer cycles and fewer
Brownian-dynamics and Monte Carlo steps.

Size Scaling
------------

Command
~~~~~~~

.. code-block:: bash

   python -m pymembrane.examples.size_scaling --quick

Arguments
~~~~~~~~~

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
     - Icosphere subdivision levels used to generate the spherical meshes.
       Runtime default: ``[0, 1, 2]`` with ``--quick`` or ``[0, 1, 2, 3]``
       otherwise. This option is mutually exclusive with ``--vertices``.
   * - ``--vertices``
     - ``None``
     - Target vertex counts. Each requested size is mapped to the nearest
       available icosphere subdivision level. This option is mutually
       exclusive with ``--subdivisions``.
   * - ``--steps``
     - ``None``
     - Number of evolution steps used for the Monte Carlo and Brownian
       workflows. Runtime default: ``100`` with ``--quick`` or ``1000``
       otherwise.
   * - ``--repeat``
     - ``None``
     - Number of timed repeats collected for each workflow. Runtime default:
       ``3``.
   * - ``--warmup``
     - ``None``
     - Number of untimed warmup runs collected before timing each workflow.
       Runtime default: ``1``.
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

Recommended commands
~~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

   python -m pymembrane.examples.size_scaling --quick
   python -m pymembrane.examples.size_scaling --vertices 1000 5000 10000 50000 --steps 10000 --repeat 3 --json size_scaling.json

Expected output
~~~~~~~~~~~~~~~

The example prints a table with ``energy_eval_only``, ``mc_vertex_move``,
``mc_edge_flip``, and ``brownian_dynamics`` rows. If ``--dump-output`` is
used, a ``dump_vtk`` row is added. If ``--json`` or ``--csv`` is supplied, the
timings are written to the chosen file.

Quick mode
~~~~~~~~~~

``--quick`` reduces the default subdivision list to ``0 1 2`` and runs a short
version of the benchmark with ``100`` steps, ``--repeat 3``, and
``--warmup 1``. For more stable timings, increase ``--steps`` and keep
``--repeat 3`` or larger.

Liquid Membrane Example
~~~~~~~~~~~~~~~~~~~~~~~

Command
^^^^^^^

.. code-block:: bash

   python -m pymembrane.examples.liquid_membrane --quick

Arguments
^^^^^^^^^

.. list-table::
   :header-rows: 1
   :widths: 18 16 66

   * - Argument
     - Default
     - Description
   * - ``--quick``
     - ``False``
     - Run a short version of the example for testing the installation.
   * - ``--steps``
     - ``None``
     - Number of Monte Carlo steps used in the main simulation run. Runtime
       default: ``100`` with ``--quick`` or ``1000`` otherwise.
   * - ``--subdivision``
     - ``None``
     - Icosphere subdivision level used to build the initial spherical mesh.
       Runtime default: ``1`` with ``--quick`` or ``2`` otherwise.
   * - ``--temperature``
     - ``1e-1``
     - Monte Carlo temperature used for the liquid-membrane workflow.
   * - ``--output-dir``
     - ``.``
     - Directory where the VTK, OBJ, and summary files are written.
   * - ``--json``
     - ``summary.json``
     - Path to the summary JSON file.
   * - ``--seed``
     - ``202208``
     - Base random seed used for the Monte Carlo integrators.
   * - ``--keep-meshes``
     - ``False``
     - Keep the generated temporary mesh input files on disk.

Recommended commands
^^^^^^^^^^^^^^^^^^^^

.. code-block:: bash

   python -m pymembrane.examples.liquid_membrane --quick
   python -m pymembrane.examples.liquid_membrane --steps 1000 --output-dir liquid_output

Expected output
^^^^^^^^^^^^^^^

``initial.vtk``, ``final.vtk``, ``final.obj`` and ``summary.json`` are
written to the chosen output directory. If generated mesh inputs are retained,
the ``mesh_inputs/*.inp`` files are also available.

Quick mode
^^^^^^^^^^

``--quick`` reduces the run to a short quick check with fewer Monte Carlo
steps, while keeping the same mesh generation, force model, and integrator
names.
