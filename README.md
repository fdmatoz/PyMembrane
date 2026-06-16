<p align="center">
  <img src="docs/_static/logo-github.png" alt="PyMembrane logo" width="750">
</p>

PyMembrane

PyMembrane is a simulation framework for elastic and liquid membranes. It combines a high-performance C++ backend with a Python interface for defining systems, forces, integrators, and simulation workflows.

Getting Started

The recommended workflow is to install PyMembrane in a dedicated conda environment and run the examples provided with the Python package.

Installation

PyMembrane builds a CPU-based C++ extension using CMake and pybind11. A compiler with C++14 support is required.

1. Create a conda environment

conda create -n pymemb python=3.8 numpy cmake pybind11
conda activate pymemb

2. Install PyMembrane

From the repository root, install the package in editable mode:

pip install -e .

Verify the installation:

python -c "import pymembrane; print(pymembrane.__file__)"
python -c "from pymembrane import *; print('import ok')"

Clean rebuild

After modifying the native C++ code, remove previous build artifacts before reinstalling:

rm -rf build *.egg-info
pip install -e .

Non-editable installation

For a standard installation:

pip uninstall -y pymembrane
rm -rf build *.egg-info
pip install .

Verify the installed package:

python -c "import pymembrane; print(pymembrane.__file__)"
python -c "from pymembrane import *; print('import ok')"

Troubleshooting

When installation fails, confirm that the active environment contains:

* a C++14-compatible compiler;
* CMake;
* pybind11; and
* NumPy.

VTK is not required for the default PyMembrane installation.

Offline Documentation

Prebuilt HTML documentation can optionally be included in the installed package. First build the documentation, then copy it into the package before installing:

sphinx-build -b html ./docs ./docs/_build/html
python scripts/sync_offline_docs.py
pip install .

Open the installed documentation with:

python -m pymembrane.docs --open

Sphinx is required only to build the documentation. It is not required to use an installation that already contains the prebuilt HTML files.

Running the Examples

Runnable examples are distributed under pymembrane.examples and include their required input data. They can therefore be executed from any working directory after installation.

python -m pymembrane.examples.periodic --quick
python -m pymembrane.examples.minimizer --quick
python -m pymembrane.examples.buckling --quick
python -m pymembrane.examples.disclination --quick
python -m pymembrane.examples.size_scaling --quick
python -m pymembrane.examples.liquid_membrane --quick

For example, an installed example can be run from outside the repository:

cd /tmp
python -m pymembrane.examples.periodic --quick

The --quick option runs a shortened version of an example for checking the installation and packaged workflow. It reduces the runtime while retaining the same physical setup, force models, and overall simulation procedure as the full example.

The liquid-membrane example demonstrates dynamic triangulation using Monte Carlo edge flips.

The size-scaling example generates spherical meshes at increasing resolutions and measures the time required for:

* mesh generation;
* Monte Carlo vertex moves; and
* Brownian dynamics.

The --quick option is intended only as a functional check. For more stable timing measurements, increase --steps and use --repeat 3 or higher.

Documentation source examples are located under docs/examples. Their installed, runnable counterparts are located under pymembrane.examples and include the input files required at runtime.

Visualizing Results

Simulation output is written through the PyMembrane dumper interface:

system.dumper.vtk("output")
system.dumper.obj("output")

The default dumper writes legacy ASCII .vtk files directly from Python. The VTK Python package is therefore not required.

VTK output can be opened in ParaView or other applications that support legacy VTK POLYDATA files. OBJ output is also available for lightweight geometry inspection.

Additional supported output formats include .ply, .json, and plain-text data.

Minimal Example

A typical PyMembrane simulation follows this structure:

from pymembrane import Box, Evolver, System
box = Box(50.0, 50.0, 50.0)
system = System(box)
system.read_mesh_from_files(
    files={
        "vertices": "vertices.dat",
        "faces": "faces.dat",
    }
)
evolver = Evolver(system)
evolver.add_force(
    "Mesh>Harmonic",
    {
        "k": {"0": "100.0"},
        "l0": {"0": "1.0"},
    },
)
evolver.add_integrator(
    "Mesh>MonteCarlo>vertex>move",
    {
        "dr": "0.01",
    },
)
evolver.set_global_temperature("1e-6")
evolver.evolveMC(steps=100)
system.dumper.vtk("output")

See the packaged examples for complete, runnable implementations of the simulations presented in the documentation and accompanying manuscript.