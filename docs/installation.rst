Installation
============

Quick Check
-----------

These commands provide a short way to verify that the package imports and that
the packaged examples run with their bundled input data:

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

The packaged examples are installed with PyMembrane as self-contained Python
modules. Their runtime input data is bundled inside ``pymembrane.examples``, so
they can be run from any working directory:

.. code-block:: bash

   python -m pymembrane.examples.periodic --quick
   python -m pymembrane.examples.buckling --quick
   python -m pymembrane.examples.minimizer --quick
   python -m pymembrane.examples.disclination --quick
   python -m pymembrane.examples.hybrid_mc_bd --quick
   python -m pymembrane.examples.liquid_membrane --quick

The ``python -m package.module`` form is the standard Python interface for
executing an installed module as top-level code.

Dependency note
---------------

- Normal installation and normal example output do not require VTK.
- The examples do not require plotting packages to run.
- Mesh output is written with the built-in dumper methods such as
  ``system.dumper.vtk(...)`` and can be viewed in tools such as
  `ParaView <https://www.paraview.org/>`_.

Offline HTML documentation
--------------------------

If you want the installed package to carry offline browsable HTML docs, build
the Sphinx site first and copy it into the package bundle before installation:

.. code-block:: bash

   sphinx-build -b html ./docs ./docs/_build/html
   python scripts/sync_offline_docs.py
   pip install .
   python -m pymembrane.docs --open

The installed helper also supports printing the bundled documentation path
without opening a browser:

.. code-block:: bash

   python -m pymembrane.docs

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
