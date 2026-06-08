[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

<p align="center">
  <img height="300" src="docs/_static/logo-github.png">
</p>

### Overview:

PyMembrane is an advanced simulation software crafted to model liquid and elastic membranes. This robust tool captures the essence of these membranes through a coarse-grained representation built on unstructured, triangulated two-dimensional meshes that are positioned within a three-dimensional space.

### Technicalities:

- **Programming Language:** At its core, PyMembrane is constructed in C++. This ensures rapid and efficient computations even for the most complex simulations.
  
- **User Interface:** The software sports an intuitive Python interface, melding the power of C++ with the simplicity of Python. This unique blend facilitates quick setup, execution, and analysis phases.

- **Software Engineering:** PyMembrane is grounded in modern software engineering principles, ensuring a design that is both modular and robust. This modular architecture not only guarantees consistency but also ensures that the software remains easy to maintain.
  
- **Customization:** A distinguishing feature of PyMembrane is its inherent flexibility. The software is built with standardized C++ interfaces, simplifying the addition of new force models or motion equation integrators.
  
- **Efficiency:** The software harnesses widely recognized data structures for its unstructured meshes. This ensures prompt mesh operations and accurate force computations.

### Extendibility:

The modular design is the backbone of PyMembrane, promoting straightforward implementation of custom extensions. This ensures that as the research realm evolves, PyMembrane can seamlessly adapt, accommodating new features with ease. This emphasis on adaptability and user-centric design positions PyMembrane as a top-tier choice for membrane simulations.

## Getting Started

PyMembrane is not just about powerful computations; it's also about simplicity. With an assortment of templates tailored for typical simulations, bolstered by comprehensive documentation, users can effortlessly configure and initiate research-grade simulations.

### Installation

PyMembrane builds a CPU C++ extension through CMake and pybind11.

Create an environment with the build dependencies:

```bash
conda create -n pymemb python=3.8 numpy cmake pybind11
conda activate pymemb
```

Install from the repository root:

```bash
pip install -e .
python -c "import pymembrane; print(pymembrane.__file__)"
python -c "from pymembrane import *; print('import ok')"
```

For a clean rebuild after native C++ changes:

```bash
rm -rf build *.egg-info
pip install -e .
```

Non-editable installs use the same package metadata:

```bash
pip uninstall -y pymembrane
rm -rf build *.egg-info
pip install .
python -c "import pymembrane; print(pymembrane.__file__)"
python -c "from pymembrane import *; print('import ok')"
```

If the compiler, CMake, pybind11, or NumPy headers are missing, install them
in the active environment and rerun the commands above. A C++14-capable
compiler is required.

### Visualizing Results

Results often use formats like .vtk or .json. Employ visualization tools or the provided scripts for analyzing simulation results. [Paraview](https://www.paraview.org/) is recommended for visualizing mesh files and attributes.

Normal dumping no longer requires VTK to be installed. The default Python dumper preserves the historic method names such as `s.dumper.vtk(...)` and `s.dumper.edge_vtk(...)`, but now writes legacy ASCII VTK directly from Python. `s.dumper.obj(...)` also writes a lightweight Wavefront OBJ mesh.

# Documentation & Community

- **Detailed Documentation**: For an in-depth understanding, visit the [official documentation](https://fdmatoz.github.io/PyMembrane/).
  
- **Community Interaction**: Engage with the PyMembrane community through forums, chats, or on GitHub.

## Quick Start

The standard reviewer-facing workflow is:

```bash
pip install -e .
python -m pymembrane.examples.periodic --quick
```

The documentation source scripts remain under `docs/examples`, and the
installed runnable copies live under `pymembrane.examples`.

### Smoke Test

From any working directory, run one of the packaged examples:

```bash
cd /tmp
python -m pymembrane.examples.periodic --quick
```

After `pip install .`, the examples are also available from the installed
package namespace, so they work even if you delete the repository:

```bash
python -m pymembrane.examples.periodic --quick
python -m pymembrane.examples.minimizer --quick
python -m pymembrane.examples.buckling --quick
python -m pymembrane.examples.disclination --quick
```

# Cite Us



# About

PyMembrane 2023 by [Daniel Matoz Fernandez](http://www.danielmatoz.com).

# License

Distributed under the [MIT license](LICENSE.txt).
