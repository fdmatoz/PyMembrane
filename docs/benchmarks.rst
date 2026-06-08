Benchmarks
==========

Sphere CPU Benchmark
--------------------

PyMembrane now includes a small, reproducible CPU benchmark based on generated
triangulated spherical meshes. The goal is to provide a portable timing
reference for the public release without requiring external mesh downloads,
optional visualization libraries, or path edits.

Why this benchmark exists
-------------------------

The paper uses closed triangulated shells in several places, including the
virus, minimization, and Brownian/Monte Carlo examples. A generated icosphere
benchmark stays close to those use cases while remaining simple enough for a
reviewer to run directly from the installed package.

How meshes are generated
------------------------

The benchmark starts from a regular icosahedron and recursively subdivides each
triangle into four smaller triangles. After each split, the new vertices are
projected back to a sphere of fixed radius. The generated meshes are written to
temporary PyMembrane input files using the same vertex and face format used by
the documented examples.

How to run
----------

From the repository root:

.. code-block:: bash

   pip install -e .
   python -m pymembrane.benchmarks.sphere --quick
   python -m pymembrane.benchmarks.sphere --subdivisions 0 1 2 3 --json sphere_benchmark.json

If you want to keep the generated ``.inp`` files instead of using temporary
directories:

.. code-block:: bash

   python -m pymembrane.benchmarks.sphere --keep-meshes --output-dir sphere_benchmark_output

What is measured
----------------

- Python icosphere mesh generation
- Writing PyMembrane-compatible vertex/face input files
- Mesh loading through ``System.read_mesh_from_files(...)``
- Lightweight geometry/compute calls such as ``edge_lengths()`` and ``volume()``
- Repeated energy evaluation with the same closed-shell force family used in
  the documentation examples
- A short Monte Carlo vertex-move workflow using the existing public integrator

What is not measured
--------------------

- GPU acceleration
- External package comparisons
- Large-scale parallel scaling
- Edge flipping

How to report results
---------------------

When quoting benchmark numbers, include:

- CPU model
- operating system
- Python version
- compiler/build mode if known
- the exact benchmark command used

This benchmark is intended as a portable CPU reference for the public
C++/pybind11 release. Dedicated GPU acceleration remains future work and is not
required to install or run the current examples.

