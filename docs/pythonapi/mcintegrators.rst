Monte Carlo Integrators
=======================

In PyMembrane's simulation framework, integrators serve to evolve the system. The `Evolver` class provides the capability to add specific integrators to drive the system's behavior.

Parameters and Integrators
--------------------------

The integrators, such as those belonging to the `"Mesh>MonteCarlo"` family, are added to the system using the `add_integrator` method of the :doc:`Evolver<evolver>` class. Parameters for these integrators are also passed in dictionary format, much like force potentials.

The general format for adding an integrator is:

.. code-block:: python

   evolver.add_integrator("<IntegratorName>", {"<Parameter1>": "<Value1>",
                                               "<Parameter2>": "<Value2>",
                                               ...})

Here's a breakdown of the components:

- `<IntegratorName>`: The specific name of the integrator. Examples include `"Mesh>MonteCarlo>vertex>move"` and `"Mesh>MonteCarlo>vertex>swap"`.
- `<Parameter1>`, `<Parameter2>`, ...: These are parameters that influence the behavior of the integrator. Their nature depends on the specific integrator being used.
- `<Value1>`, `<Value2>`, ...: The corresponding values for the parameters. They are typically passed as strings.

Example
-------

For a clearer understanding, let's consider the provided examples:

.. code-block:: python

   evolver.add_integrator("Mesh>MonteCarlo>vertex>move", {"dr": "0.05"})
   evolver.add_integrator("Mesh>MonteCarlo>vertex>swap", {"every step": "1"})

In these examples:
- The first integrator, :doc:`"Mesh>MonteCarlo>vertex>move"<mcvertexmove>`, uses a parameter `"dr"` with a value of `"0.05"`.
- The second integrator, :doc:`"Mesh>MonteCarlo>vertex>swap"<mcedgeswap>`, has a parameter `"every step"` set to a value of `"1"`.

When working with integrators, ensuring the correct parameterization is essential for accurate and reliable simulation outcomes.

Monte Carlo integrators defined in PyMembrane
----------------------------------------------

.. toctree::
   :maxdepth: 1

   mcvertexmove
   mcvertexswap
   mcedgeswap 
   mcedgeflip