Installation
============

Reviewer Quick Check
--------------------

These commands are the shortest reviewer-facing path to verify that the package
imports and that packaged examples run without changing Python import paths and
without downloading input files manually:

.. code-block:: bash

   conda create -n pymemb python=3.8 numpy cmake pybind11
   conda activate pymemb
   pip install -e .
   python -c "import pymembrane; print(pymembrane.__file__)"
   python -c "from pymembrane import *; print('import ok')"
   python -m pymembrane.examples.periodic --quick
   python -m pymembrane.examples.hybrid_mc_bd --quick

Prerequisites
-------------

- Currently supported on Linux and Mac OSX.
- A C++14-capable compiler is required to build the extension module.
- Recommended to use `Miniconda <https://docs.conda.io/en/latest/miniconda.html>`_.

Editable installation
---------------------

The standard development workflow is an editable install from the repository
root:

.. code-block:: bash

   conda create -n pymemb python=3.8 numpy cmake pybind11
   conda activate pymemb
   pip install -e .
   python -c "import pymembrane; print(pymembrane.__file__)"
   python -c "from pymembrane import *; print('import ok')"

This keeps the Python package linked to the working tree, which is convenient
when editing the code or rebuilding the C++ extension.

Normal build requirements are:

- a C++14-capable compiler,
- CMake,
- pybind11,
- NumPy.

Non-editable installation
-------------------------

If you only want a local install of the current checkout without development
mode, use:

.. code-block:: bash

   pip install .

Rebuild after C++ changes
-------------------------

If you modify the C++ extension sources, remove the local build directory and
reinstall:

.. code-block:: bash

   rm -rf build *.egg-info
   pip install -e .

Examples
--------

The packaged examples are installed with PyMembrane and can be run as Python
modules from any working directory:

.. code-block:: bash

   python -m pymembrane.examples.periodic --quick
   python -m pymembrane.examples.buckling --quick
   python -m pymembrane.examples.minimizer --quick
   python -m pymembrane.examples.disclination --quick
   python -m pymembrane.examples.hybrid_mc_bd --quick

The ``python -m package.module`` form is the standard Python interface for
executing an installed module as top-level code.

Dependency note
---------------

- Normal installation does not require a GPU toolchain.
- Normal installation and normal example output do not require VTK.
- The examples do not require plotting packages to run.
- Mesh output is written with the built-in dumper methods such as
  ``system.dumper.vtk(...)`` and can be viewed in tools such as
  `ParaView <https://www.paraview.org/>`_.

Troubleshooting
---------------

``cmake: command not found``
   Install the build requirements in the active environment:

   .. code-block:: bash

      conda install cmake pybind11 numpy

``C++ compiler not found``
   Install a C++14-capable compiler, then rerun:

   .. code-block:: bash

      pip install -e .

``pybind11 missing`` or ``NumPy headers missing``
   Install both packages in the active environment:

   .. code-block:: bash

      conda install pybind11 numpy

``ModuleNotFoundError: pymembrane``
   Confirm which Python is active and whether the package imports:

   .. code-block:: bash

      python -c "import pymembrane; print(pymembrane.__file__)"

``Example data not found``
   Reinstall from the repository root so the packaged example data is available:

   .. code-block:: bash

      pip install -e .

``Old build artifacts after C++ changes``
   Clean the local build tree and reinstall:

   .. code-block:: bash

      rm -rf build *.egg-info
      pip install -e .

``VTK import errors from old environments``
   PyMembrane no longer needs VTK for normal dumping. Reinstall the current
   package and rerun the example:

   .. code-block:: bash

      pip install -e .
      python -m pymembrane.examples.periodic --quick

Documentation & Community
-------------------------

- **Detailed Documentation**: For an in-depth understanding, visit the `official documentation <https://fdmatoz.github.io/PyMembrane/>`_.
- **Community Interaction**: Engage with the PyMembrane community through forums, chats, or on GitHub.
