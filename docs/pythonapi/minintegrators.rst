Minimization Algorithms
=======================

Minimizers in PyMembrane
========================

Minimization is an important operation in many computational systems, allowing for the optimization of configurations or properties. In the PyMembrane framework, minimizers work to find an optimal state of the system by adjusting configurations to minimize certain energy or force criteria.

Parameters and Minimizers
-------------------------

Minimizers in PyMembrane are integrated into the system using the `add_minimizer` method of the `Evolver` class. Like the other components, parameters for minimizers are provided in a dictionary format.

The standard format to integrate a minimizer is:

.. code-block:: python

   evolver.add_minimizer("<MinimizerName>", {"<Parameter1>": "<Value1>",
                                             "<Parameter2>": "<Value2>",
                                             ...})

Here's a quick breakdown:

- `<MinimizerName>`: The specific name of the minimizer.
- `<Parameter1>`, `<Parameter2>`, ...: Parameters specific to the behavior of the minimizer.
- `<Value1>`, `<Value2>`, ...: The corresponding values for these parameters, typically specified as strings.

Example
-------

Take the following as an illustrative example:

.. code-block:: python

   evolver.add_minimizer("Mesh>Fire", {"dt": "0.01", "max_iter": "100000", "ftol": "0.0001", "etol": "0.0000001"})

In this instance:
- The minimizer :doc:`"Mesh>Fire"<fire>` refers to the Fast Inertial Relaxation Engine (FIRE) algorithm, which is particularly effective for energy minimization in atomistic simulations.
- `"dt"` specifies the time step for the minimization, set to `"0.01"`.
- `"max_iter"` denotes the maximum number of iterations for the minimizer, in this case, `"100000"`.
- `"ftol"` and `"etol"` represent force and energy tolerance levels, respectively. The minimization stops when these thresholds are reached, ensuring an optimized and stable configuration.

It's crucial to fine-tune the parameters when using minimizers to ensure the effectiveness and reliability of the simulation results.


Minimizers defined in PyMembrane
--------------------------------

.. toctree::
   :maxdepth: 1

    fire
   
    