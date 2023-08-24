PyMembrane Tutorial
===================

This tutorial provides an introduction to the PyMembrane package, focusing on its applications in the physics of liquid and solid membranes. The package includes fully commented Python codes along with input files to facilitate understanding and usage (refer to Matoz, 2023).

Objective
---------
The demonstrations herein not only highlight PyMembrane's capabilities but also guide users through a standard workflow for setting up a simulation. The examples follow a consistent layout, with code comments providing details on each step.

Key Steps
---------
1. **Step 1**: Initialize the simulation box by creating an instance of the ``Box`` class.
2. **Step 2**: Create an instance of the ``System()`` class to handle mesh, boundary conditions, and visualization outputs.
3. **Step 3**: Create an instance of the ``Evolver()`` class, which is responsible for managing energy and force models, constraints, integrators, minimizers, and simulation runs (using methods like ``evolveMC(steps)``, ``evolveMD(steps)``, or ``minimize()``).

Typical Use Case for System and Evolver Classes
-----------------------------------------------
.. code-block:: python
    import pymembrane as pyb
    # Box
    box = pyb.Box(Lx, Ly, Lz, periodic)
    # System
    system = pyb.System(box)
    system.read_mesh_from_files(files={'vertices': vertex_file, 
                                       'faces': face_file})
    # Evolver
    evolver = pyb.Evolver(s)

Force Potentials and Integrators
--------------------------------
.. code-block:: python
    # Add a force and integrator to the system
    evolver.add_force('<PotentialName>', {'<Parameter1>': {'<Type_1>': '<Value_1>', ...},
                                          '<Parameter2>': {'<Type_1>': '<Value_1>', ...},
                                          ...})
    # Add an integrator to the system
    evolver.add_integrator('<IntegratorName>', {'<Parameter1>': '<Value1>',
                                                '<Parameter2>': '<Value2>',
                                                ...})

Models
------
Stretching Energy
    The discrete stretching energy uses Hookean springs assigned to mesh edges and is modeled as:
    .. math:: E_{\text{stretch}} = \frac{1}{2} k (l_e - l_0)^2
    In PyMembrane, this is implemented with the setting 'Mesh>Harmonic', using parameters 'k' for spring constant and 'l0' for rest length.

Bending Energy
    The bending energy model is based on the dihedral angle between triangles sharing an edge:
    .. math:: E_{\text{bend}} = \frac{1}{2}\kappa \sum_{e} (1-\hat{\mathbf{n}}_{e}^{(1)}\cdot\hat{\mathbf{n}}_{e}^{(2)})

Edge Length Limit
    To prevent deformed triangles causing numerical instability, PyMembrane restricts edge length:
    .. math::
       E =
       \begin{cases}
       0 & l_{\text{min}} < l_e < l_{\text{max}}\\
       \infty & \text{otherwise}
       \end{cases},
    You can set these limits using 'Mesh>Limit', with parameters 'lmin' and 'lmax'.

Additional Details
------------------
We have chosen the simplest models for demonstration purposes, although other combinations of stretching and bending energy models are available. Consult the full documentation :doc:`../pythonapi/potentials` in the main documentation for more information.

Please refer to the full documentation and examples provided in the package for further understanding and practical insights into using PyMembrane for your specific membrane physics simulations.