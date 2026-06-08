# PyMembrane Edge-Flip Diagnosis

## Executive Summary

The native package now installs and imports successfully as `pymembrane`, but the documented Monte Carlo edge-flip workflow is still not present in the inspected backend registry. The installed Python surface exposes `Evolver.add_integrator(...)` and `MeshOperations.flip_edge(...)`, but the real backend source registers only Brownian, NVT/NPT, Langevin, and sphere-dipole integrators. There is no `Mesh>MonteCarlo>edge>flip` registration in `EvolverClass::add_integrator`.

The lower-level C++ edge-flip primitive is exposed to Python as `MeshOperations.flip_edge(edge_index, flip_face_up)`. Its implementation performs a linear scan over all edges to find the requested index before calling the local flip helper. That is likely expensive for a future Monte Carlo driver that calls the primitive once per attempted move, but the bigger issue remains API completeness: the public MC edge-flip integrator is not registered in the backend source inspected here.

The benchmark added for this diagnosis therefore measures the exposed primitive, not a public Monte Carlo edge-flip integrator. Accepted/rejected counters are not exposed by the current Python API, so they remain `null` in the benchmark output.

## Installed Package And Environment

Recorded successful install/import path:

```text
/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/pymembrane/__init__.py
```

Environment used for the validation run:

| Field | Value |
|---|---|
| OS | Darwin 25.5.0 arm64 |
| Python | Python 3.8.20 |
| CMake | 4.2.3 |
| Compiler | Apple clang 17.0.0 |
| Install mode | editable install succeeded |
| Native extension | `/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/pymembrane/md/_core.cpython-38-darwin.so` |

## Python API Surface

Observed from the installed package:

```python
import pymembrane
[x for x in dir(pymembrane) if not x.startswith("_")]
```

Returned:

```text
['Dumper', 'DumperClass', 'Evolver', 'HE_EdgeProp', 'HE_FaceProp', 'HE_HalfEdgeProp', 'HE_VertexProp', 'MeshOperations', 'Path', 'System', 'absolute_import', 'bool3', 'box', 'dump', 'dumper', 'inth3', 'json', 'md', 'np', 'pickle', 'real3', 'realTensor']
```

Relevant class members:

| Class | Relevant members |
|---|---|
| `Evolver` | `add_integrator`, `evolve` |
| `MeshOperations` | `flip_edge` |
| `System` | no direct edge-flip methods; contains `dumper` |
| `DumperClass` | `vtk`, `obj` |

The installed Python surface does expose a generic `Evolver.add_integrator(...)` method, but the backend registry still has no matching edge-flip Monte Carlo integrator name in source.

## Source Inspection

Public integrator registry in the backend source:

- `md/src/evolver/evolverclass.cpp:166-220`

The inspected registry contains:

- `Mesh>Brownian>Vertex`
- `Mesh>Brownian-Controlled>Vertex`
- `Mesh>NVT>Vertex`
- `Mesh>NPT>Vertex`
- `Mesh>NPT>Berendsen>Vertex`
- `Mesh>Langevin>Vertex`
- `Mesh>Langevin-Limit>Vertex`
- `Mesh>Brownian>SphereDipole`
- `Mesh>Particles>SphereDipole`

There is no `Mesh>MonteCarlo>edge>flip` entry.

Python-facing lower-level edge-flip primitive:

- `md/src/mesh/pybind_export_meshoperations.hpp:28-33`

Native implementation:

- `md/src/mesh/meshoperations.cpp:8-48`

The edge-flip kernel scans edge indices and only flips when the current index matches `flip_edge_index`:

```cpp
for (int e = blockIdx.x * blockDim.x + threadIdx.x; e < Numedges; e += blockDim.x * gridDim.x)
{
    if (e == flip_edge_index)
    {
        device::EdgeFlip_dev(...);
    }
}
```

That is an O(E) lookup for a single requested edge.

Local topology mutation helper:

- `md/src/mesh/edge_flip.hpp:113-309`

Full-mesh Python copies are exposed by the system getters:

- `md/src/system/systemclassmesh.cpp:53-73`

These getters return copied host vectors for vertices, edges, faces, and halfedges.

## Benchmark Commands And Results

Command lines run:

```bash
python benchmarks/benchmark_edge_flip.py --preset tiny --steps 10 --seed 1 --json /tmp/edge_flip_tiny.json
python benchmarks/benchmark_edge_flip.py --preset small --steps 1000 --seed 1 --json /tmp/edge_flip_small.json
python benchmarks/benchmark_edge_flip.py --preset small --steps 100 --seed 1 --profile
```

