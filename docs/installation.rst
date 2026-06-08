Installation
============

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

   rm -rf build
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

The ``python -m package.module`` form is the standard Python interface for
executing an installed module as top-level code.

Dependency note
---------------

- Normal installation does not require VTK.
- The examples do not require plotting packages to run.
- Mesh output is written with the built-in dumper methods such as
  ``system.dumper.vtk(...)`` and can be viewed in tools such as
  `ParaView <https://www.paraview.org/>`_.

Documentation & Community
-------------------------

- **Detailed Documentation**: For an in-depth understanding, visit the `official documentation <https://fdmatoz.github.io/PyMembrane/>`_.
- **Community Interaction**: Engage with the PyMembrane community through forums, chats, or on GitHub.
