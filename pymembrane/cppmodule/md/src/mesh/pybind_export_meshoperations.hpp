#ifndef __pybind_export_meshoperations_hpp__
#define __pybind_export_meshoperations_hpp__

#include "meshoperations.hpp"


void export_MeshOperations(py::module &m)
{
    py::class_<std::vector<MeshOperations>>(m, "MeshOperations")
        .def(py::init<SystemClass &>())
        .def("check_if_edge_can_flip", MeshOperations::check_if_edge_can_flip, py::arg("edge"))
        .def("edge_flip", MeshOperations::edge_flip, py::arg("edge"))
        .def("equiangulation", MeshOperations::equiangulation);
}
#endif