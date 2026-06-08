# PyMembrane Monte Carlo Edge-Flip Diagnosis

## Executive summary

The Monte Carlo edge-flip implementation does exist in the current backend source.

- It is defined in C++ in [`pymembrane/cppmodule/md/src/integrators/montercarlo_flip_edge.hpp`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/md/src/integrators/montercarlo_flip_edge.hpp) and [`pymembrane/cppmodule/md/src/integrators/montercarlo_flip_edge.cpp`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/md/src/integrators/montercarlo_flip_edge.cpp).
- It is included by compiled code through [`pymembrane/cppmodule/md/src/evolver/evolverclass.cpp`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/md/src/evolver/evolverclass.cpp).
- It is registered in `EvolverClass::add_integrator(...)` under the exact string `Mesh>MonteCarlo>edge>flip`.
- It is callable from Python today through `Evolver.add_integrator("Mesh>MonteCarlo>edge>flip", {...})`.
- The lower-level `MeshOperations` helper class exists in C++ source, but it is not exposed in the installed Python module inspected here.
- It uses a Metropolis acceptance rule and rolls back rejected moves by calling the same flip routine a second time.
- It uses edge-local energy differences via `compute_edge_energy(edge_index)`, not a full-system `compute.energy(...)` call.
- It does not expose attempted, rejected, or invalid counters through the public API inspected here. `evolveMC(...)` only returns accepted counts per registered integrator.

The main code-level performance concern visible from inspection is algorithmic: each Monte Carlo sweep loops over all edges and for each edge performs at least one connectivity-changing flip attempt and two local edge-energy evaluations. Rejected moves perform the flip twice. There is no evidence in the inspected edge-flip integrator of full-mesh copying or global system-energy recomputation.

## Files inspected

Core implementation and registration:

- [`pymembrane/cppmodule/md/src/integrators/montercarlo_flip_edge.hpp`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/md/src/integrators/montercarlo_flip_edge.hpp)
- [`pymembrane/cppmodule/md/src/integrators/montercarlo_flip_edge.cpp`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/md/src/integrators/montercarlo_flip_edge.cpp)
- [`pymembrane/cppmodule/md/src/evolver/evolverclass.cpp`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/md/src/evolver/evolverclass.cpp)
- [`pymembrane/cppmodule/md/src/evolver/evolverclass.hpp`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/md/src/evolver/evolverclass.hpp)
- [`pymembrane/cppmodule/md/src/evolver/pybind_export_evolver.hpp`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/md/src/evolver/pybind_export_evolver.hpp)
- [`pymembrane/cppmodule/md/src/integrators/montecarlointegrator.hpp`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/md/src/integrators/montecarlointegrator.hpp)

Low-level topology helper:

- [`pymembrane/cppmodule/md/src/mesh/edge_flip.hpp`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/md/src/mesh/edge_flip.hpp)
- [`pymembrane/cppmodule/md/src/mesh/meshoperations.cpp`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/md/src/mesh/meshoperations.cpp)
- [`pymembrane/cppmodule/md/src/mesh/meshoperations.hpp`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/md/src/mesh/meshoperations.hpp)

Comparable Monte Carlo integrators:

- [`pymembrane/cppmodule/md/src/integrators/montercarlo_vertex.hpp`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/md/src/integrators/montercarlo_vertex.hpp)
- [`pymembrane/cppmodule/md/src/integrators/montercarlo_swap_vertex.hpp`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/md/src/integrators/montercarlo_swap_vertex.hpp)
- [`pymembrane/cppmodule/md/src/integrators/montercarlo_swap_vertex.cpp`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/md/src/integrators/montercarlo_swap_vertex.cpp)
- [`pymembrane/cppmodule/md/src/integrators/montercarlo_swap_edge.hpp`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/md/src/integrators/montercarlo_swap_edge.hpp)

Build and Python package surface:

- [`pymembrane/cppmodule/md/CMakeLists.txt`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/md/CMakeLists.txt)
- [`pymembrane/cppmodule/__init__.py`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/__init__.py)
- [`pymembrane/cppmodule/md/src/mesh/pybind_export_mesh.hpp`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/md/src/mesh/pybind_export_mesh.hpp)

Existing auxiliary files found during inspection:

