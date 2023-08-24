/************************************************************************************
* MIT License                                                                       *
*                                                                                   *
* Copyright (c) 2023 Dr. Daniel Alejandro Matoz Fernandez                           *
*               fdamatoz@gmail.com                                                  *
* Permission is hereby granted, free of charge, to any person obtaining a copy      *
* of this software and associated documentation files (the "Software"), to deal     *
* in the Software without restriction, including without limitation the rights      *
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell         *
* copies of the Software, and to permit persons to whom the Software is             *
* furnished to do so, subject to the following conditions:                          *
*                                                                                   *
* The above copyright notice and this permission notice shall be included in all    *
* copies or substantial portions of the Software.                                   *
*                                                                                   *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR        *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,          *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE       *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER            *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,     *
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     *
* SOFTWARE.                                                                         *
*************************************************************************************/
#ifndef __pybind_export_system_hpp__
#define __pybind_export_system_hpp__

#include "systemclass.hpp"

void export_SystemClass(pybind11::module &m)
{
    pybind11::class_<SystemClass>(m, "System")
        .def(pybind11::init<>())
        .def(pybind11::init<const BoxType &>())
        .def("read_mesh_from_files", &SystemClass::read_mesh_from_files, pybind11::arg("files"), "read the mesh from a file, needs file route <faces, vertices>")
        .def("read_mesh_from_json", &SystemClass::read_mesh_from_json, pybind11::arg("file"), "read the mesh from a json file lists")
        .def_readonly("mesh_is_close", &SystemClass::close_surface, "return true if is close false otherwise")
        

        /*.def("getVertices", &SystemClass::get_vertices, "get the vertices in memb format")
        .def("getEdges", &SystemClass::get_edges, "get the edges in memb format")
        .def("getFaces", &SystemClass::get_faces, "get the faces in memb format")
        .def("getHalfedges", &SystemClass::get_halfedges, "return the halfedges in memb format")
        .def("setVertices", &SystemClass::set_vertices, "set the vertices, needs vertices in memb format")
        .def("setEdges", &SystemClass::set_edges, "set the edges, needs edges in memb format")
        .def("setFaces", &SystemClass::set_faces, "set the faces, needs faces in memb format")
        .def("setHalfedges", &SystemClass::set_halfedges, "set the halfedges, needs halfedges in memb format")
        .def("setMesh", &SystemClass::set, "set the vertices, faces, halfedges and edges, needs vertices, faces, halfedges and edges in memb format")
        .def("compute_mesh", &SystemClass::get_compute_mesh)
        .def("dump", &SystemClass::get_dumper)
        .def_readonly("Numvertices", &SystemClass::Numvertices)
        .def_readonly("Numedges", &SystemClass::Numedges)
        .def_readonly("Numhalfedges", &SystemClass::Numhalfedges)
        .def("get_mesh_info", &SystemClass::get_mesh_info)
        .def("getStressVertices", &SystemClass::get_stress_vertices)
        .def("getStressEdges", &SystemClass::get_stress_edges)
        .def("getStressFaces", &SystemClass::get_stress_faces)
        .def("getStressVirial", &SystemClass::get_stress_virial)
        .def("getStressKinetic", &SystemClass::get_stress_kinetic)*/

        .def_readonly("Numfaces", &SystemClass::Numfaces)
        .def_readonly("Numedges", &SystemClass::Numedges)
        .def_readonly("Numvertices", &SystemClass::Numvertices)
        .def_readonly("Numedges", &SystemClass::Numedges)
        .def_readonly("Numhalfedges", &SystemClass::Numhalfedges)
        .def("get_mesh_info", &SystemClass::get_mesh_info)
        
        .def_property("stress_vertices", &SystemClass::get_stress_vertices, [] (SystemClass& self,pymemb::vector<realTensor> & _stress ){})
        .def_property("stress_edges", &SystemClass::get_stress_edges, [] (SystemClass& self,pymemb::vector<realTensor> & _stress ){})
        .def_property("stress_faces", &SystemClass::get_stress_faces, [] (SystemClass& self,pymemb::vector<realTensor> & _stress ){})
        .def_property("stress_virial", &SystemClass::get_stress_virial, [] (SystemClass& self,pymemb::vector<realTensor> & _stress ){})
        .def_property("stress_kinetic", &SystemClass::get_stress_kinetic, [] (SystemClass& self,pymemb::vector<realTensor> & _stress ){})
        
        .def_property("dumper", &SystemClass::get_dumper, [] (SystemClass& self,DumperClass & _dumper ){})
        .def_property("compute", &SystemClass::get_compute_mesh,  [] (SystemClass& self,ComputeMesh & _computer ){})
        .def_property("vertices", &SystemClass::get_vertices, &SystemClass::set_vertices)
        .def_property("edges", &SystemClass::get_edges, &SystemClass::set_edges)
        .def_property("faces", &SystemClass::get_faces, &SystemClass::set_vertices)
        .def_property("halfedges", &SystemClass::get_halfedges, &SystemClass::set_halfedges)


        .def("vertices_to_numpy", [](SystemClass &self) {
            std::vector<std::vector<double>> vertices;
            std::vector<std::vector<int>> faces;

            for (auto &v : self.vertices)
            {
                vertices.push_back({v.r.x, v.r.y, v.r.z});
            }
            return vertices;
        }, "return the mesh vertices in numpy format")

        .def("faces_to_numpy", [](SystemClass &self) {
            std::vector<std::vector<int>> faces;
            for (auto &f : self.faces)
            {
                faces.push_back({f.v1, f.v2, f.v3});
            }
            return faces;
        }, "return the mesh faces in numpy format")

        //Box
        .def_property("box",&SystemClass::get_box, &SystemClass::set_box)
        .def("minImage", &SystemClass::minImage)
        .def("need_wrapping", &SystemClass::need_wrapping)
        .def("enforce_boundaries", &SystemClass::enforce_periodic_boundary_conditions)
        ;
}

#endif
