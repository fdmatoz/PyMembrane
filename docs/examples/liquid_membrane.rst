Liquid membrane example
=======================

This example demonstrates a bending-dominated dynamically triangulated
membrane. Vertex moves update the membrane shape, while edge flips update mesh
connectivity.

What This Example Demonstrates
------------------------------

- a simple liquid-membrane workflow built from the standard PyMembrane API
- Monte Carlo vertex moves combined with Monte Carlo edge flips
- bending energy as the main physical contribution
- a weak harmonic regularizer used only to keep the mesh well conditioned in
  this minimal example
- the ``Box`` / ``System`` / ``Evolver`` / dumper workflow used throughout the
  packaged examples

The example uses ``Mesh>Bending>Dihedral`` as the main energy term and
``Mesh>Limit`` to keep the mesh in a stable range during the short run. A weak
``Mesh>Harmonic`` term is included only as a regularizer for this minimal
example.

How to Run
----------

.. code-block:: bash

   python -m pymembrane.examples.liquid_membrane --quick

For a longer run with files written into a separate directory:

.. code-block:: bash

   python -m pymembrane.examples.liquid_membrane --steps 1000 --output-dir liquid_output

Command-Line Arguments
----------------------

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

Expected Output
---------------

The example writes:

- ``initial.vtk``
- ``final.vtk``
- ``final.obj``
- ``summary.json``
- optionally ``mesh_inputs/*.inp`` when generated mesh inputs are retained

Quick Mode
----------

``--quick`` runs a short version of the same model. It keeps the
same mesh generation, force model, and integrator names while reducing the
number of Monte Carlo steps.

Scope Note
----------

This example demonstrates a bending-dominated dynamically triangulated
membrane. Vertex moves update the membrane shape, while edge flips update mesh
connectivity. A weak harmonic term is used only as a regularizer in this
minimal example; the liquid-membrane behavior comes from bending energy,
limit protection, and Monte Carlo edge flips. For large-scale simulations
where maximum performance for fluid membranes is the primary objective,
specialized dynamically triangulated membrane packages may be preferable.
PyMembrane’s focus is on providing a modular and extensible C++/Python
framework in which forces, integrators, minimizers, constraints, and output
routines can be combined and extended for new membrane models.
