Tutorial Notebook
=================

This page links to a beginner-friendly Jupyter notebook that walks through one
complete PyMembrane simulation using the existing packaged periodic example
physics. The required input files are bundled with the installed package and
the notebook does not require plotting packages.

Beginner Notebook
-----------------

Download the notebook:

- :download:`pymembrane_first_simulation.ipynb <tutorials/pymembrane_first_simulation.ipynb>`

Open it locally with Jupyter if you want the step-by-step tutorial format:

.. code-block:: bash

   jupyter notebook docs/tutorials/pymembrane_first_simulation.ipynb

or:

.. code-block:: bash

   jupyter lab docs/tutorials/pymembrane_first_simulation.ipynb

Jupyter is optional and is not required for normal PyMembrane installation.

Equivalent packaged example
---------------------------

The notebook uses the same packaged periodic example physics that is already
available from the installed module namespace:

.. code-block:: bash

   python -m pymembrane.examples.periodic --quick

The hybrid example is also available as:

.. code-block:: bash

   python -m pymembrane.examples.hybrid_mc_bd --quick

Notes
-----

- The notebook is a lightweight tutorial run, not a production simulation.
- Output files are written with the default dumper as ``.vtk`` and ``.obj``.
- The input mesh files come from the bundled example data shipped with PyMembrane.