- [`docs/attached/edgeflip.py`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/docs/attached/edgeflip.py)
- [`docs/pythonapi/mcedgeflip.rst`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/docs/pythonapi/mcedgeflip.rst)
- [`pymembrane/cppmodule/benchmarks/benchmark_edge_flip.py`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/benchmarks/benchmark_edge_flip.py)
- [`pymembrane/cppmodule/tests/test_benchmark_edge_flip.py`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/tests/test_benchmark_edge_flip.py)

## Build and registration status

### Does the implementation exist?

Yes.

- Class: `MonteCarloIntegratorFlipEdge`
- Header: [`pymembrane/cppmodule/md/src/integrators/montercarlo_flip_edge.hpp`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/md/src/integrators/montercarlo_flip_edge.hpp)
- Implementation: [`pymembrane/cppmodule/md/src/integrators/montercarlo_flip_edge.cpp`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/md/src/integrators/montercarlo_flip_edge.cpp)

### Is it included by the build?

Yes.

Evidence:

- [`pymembrane/cppmodule/md/CMakeLists.txt`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/md/CMakeLists.txt) uses:
  - `file(GLOB_RECURSE PYMEMBRANE_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")`
- That glob includes `src/integrators/montercarlo_flip_edge.cpp`.
- The only explicit source exclusions in that file are:
  - dumper `.cpp`
  - `meshoperations.cpp`
- `montercarlo_flip_edge.cpp` is not excluded.

Additional local build evidence was present in the existing build tree:

- compiled object file under `.../CMakeFiles/_core.dir/src/integrators/montercarlo_flip_edge.cpp.o`
- compile command entry for `montercarlo_flip_edge.cpp`

### Is it registered in `Evolver.add_integrator(...)`?

Yes.

In [`pymembrane/cppmodule/md/src/evolver/evolverclass.cpp`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/md/src/evolver/evolverclass.cpp):

- the file includes `../integrators/montercarlo_flip_edge.hpp`
- `EvolverClass::add_integrator(...)` contains:
  - `else if (name.compare("Mesh>MonteCarlo>edge>flip") == 0)`
  - construction via `std::make_shared<MonteCarloIntegratorFlipEdge>(_system, mesh_force_list)`

### Exact Python integrator string

The exact registered string is:

```python
"Mesh>MonteCarlo>edge>flip"
```

Strings with additional spaces or different capitalization were not registered in the source inspected here.

## Python API status

### Public Python surface confirmed

From the installed module:

- `Box`: present
- `System`: present
- `Evolver`: present
- `Evolver.add_integrator`: present
- `Evolver.evolveMC`: present

### `MeshOperations` availability

`MeshOperations` exists in C++ source, but I did not find it exported in the installed Python module inspected in this repository state.

Evidence:

- C++ class exists in [`pymembrane/cppmodule/md/src/mesh/meshoperations.hpp`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/md/src/mesh/meshoperations.hpp)
- The installed Python modules `pymembrane`, `pymembrane.cppmodule`, and `pymembrane.cppmodule.md` did not expose `MeshOperations` in `dir(...)`
- [`pymembrane/cppmodule/md/src/mesh/pybind_export_mesh.hpp`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/md/src/mesh/pybind_export_mesh.hpp) did not show a `py::class_<MeshOperations>` binding in the inspected content
- [`pymembrane/cppmodule/md/CMakeLists.txt`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/md/CMakeLists.txt) explicitly excludes `meshoperations.cpp` from the compiled source list

### Existing auxiliary benchmark script status

The auxiliary script [`pymembrane/cppmodule/benchmarks/benchmark_edge_flip.py`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/benchmarks/benchmark_edge_flip.py) does not match the current installed API state inspected here.

Observed mismatches:

- it expects `mb.MeshOperations(system)`, but `MeshOperations` was not exposed in the installed Python surface
- its `build_system(...)` function calls `system.read_mesh_from_files(..., False)`, but the current binding accepted only a single `files` dictionary argument in the direct probe

In a direct runtime check, `build_system(...)` failed with:

```text
TypeError: read_mesh_from_files(): incompatible function arguments
```

### Candidate-name probe results

Using a tiny generated icosphere mesh:

- `e.add_integrator("Mesh>MonteCarlo>edge>flip", {"every step": "1"})`
  - no exception
  - later `e.evolveMC(steps=1)` returned `{'Mesh>MonteCarlo>edge>flip': 0}`
  - this is the only candidate confirmed to register

