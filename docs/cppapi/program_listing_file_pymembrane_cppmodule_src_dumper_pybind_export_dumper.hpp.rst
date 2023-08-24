
.. _program_listing_file_pymembrane_cppmodule_src_dumper_pybind_export_dumper.hpp:

Program Listing for File pybind_export_dumper.hpp
=================================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_dumper_pybind_export_dumper.hpp>` (``pymembrane/cppmodule/src/dumper/pybind_export_dumper.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef __pybind_export_dumper_hpp__
   #define __pybind_export_dumper_hpp__
   
   #include "dumper.hpp"
   
   void export_DumperClass(py::module &m)
   {
       py::class_<DumperClass>(m, "Dumper")
           .def(py::init<SystemClass &>())
           .def("vtk", &DumperClass::mesh_vtk, py::arg("filename"), py::arg("periodic")=false, "dump the mesh (vertices and faces) in vtk format")
           .def("edge_vtk", &DumperClass::mesh_edge_vtk, "dump the mesh (edges) in vtk format", py::arg("filename"))
           .def("json", &DumperClass::mesh_json, "dump the mesh (vertices and faces) in json format", py::arg("filename"), py::arg("faces")=true, py::arg("vertices")=true)
           .def("ply", &DumperClass::mesh_ply, "dump the mesh (vertices and faces) in ply format", py::arg("filename"))
           .def("txt", &DumperClass::mesh_txt, "dump the mesh (vertices and faces) in text format", py::arg("filename"), py::arg("faces")=true, py::arg("vertices")=true)
           .def("setvtkLegacyFormat", &DumperClass::setvtkLegacyFormat, "set the vtk legacy format flag")
           
           .def("user_data_vertex", (void (DumperClass::*)(const std::string&, std::vector<real>&)) & DumperClass::user_vertex_data, "dump user data on vertices")
           .def("user_data_vertex", (void (DumperClass::*)(const std::string&, std::vector<realTensor>&)) & DumperClass::user_vertex_data, "dump user data on vertices")
   
           .def("user_data_face", (void (DumperClass::*)(const std::string&, std::vector<real>&)) & DumperClass::user_face_data, "dump user data on faces")
           .def("user_data_face", (void (DumperClass::*)(const std::string&, std::vector<realTensor>&)) & DumperClass::user_face_data, "dump user data on faces")
           
           .def("user_data_edge", (void (DumperClass::*)(const std::string&, std::vector<real>&)) & DumperClass::user_edge_data, "dump user data on edges")
           .def("user_data_edge", (void (DumperClass::*)(const std::string&, std::vector<realTensor>&)) & DumperClass::user_edge_data, "dump user data on edges")
           ;
   }
   
   #endif
