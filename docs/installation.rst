Installation
============

Prerequisites
-------------

- Currently supported on Linux and Mac OSX.
- Required installations: Python, associated libraries, and suitable compilers.
- Recommended to use `Miniconda <https://docs.conda.io/en/latest/miniconda.html>`_.

Steps:
^^^^^^

1. Ensure a C++14 or later standard-supported C/C++ compiler is present.

2. Clone the repository:

   .. code-block:: bash
      git clone https://github.com/fdmatoz/PyMembrane.git

3. Create a new conda environment:
   .. code-block:: bash
      conda create -n PYMEMB python=3.8 numpy

4. Install necessary packages in the conda environment:
   .. code-block:: bash
      conda install -c anaconda cmake

5. Issues with VTK libraries? PyMembrane utilizes [VTK](https://vtk.org/download/) for VTP files, viewable with [ParaView](https://www.paraview.org/). On Linux, use a package manager (e.g., apt) for VTK libraries. For Mac OSX, let PyMembrane fetch and compile VTK locally.

Installing the PyMembrane Python Module
---------------------------------------

From the `pymembrane` directory, execute:

.. code-block:: bash

   python setup.py install

.. note::
   Without Anaconda, you might need root access. The :code::`python setup.py install --user` option is currently unsupported.

Visualizing Results
-------------------

Results often use formats like .vtk or .json. Employ visualization tools or the provided scripts for analyzing simulation results. `Paraview <https://www.paraview.org/>`_ is recommended for visualizing mesh files and attributes.

Documentation & Community
-------------------------

- **Detailed Documentation**: For an in-depth understanding, visit the `official documentation <https://fdmatoz.github.io/PyMembrane/>`_.
  
- **Community Interaction**: Engage with the PyMembrane community through forums, chats, or on GitHub.



