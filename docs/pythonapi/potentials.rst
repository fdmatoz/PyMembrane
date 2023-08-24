Force Potentials
================

In the PyMembrane framework, force potentials play a vital role in simulating the behavior of the system. These potentials drive the system dynamics and can be precisely controlled using the :doc:`Evolver<evolver>` class.

Parameters and Potentials
-------------------------

The force potentials, such as `"Mesh>Harmonic"`, are introduced into the system using the `add_force` method of the `Evolver` class. A unique aspect of adding these potentials is the way parameters are structured. Parameters for these potentials are passed as dictionaries.

The general format for adding a force potential is:

.. code-block:: python

   evolver.add_force("<PotentialName>", {"<Parameter1>": {"<Type_1>": "<Value_1>", "<Type_2>": "<Value_2>", ...},
                                        "<Parameter2>": {"<Type_1>": "<Value_1>", "<Type_2>": "<Value_2>", ...},
                                        ...})

Here's a breakdown of the components:

- `<PotentialName>`: The name of the force potential. For example: `"Mesh>Harmonic"<harmonic>`.
- `<Parameter1>`, `<Parameter2>`, ...: These are the parameters defined for the potential. In the example given, `"k"` and `"l0"` are the parameters.
- `<Type_i>`: This denotes the type or category of the parameter. `"0"` has been used in the example.
- `<Value_i>`: The value assigned to the specific type of parameter. It should be noted that values are passed as strings, like `str(50.0)` or `str(1.0)`.

Example
-------

Consider the following example:

.. code-block:: python

   evolver.add_force("Mesh>Harmonic", {"k": {"0": "50.0"},
                                      "l0": {"0": "1.0"}})

In this example:
- The potential used is :doc:`"Mesh>Harmonic"<harmonic>`.
- `"k"` is a parameter of this potential with a type `"0"` having a value of `50.0`.
- Similarly, `"l0"` is another parameter with type `"0"` and a value of `1.0`.

It's essential to ensure that all required parameters for a specific potential are provided correctly to achieve the desired simulation results.

Force potentials defined in PyMembrane
---------------------------------------

.. toctree::
   :maxdepth: 1

   harmonic
   cauchygreen
   limit
   linetension
   constantarea
   bending
   bendingdihedral
   bendingmean
   substrate
