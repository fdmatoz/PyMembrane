#ifndef __pybind_export_compute_hpp__
#define __pybind_export_compute_hpp__

#include "../system/systemclass.hpp"
#include "../evolver/evolverclass.hpp"
#include "computemesh.hpp"

void export_ComputeMesh(py::module &m)
{
    py::class_<ComputeMesh>(m, "ComputeMesh")
        .def(py::init<SystemClass &>())
        .def("face_normals", &ComputeMesh::compute_face_normals, R"pbdoc(
            Compute face normals and store it in the face.normal
            )pbdoc")
        .def("vertex_normals", &ComputeMesh::compute_vertex_normals, py::arg("vertex_normal_angle_weight") = false, R"pbdoc(
            Compute vertex normals and store it in the vertex.normal
            )pbdoc")
        .def("edge_lengths", &ComputeMesh::compute_edge_lengths, R"pbdoc(
            Compute the edge lengths
            )pbdoc")
        .def("gaussian_curvature", &ComputeMesh::gaussiancurvature, R"pbdoc(
            Compute the gaussian curvature
            )pbdoc")
        .def("mean_curvature", &ComputeMesh::meancurvature, R"pbdoc(
            Compute the mean curvature
            )pbdoc")
        .def("curvature", &ComputeMesh::compute_mesh_curvature, R"pbdoc(
            Compute the mean and gaussian curvature
            )pbdoc")
        .def("volume", &ComputeMesh::compute_mesh_volume, R"pbdoc(
            Compute the mesh volume. Need close surfaces
            )pbdoc")
        .def("face_area", &ComputeMesh::compute_face_area, R"pbdoc(
            Compute the face area.
            )pbdoc")
        .def("area", &ComputeMesh::compute_mesh_area, R"pbdoc(
            Compute the mesh area.
            )pbdoc")
        .def("area_typed", &ComputeMesh::compute_area_typed, py::arg("type"), R"pbdoc(
                Compute the mesh area for a given type.
                )pbdoc")
        .def("face_metric", &ComputeMesh::compute_face_metric, R"pbdoc(
            Compute the face metric.
            )pbdoc")
        .def("vertex_area", &ComputeMesh::compute_vertex_area, R"pbdoc(
            Compute the face metric.
            )pbdoc")
        .def("energy", &ComputeMesh::compute_mesh_energy, py::arg("Evolver"), R"pbdoc(
            Compute the energy of the mesh. Needs EvolverClass. Returns the sum of the energy
            defined in the vertices, edges and faces. Is not normalized, nor by edge length, nor
            face area
            )pbdoc")
        .def("forces", &ComputeMesh::compute_vertex_forces, py::arg("Evolver"), R"pbdoc(
            Compute the forces on the mesh. Needs EvolverClass. Return a vector (x,y,z) with the forces
            in each vertex.
            )pbdoc")
        .def("stresses", &ComputeMesh::compute_stresses, py::arg("Evolver"), py::arg("IncludeVel") = false, R"pbdoc(
            Compute the forces on the mesh. Needs EvolverClass and a bool argument to include the velocities.
            Return a 9 components tensor.
            )pbdoc")
        .def("kinetic_energy_tensor", &ComputeMesh::compute_kinetic_energy_tensor, R"pbdoc(
            Compute the kinetic_ energy tensor on the mesh. Needs EvolverClass. Return a 9 components tensor.
            )pbdoc")
        .def("kinetic_energy", &ComputeMesh::compute_kinetic_energy, R"pbdoc(
            Compute the kinetic_ energy tensor on the mesh. Return a real number.
            )pbdoc")
        .def("temperature", &ComputeMesh::compute_temperature, R"pbdoc(
            Compute the temperature in the system. Return a real number.
            )pbdoc")
        .def("pressure", &ComputeMesh::compute_pressure, py::arg("Evolver"), R"pbdoc(
            Compute the temperature in the system. Return a real number.
            )pbdoc")
        .def("stresses_virial", &ComputeMesh::compute_stresses_virial, py::arg("Evolver"), py::arg("IncludeVel") = false, R"pbdoc(
            Compute the forces on the mesh. Needs EvolverClass and a bool argument to include the velocities.
            Return a 9 components tensor.
            )pbdoc")

        .def("stresses_atom", &ComputeMesh::compute_stresses_atom, py::arg("Evolver"), py::arg("IncludeVel") = false, R"pbdoc(
            Compute the forces on the mesh. Needs EvolverClass and a bool argument to include the velocities.
            Return a 9 components tensor of Numvertices size.
            )pbdoc")
            ;

}
#endif
