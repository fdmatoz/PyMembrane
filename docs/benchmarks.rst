Performance benchmark and scope comparison
==========================================

.. warning::

   This benchmark is not intended as a ranking of membrane-simulation
   packages.

   Different tools implement different discretizations, update schemes,
   neighbor data structures, parallelization strategies, compiler settings, and
   I/O conventions. Direct wall-clock comparison between packages is meaningful
   only when the physical model, mesh, update schedule, acceptance criterion,
   hardware, compiler flags, and thread count are matched.

   The purpose of this benchmark is to document PyMembrane’s internal scaling
   and to provide a reproducible protocol that users can adapt when comparing
   PyMembrane with other tools.

Internal PyMembrane scaling benchmark
-------------------------------------

The installed example
``python -m pymembrane.examples.size_scaling`` generates spherical meshes of
increasing size and reports the cost of core operations on each mesh.

The benchmark supports both subdivision-based and target-size-based runs:

.. code-block:: bash

   pip install -e .
   python -m pymembrane.examples.size_scaling --quick
   python -m pymembrane.examples.size_scaling --vertices 1000 5000 10000 50000 --steps 10000 --repeat 3 --json size_scaling.json

The reported rows are:

- ``energy_eval_only``: repeated ``system.compute.energy(evolver)`` calls
- ``mc_vertex_move``: Monte Carlo vertex displacement attempts
- ``mc_edge_flip``: Monte Carlo edge-flip attempts
- ``brownian_dynamics``: Brownian-dynamics vertex updates
- ``dump_vtk``: optional output timing, separated from compute timing when
  ``--dump-output`` is used

For each row, the example reports:

- number of vertices, edges, and faces
- number of attempted operations
- wall time
- rate in attempted operations per second
- seconds per ``10^6`` attempted operations
- acceptance ratio, when available from the API
- peak memory, when available
- CPU model
- operating system
- Python version
- compiler and compiler flags, when available
- number of threads
- PyMembrane version or git commit

For the Monte Carlo rows, the ``seconds per 10^6 attempted operations``
normalization is read as seconds per ``10^6`` attempted vertex moves or edge
flips, depending on the row.

When quoting timings, include the exact command used and the relevant
environment metadata. These benchmarks are intended to document PyMembrane’s
serial scaling and practical cost of core operations, particularly
dynamic-connectivity liquid-membrane moves, rather than to establish a
universal ranking among membrane-simulation tools.

The legacy command ``python -m pymembrane.benchmarks.sphere`` remains as a
compatibility wrapper and points to the installed example.

Feature and scope comparison
----------------------------

The table below summarizes the public scope of several membrane-simulation
tools as described in their documentation or primary publications. The
comparison is intentionally qualitative and focuses on use case, model scope,
and workflow style rather than on speed ranking.

.. list-table::
   :header-rows: 1
   :widths: 14 18 16 14 14 14 14 14 12 18

   * - Package
     - Primary purpose
     - Membrane representation
     - Liquid membrane / dynamic triangulation
     - Elastic membrane support
     - Monte Carlo support
     - Brownian or molecular dynamics support
     - Energy minimization support
     - Python / parallel support
     - Best comparison role
   * - PyMembrane
     - Flexible C++/Python framework for membrane simulations
     - Triangulated surfaces
     - Yes
     - Yes
     - Yes
     - Yes
     - Yes
     - Python API; serial C++ core
     - Flexible framework and internal scaling reference
   * - TriMem
     - Specialized hybrid Monte Carlo code for lipid membranes
     - Triangulated lipid membranes
     - Yes
     - Yes
     - Yes
     - Limited / not the main focus
     - Yes
     - Python-wrapped C++ with OpenMP
     - Closest performance comparator for fluid membranes with edge flips
   * - flippy
     - User-friendly lipid membrane simulation framework
     - Dynamically triangulated membranes
     - Yes
     - Yes, including mixed fluid / elastic regions
     - Yes
     - Not the main focus
     - Yes
     - Python package
     - Related fluid-membrane workflow with mixed fluid/elastic regions
   * - FreeDTS
     - Mesoscale biomembrane simulation
     - Dynamically triangulated surfaces with inclusions
     - Yes
     - Yes
     - Yes
     - Not the main focus
     - Yes
     - Project-specific interface
     - Biomembrane reference with vertex-based inclusions
   * - Surface Evolver
     - Surface-energy minimization with constraints
     - Triangulated / simplex surfaces
     - Not a native fluid-dynamics code
     - Yes
     - No native Monte Carlo membrane workflow
     - No native Brownian / molecular dynamics workflow
     - Yes
     - Command-line / scriptable interface
     - Historical minimization and constrained-surface reference
   * - Morpho
     - Programmable shape optimization and soft-matter modeling
     - Mesh-based optimization problems
     - Not a native dynamically triangulated membrane code
     - Yes
     - Not a native Monte Carlo membrane workflow
     - Not a native Brownian / molecular dynamics workflow
     - Yes
     - Programmable environment
     - Optimization and scripting reference
   * - LAMMPS
     - General particle and coarse-grained molecular dynamics
     - Particles and coarse-grained models
     - Not a native triangulated membrane code
     - Possible through model setup
     - Yes, through Monte Carlo-related functionality and packages
     - Yes
     - Yes
     - Python library with MPI parallelism
     - Particle / coarse-grained reference, not a direct triangulated-membrane comparator
   * - HOOMD-blue
     - Particle molecular dynamics and hard-particle Monte Carlo
     - Particles and coarse-grained models
     - Not a native triangulated membrane code
     - Possible through model setup
     - Yes
     - Yes
     - Yes
     - Python API; CPU/GPU support
     - Particle / coarse-grained reference, not a direct triangulated-membrane comparator

Scope note
----------

For large-scale simulations where the primary objective is maximum performance
for fluid membranes, specialized dynamically triangulated membrane packages may
be preferable. PyMembrane’s focus is on providing a modular and extensible
C++/Python framework in which forces, integrators, minimizers, constraints, and
output routines can be combined and extended for new membrane models.
