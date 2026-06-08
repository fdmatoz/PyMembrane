# PyMembrane

PyMembrane is a CPU C++ membrane simulation package exposed to Python through
pybind11. The public Python package name is `pymembrane`.

## Developer Installation

Create a conda environment with the native build dependencies. A C++14-capable
compiler and CMake must be available in the active environment:

```bash
conda create -n pymemb python=3.8 numpy cmake pybind11
conda activate pymemb
```

From the repository root, install the package with pip:

```bash
pip install -e .
```

For day-to-day development, use an editable install:

```bash
pip install -e .
```

Run an import smoke test:

```bash
python -c "import pymembrane; print(pymembrane.__file__)"
python -c "from pymembrane import *; print('import ok')"
```

For native C++ changes, rebuild the extension with:

```bash
rm -rf build *.egg-info
pip install -e .
```

## Runtime Smoke Test

After installation, run the quick example from any directory:

```bash
cd /tmp
python /path/to/repo/examples/smoke.py --quick --output-dir /tmp/pymembrane-smoke
```

Or run the minimal test suite:

```bash
pytest
```

Non-editable installs use the same workflow:

```bash
pip uninstall -y pymembrane
rm -rf build *.egg-info
pip install .
python -c "import pymembrane; print(pymembrane.__file__)"
python -c "from pymembrane import *; print('import ok')"
```

## Visualization Dumps

PyMembrane does not require VTK for installation. The default Python dumper
writes lightweight legacy ASCII VTK and OBJ files directly:

```python
s.dumper.vtk("output")
s.dumper.obj("output")
```

`s.dumper.vtk("output")` writes `output.vtk` using the lightweight ASCII
legacy VTK `POLYDATA` format. The file is readable by ParaView and similar
tools.

`s.dumper.obj("output")` writes `output.obj` using Wavefront OBJ geometry with
1-based face indices. OBJ is intended for quick geometry inspection and broad
tool compatibility.

## Troubleshooting

- `cmake: command not found`: install CMake in the active environment.
- Compiler errors: install the macOS command-line tools or a conda-forge
  compiler toolchain, then rerun `rm -rf build *.egg-info && pip install -e .`.
- `ModuleNotFoundError: pymembrane`: run `pip install -e .` from the
  repository root so the package is installed in the active environment.
- `ModuleNotFoundError: pymembrane.md._core`: check the CMake output for C++
  compiler errors or missing build dependencies.
- Missing build dependencies: make sure `setuptools`, `wheel`, `cmake`,
  `pybind11`, and `numpy` are installed in the active conda environment.
- This project does not require CUDA or any GPU toolchain.