- `e.add_integrator("Mesh > MonteCarlo > edge > flip", {"every step": "1"})`
  - no Python exception, but backend printed `integrator not found`
  - therefore not a valid registered name

- `e.add_integrator("Mesh>MonteCarlo>Edge>Flip", {"every step": "1"})`
  - no Python exception, but backend printed `integrator not found`
  - therefore not a valid registered name

Important API detail:

- invalid names do not raise an exception in this path
- backend behavior is to print `integrator not found`

## Implementation behavior

### Class and inheritance

- Class name: `MonteCarloIntegratorFlipEdge`
- Base class: `MonteCarloIntegrator`
- Constructor signature:

```cpp
MonteCarloIntegratorFlipEdge(SystemClass &system, VertexCompute &potentials)
```

### Accepted parameters

String-valued properties in `set_property(std::map<std::string, std::string>&)`:

- `T`
- `seed`
- `every step`

Typed map property in `set_property(std::map<std::string, std::map<std::string, std::string>>&)`:

- `freeze`

No other names are accepted in the inspected implementation.

### Random edge selection strategy

The integrator does not select a random edge per attempt.

In [`pymembrane/cppmodule/md/src/integrators/montercarlo_flip_edge.cpp`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/md/src/integrators/montercarlo_flip_edge.cpp), `integrate()` does:

```cpp
for (int edge_index = 0; edge_index < _system.Numedges; edge_index++)
```

So one MC sweep is a sequential scan over all edges. The RNG is used only for Metropolis acceptance.

### Validity checks

Two levels exist:

1. type-freeze check in the integrator:
   - `if (freezed_edge[_system.edges[edge_index].type] == false)`

2. topological/geometric validity inside the low-level flip helper:
   - `EdgeFlip_lambda(...)` is called
   - if the flip is not allowed, it returns `false`

The low-level helper in [`pymembrane/cppmodule/md/src/mesh/edge_flip.hpp`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/md/src/mesh/edge_flip.hpp) contains explicit checks for:

- boundary edges
- low local connectivity
- obtuse-triangle related conditions described in comments

### Energy-before/after strategy

The integrator uses local edge energy, not full-system energy.

Before attempting the flip:

```cpp
double delE = -this->ComputeEnergyFromEdge(edge_index);
```

After a successful trial flip:

```cpp
delE += this->ComputeEnergyFromEdge(edge_index);
```

`ComputeEnergyFromEdge(edge_index)` is defined in [`pymembrane/cppmodule/md/src/integrators/montecarlointegrator.hpp`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/md/src/integrators/montecarlointegrator.hpp) and sums:

```cpp
for (const auto& f : _potentials)
    edge_energy += f.second->compute_edge_energy(edge_index);
```

So the code evidence supports:

- local edge-based energy difference
- no call to the public whole-system `compute.energy(evolver)` path

### Metropolis acceptance rule

Yes.

After computing `delE`, the implementation uses:

```cpp
if (!(delE < 0.0))
{
    if (!(m_rng->drnd() < exp(-delE / get_temperature())))
```

This is a standard Metropolis-style accept/reject comparison as written.

### Rollback mechanism

Yes.

Rejected moves are rolled back by calling the same flip helper again:

```cpp
this->edge_flip(edge_index, true);
accepted_moves--;
```

So the code path for a rejected trial is:

1. compute local edge energy before
2. flip edge
3. compute local edge energy after
4. reject by Metropolis test
5. flip the same edge again to revert connectivity

### Counters and statistics

Exposed:

- accepted moves count only

Not found in the inspected implementation:

- attempted counter
- rejected counter
- invalid counter
- per-reason rejection statistics
- logging/stat reporting API

`integrate()` returns a single `int accepted_moves`.

`EvolverClass::evolve_mesh_mc(...)` aggregates those accepted counts in a `std::map<std::string, int>`.

### Printing/logging

The integrator itself does not print per-step statistics.

The low-level helper has `py::print(...)` calls in some invalidity cases inside [`pymembrane/cppmodule/md/src/mesh/edge_flip.hpp`](/Users/danielmatozfernandez/Documents/2025/PyMembraneReview/Original/ResubGithub/PyMembrane/ResubGithub/PyMembrane/pymembrane/cppmodule/md/src/mesh/edge_flip.hpp), but those are validity diagnostics, not MC counters.