Observed output:

| Preset | Mesh | V / E / F | Steps | Wall time | Steps/s | Attempted flips | Accepted / Rejected |
|---|---|---:|---:|---:|---:|---:|---:|
| tiny | icosphere_subdivisions_0 | 12 / 30 / 20 | 10 | `1.9917e-05 s` | `5.020836e+05` | 10 | `null / null` |
| small | icosphere_subdivisions_1 | 42 / 120 / 80 | 1000 | `7.21375e-04 s` | `1.386242e+06` | 1000 | `null / null` |
| small + profile | icosphere_subdivisions_1 | 42 / 120 / 80 | 100 | `2.415e-04 s` | `4.140787e+05` | 100 | `null / null` |

Profile output for the 100-step run showed the Python time dominated by `random.randrange` calls and the benchmark loop. That is expected because the current benchmark exercises the exposed primitive once per step from Python and does not contain a public MC move scheduler.

## Per-Attempt Workflow

For the exposed lower-level `MeshOperations.flip_edge(edge_index, flip_face_up)` primitive:

1. Python calls the pybind method `MeshOperations.flip_edge`.
2. The C++ wrapper launches `EdgeFlip_kernel`.
3. The kernel linearly scans edge indices until it finds `flip_edge_index`.
4. The helper `device::EdgeFlip_dev(...)` validates the local quadrilateral.
5. Boundary edges are rejected.
6. Local connectivity counts around the two adjacent vertices are checked.
7. Local angles and triangulation checks are evaluated.
8. On success, six halfedges, two face records, and selected vertex/edge records are updated locally.

The primitive does not implement a Metropolis criterion, scheduler, or persistent accepted/rejected counters. Those are not exposed by the current Python API.

## Bottleneck Analysis

### Verified Bottleneck: O(E) Edge Lookup

The lower-level primitive scans all edges to locate the requested edge. This is confirmed by `md/src/mesh/meshoperations.cpp:18-24`.

### Verified Limitation: No Public Monte Carlo Edge-Flip Integrator

`EvolverClass::add_integrator` does not register `Mesh>MonteCarlo>edge>flip`. The documented public workflow cannot be exercised through that exact string in the inspected backend source.

### Not Supported By Current Evidence: Global Energy Recompute

The inspected primitive in `meshoperations.cpp` does not call the system-wide energy routines. There is no evidence here of a global energy recomputation inside the lower-level edge-flip primitive.

### Not Supported By Current Evidence: Full Topology Rebuild

The successful flip path mutates the local quadrilateral and nearby adjacency only. The source does not rebuild the full mesh connectivity for each flip.

### Potential Python Overhead If A Future MC Driver Calls The Primitive Per Move

Any external Python driver that calls `MeshOperations.flip_edge()` once per attempt will pay one Python/C++ boundary crossing per move.

### Potential Python Copy Overhead

The system getters return copied vectors. Repeatedly reading `system.vertices`, `system.edges`, `system.faces`, or `system.halfedges` inside an inner loop would copy full mesh data repeatedly.

### I/O And Printing

The primitive itself does not write files. The equiangulation helper prints for flipped edges, but that is a separate path.

## Current Limitations

- The benchmark measures the exposed primitive, not a public Monte Carlo edge-flip integrator.
- Accepted/rejected counts are not exposed by the current API.
- The public `Mesh>MonteCarlo>edge>flip` registry entry is absent from the inspected backend source.
- The benchmark’s timing is therefore useful for the primitive path only, not for a full MC simulation loop.

## Recommended Next Patch

1. Add or restore a CPU C++ `Mesh>MonteCarlo>edge>flip` integrator.
2. Register it in the same factory/registry used by the other integrators.
3. Expose it through:

```python
e.add_integrator("Mesh>MonteCarlo>edge>flip", {...})
```

4. Add counters for attempted, accepted, and rejected flips.
5. Add a tiny smoke test proving the integrator can be added and run from Python.
6. Only after the public integrator exists, benchmark and optimize edge lookup and local energy recomputation.

## Acceptance Criteria For The Next Patch

- `Evolver.add_integrator("Mesh>MonteCarlo>edge>flip", {...})` succeeds with a real MC implementation.
- A tiny mesh can run at least one MC edge-flip step from Python.
- Attempted, accepted, and rejected counts are visible.
- The benchmark can switch between primitive and integrator modes.
- The report can compare primitive overhead versus real MC scheduling overhead.
