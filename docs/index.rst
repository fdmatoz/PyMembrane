PyMembrane
==========

A modular C++/Python framework for simulations of elastic and liquid membranes
on triangulated surfaces.

PyMembrane combines a C++ backend with a Python interface for setting up,
running, and extending membrane simulations. It supports triangulated elastic
shells, dynamically triangulated liquid membranes, Brownian dynamics,
Monte Carlo workflows, minimization, constraints, and lightweight mesh
output.

.. grid:: 2 2 4 4
   :gutter: 3

   .. grid-item-card:: Installation
      :link: installation
      :link-type: doc

      Create a conda environment, build the extension, and verify that the
      package imports correctly.

   .. grid-item-card:: Examples
      :link: examples/quickstart
      :link-type: doc

      Run packaged examples for periodic sheets, shell buckling, liquid
      membranes, hybrid MC-BD workflows, and size scaling.

   .. grid-item-card:: Tutorial notebook
      :link: tutorials
      :link-type: doc

      Follow a beginner-friendly Jupyter notebook that walks through a first
      PyMembrane simulation.

   .. grid-item-card:: Command-line arguments
      :link: examples/command_line_arguments
      :link-type: doc

      See the available command-line options for each packaged example.

   .. grid-item-card:: Python API
      :link: pythonapi/pythonapi
      :link-type: doc

      Browse the Python interface for boxes, systems, evolvers, forces,
      integrators, minimizers, and output.

   .. grid-item-card:: C++ API and extension points
      :link: cppapi/cppapi
      :link-type: doc

      Inspect the underlying C++ classes and the implementation details used
      by the Python bindings.

   .. grid-item-card:: Benchmarks
      :link: benchmarks
      :link-type: doc

      Run the size-scaling benchmark that reports mesh generation and
      simulation timings on spherical meshes.

   .. grid-item-card:: Related examples
      :link: examples/liquid_membrane
      :link-type: doc

      Explore the liquid-membrane and hybrid MC-BD examples built on the
      packaged workflow.

.. rst-class:: spaced-section

Install and check
-----------------

.. code-block:: bash

   conda create -n pymemb python=3.8 numpy cmake pybind11
   conda activate pymemb
   pip install -e .
   python -c "from pymembrane import *; print('import ok')"

Run an example
--------------

.. code-block:: bash

   python -m pymembrane.examples.periodic --quick

Packaged examples include their input data and can be run from the installed
Python namespace.

What can I simulate?
--------------------

- Elastic membranes and shells
- Closed and open triangulated surfaces
- Periodic sheets
- Bending-dominated liquid membranes with edge flips
- Hybrid Monte Carlo and Brownian dynamics workflows
- Energy minimization with constraints

Why PyMembrane?
---------------

PyMembrane is designed for modularity and extensibility. Users can combine
forces, integrators, minimizers, constraints, and output routines through a
Python interface, while performance-critical components are implemented in C++.
The framework is intended for reproducible membrane-modeling workflows and for
developing new membrane models.

.. toctree::
   :hidden:
   :maxdepth: 2
   :caption: Contents:

   installation
   examples/quickstart
   tutorials
   examples/command_line_arguments
   pythonapi/pythonapi
   cppapi/cppapi
   benchmarks
   examples/liquid_membrane
   examples/hybrid_mc_bd
   examples/size_scaling
   contributors
   pythonapi/references