### Low-level helper usage

Yes, the integrator delegates to the low-level flip helper through:

```cpp
pymemb::EdgeFlip_lambda(...)
```

via `MonteCarloIntegratorFlipEdge::edge_flip(...)`.

### Does it scan all edges?

Yes, once per `integrate()` call.

### Does it copy the full mesh?

No evidence of full-mesh copying was found in the inspected edge-flip integrator.

### Does it recompute total system energy?

No code evidence of total system energy recomputation was found in the integrator.

It computes edge-local energy via `compute_edge_energy(edge_index)` across the active force list.

### Does it rebuild connectivity globally?

No explicit global connectivity rebuild was visible in the inspected `MonteCarloIntegratorFlipEdge` path.

The flip is delegated to the local connectivity update helper `EdgeFlip_lambda(...)`.

## Potential performance concerns

These concerns are supported directly by the inspected code.

1. Sequential full-edge sweep per MC step

- `integrate()` loops from `0` to `_system.Numedges - 1`
- there is no random sparse attempt set
- every sweep touches all edges

2. Rejected moves perform two topology updates

- first flip to evaluate trial state
- second flip to roll back rejected state

3. Two local edge-energy evaluations per valid attempted flip

- one before trial
- one after trial

4. Potential diagnostic printing in invalid helper paths

- `py::print(...)` exists in the low-level helper for some invalid edge conditions
- if many invalid edges are encountered in real workloads, those prints could become expensive

5. Python invalid-name handling is print-based, not exception-based

- this is not a runtime hotspot for the integrator itself
- but it makes probing and test automation more error-prone because “no exception” does not mean “registered”

6. The lower-level helper path is not a current public Python surface

- `MeshOperations` exists in source but was not exposed in the installed module inspected here
- this means the practical public path today is the registered `Evolver.add_integrator("Mesh>MonteCarlo>edge>flip", ...)` interface, not `MeshOperations.flip_edge(...)`

What I did not find in the integrator code:

- full mesh copies
- explicit global recomputation through the public `compute.energy(...)` path
- explicit global rebuild of all topology data structures each move

## Runtime benchmark status

A tiny runtime probe was performed because the public edge-flip path is available through `Evolver.add_integrator(...)`.

Setup:

- generated subdivision-0 icosphere mesh
- `Box(4.0, 4.0, 4.0)`
- forces:
  - `Mesh>Harmonic`
  - `Mesh>Limit`
  - `Mesh>Bending>Dihedral`
- integrator:
  - `Mesh>MonteCarlo>edge>flip`

Observed result for `e.evolveMC(steps=10)`:

- wall time: about `0.000306708 s`
- returned accepted map:

```python
{'Mesh>MonteCarlo>edge>flip': 0}
```

Available runtime statistic:

- accepted count only

Unavailable runtime statistics in the inspected public API:

- attempts
- rejected count
- invalid count

No runtime benchmark was performed for `MeshOperations.flip_edge(...)` because no public `MeshOperations` path was available through the installed Python API inspected here.

## What was not changed

- No production source files were edited.
- No integrators were added or removed.
- No CMake files were changed.
- No examples were changed.
- No documentation was changed.
- No packaging was changed.
- No optimization work was done.

## Recommended next patch

Plan only. Not implemented here.

1. Add a focused smoke test that confirms:

```python
e.add_integrator("Mesh>MonteCarlo>edge>flip", {...})
```

really registers and that `e.evolveMC(steps=1)` returns a result entry for that integrator.

2. Add explicit tests for invalid-name behavior, since the current API prints `integrator not found` instead of raising.

3. Expose attempted, accepted, rejected, and invalid counters if operational diagnostics are needed.

4. If profiling later confirms a bottleneck, inspect whether the sequential full-edge sweep and double-flip rollback path dominate runtime on large meshes.

5. Reconcile or remove the stale auxiliary benchmark script if it is meant to be runnable, because it currently assumes a `MeshOperations` export and an older `read_mesh_from_files(..., False)` calling convention.

6. Only after the public API contract is locked down, add a dedicated benchmark around the registered `Mesh>MonteCarlo>edge>flip` path rather than the lower-level mesh primitive.
