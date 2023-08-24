Molecular Dynamics Integrators
===============================

Molecular Dynamics (MD) is a computational method used to model the physical movements of atoms and molecules. In the context of PyMembrane, MD integrators simulate the dynamics of coarse-grained systems. These integrators are especially valuable when considering thermal fluctuations or the dynamics of interacting particles. The `Evolver` class incorporates the capability to include Molecular Dynamics integrators to handle the evolution of the system under various physical interactions.

Parameters and MD Integrators
-----------------------------

Molecular Dynamics integrators in PyMembrane are added to the system using the `add_integrator` method of the `Evolver` class. Like other integrators and force potentials, parameters for MD integrators are passed in a dictionary format.

The typical format to add an MD integrator is:

.. code-block:: python

   evolver.add_integrator("<MD_IntegratorName>", {"<Parameter1>": "<Value1>",
                                                  "<Parameter2>": "<Value2>",
                                                  ...})

A quick breakdown of the components:

- `<MD_IntegratorName>`: The specific name of the Molecular Dynamics integrator.
- `<Parameter1>`, `<Parameter2>`, ...: Parameters that guide the behavior of the integrator.
- `<Value1>`, `<Value2>`, ...: The corresponding values for the parameters, typically passed as strings.

Example
-------

Consider the following example:

.. code-block:: python

   evolver.add_integrator("Mesh>brownian", {"T": "0.2"})

In this example:
- The integrator, :doc:`"Mesh>brownian"<mdbrownian>`, represents a Brownian dynamics simulation, which models the random motion of particles suspended in a fluid resulting from their collision with the quick atoms or molecules in the gas or liquid.
- The parameter `"T"` denotes the temperature of the system and is set to a value of `"0.2"` (in appropriate units).


Molecular dynamics integrators defined in PyMembrane
-----------------------------------------------------

.. toctree::
   :maxdepth: 1

   mdbrownian
    