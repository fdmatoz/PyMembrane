Quick Start
===========

Reviewer Smoke Test
-------------------

If you only want to verify that the documented workflow works end to end, run:

.. code-block:: bash

   pip install -e .
   python -c "import pymembrane; print(pymembrane.__file__)"
   python -c "from pymembrane import *; print('import ok')"
   python -m pymembrane.examples.periodic --quick
   python -m pymembrane.examples.hybrid_mc_bd --quick

Minimal workflow
----------------

The standard PyMembrane workflow is:

.. code-block:: python

   from math import sqrt
   from pymembrane import Box, System, Evolver

   box = Box(sqrt(3.0) * 29, 50.0, 50.0, True, True, True)
   system = System(box)
   system.read_mesh_from_files(files={"vertices": "vertices.dat", "faces": "faces.dat"})
   system.enforce_boundaries()

   evolver = Evolver(system)
   evolver.add_force("Mesh>Harmonic", {"k": {"0": "100.0"}, "l0": {"0": "1.0"}})
   evolver.add_force("Mesh>Limit", {"lmin": {"0": "0.7"}, "lmax": {"0": "1.3"}})
   evolver.add_force("Mesh>Bending>Dihedral", {"kappa": {"0": "1.0"}})
   evolver.add_integrator("Mesh>Brownian>vertex>move", {"seed": "202208"})
   evolver.set_time_step("2e-3")
   evolver.set_global_temperature("1e-4")
   evolver.evolveMD(steps=10)
   system.dumper.vtk("output", periodic=True)

This short example follows the same setup used in the packaged periodic example.

Installed examples
------------------

After ``pip install -e .`` from the repository root, the examples can be run
directly as installed modules:

.. code-block:: bash

   python -m pymembrane.examples.periodic --quick
   python -m pymembrane.examples.buckling --quick
   python -m pymembrane.examples.minimizer --quick
   python -m pymembrane.examples.disclination --quick
   python -m pymembrane.examples.hybrid_mc_bd --quick

The source versions remain under ``docs/examples``. The installed copies under
``pymembrane.examples`` package the required input data.
