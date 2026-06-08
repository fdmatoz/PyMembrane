# PyMembrane C++ Backend

The `md` directory contains the C++/pybind11 backend built by the top-level
`setup.py`.

From the repository root:

```bash
pip install .
pip install -e .
```

The build requires a C++ compiler, CMake, pybind11, and NumPy. The generated
extension is installed into `pymembrane/md/` and imported as
`pymembrane.md._core`.
