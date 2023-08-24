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

#### Prerequisites

- Currently supported on Linux and Mac OSX.
- Required installations: Python, associated libraries, and suitable compilers.
- Recommended to use [Miniconda](https://docs.conda.io/en/latest/miniconda.html).

##### Steps:

1. Ensure a C++14 or later standard-supported C/C++ compiler is present.

2. Clone the repository:
```bash
git clone https://github.com/fdmatoz/pymembrane.git
```

3. Create a new conda environment:
```bash
conda create -n PYMEMB python=3.8 numpy
```

3. Activate the environment:
```bash
conda activate PYMEMB
```

5. Install necessary packages in the conda environment:
```bash
conda install -c anaconda cmake
```

6. Issues with VTK libraries? PyMembrane utilizes [VTK](https://vtk.org/download/) for VTP files, viewable with [ParaView](https://www.paraview.org/). On Linux, use a package manager (e.g., apt) for VTK libraries. For Mac OSX, let PyMembrane fetch and compile VTK locally.

### Installing the PyMembrane Python Module

From the `pymembrane` directory, execute:
```bash
python setup.py install
```

**Note**: Without Anaconda, you might need root access. The `python setup.py install --user` option is currently unsupported.

### Visualizing Results

Results often use formats like .vtk or .json. Employ visualization tools or the provided scripts for analyzing simulation results. [Paraview](https://www.paraview.org/) is recommended for visualizing mesh files and attributes.

# Documentation & Community

- **Detailed Documentation**: For an in-depth understanding, visit the [official documentation](https://fdmatoz.github.io/PyMembrane/).
  
- **Community Interaction**: Engage with the PyMembrane community through forums, chats, or on GitHub.

## Quick Start: The Disclination Problem

View the [basic example](./docs/Examples/desclination.md) for a hands-on introduction.

# Cite Us



# About

PyMembrane 2023 by [Daniel Matoz Fernandez](http://www.danielmatoz.com).

# License

Distributed under the [MIT license](LICENSE.txt).

