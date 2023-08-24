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
#include <chrono>
#include <sstream>
#include <iomanip>

#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <pybind11/iostream.h>
#include <pybind11/functional.h>

namespace py = pybind11;

//types
#include "types/pybind_export_types.hpp"

//box
#include "box/pybind_export_box.hpp"

//system
#include "system/pybind_export_system.hpp"

//compute
#include "evolver/pybind_export_evolver.hpp"

//meshes
#include "mesh/pybind_export_mesh.hpp"

//compute
#include "compute/pybind_export_compute.hpp"

//dumper
#include "dumper/pybind_export_dumper.hpp"

/*
Utils functions for the module
*/
std::string getCurrentDate() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d");
    return ss.str();
}

// Function to get the current operating system
std::string getOperatingSystem() {
    #if defined(_WIN32) || defined(_WIN64)
        return "Windows";
    #elif defined(__APPLE__) || defined(__MACH__)
        return "macOS";
    #elif defined(__linux__)
        return "Linux";
    #else
        return "Unknown";
    #endif
}

/*end*/

PYBIND11_MODULE(cppmodule, m)
{
    // Module documentation
    m.doc() = R"pbdoc(
        PyMembrane Engine
        -----------------------
        .. module:: cppmodule
        .. synopsis:: A cutting-edge membrane elasticity simulation engine.
        
        PyMembrane Engine is designed to provide a suite of advanced tools 
        for membrane elasticity simulations. The module encompasses various 
        functionalities tailored for specific simulation scenarios, ranging 
        from stretching and bending potentials to advanced integrative methods.

        .. author:: Dr. D A Matoz-Fernandez
        .. note:: Copyright 2023, D A Matoz-Fernandez. All rights reserved.
        .. version:: 1.0a
        .. autosummary::
           :toctree: _generate

        .. seealso::
           For detailed information, tutorials, and API documentation, 
           please refer to the official PyMembrane documentation.
    )pbdoc";

    // Module version
    m.attr("__author__") = "Daniel Matoz-Fernandez";
    m.attr("__repository_url__") = "https://github.com/username/pymembrane";
    m.attr("__version__") = "1.0";  // version
    m.attr("__release_date__") = "August 2023";  // release date
    //Compilation date
    m.attr("__compiled_date__") = getCurrentDate();
    // Operating system
    m.attr("__OS__") = getOperatingSystem().c_str();

    ///redirect std::cout and std::cerr
    add_ostream_redirect(m, "ostream_redirect");
    export_real3(m);
    export_inth3(m);
    export_bool3(m);
    export_realTensor(m);
    PYBIND11_NUMPY_DTYPE(real3, x, y);
    PYBIND11_NUMPY_DTYPE(inth3, x, y);
    PYBIND11_NUMPY_DTYPE(bool3, x, y);
    PYBIND11_NUMPY_DTYPE(realTensor, xx, xy, xz, yx, yy, yz, zx, zy, zz);
    //md
    export_BoxType(m);
    export_SystemClass(m);
    export_EvolverClass(m);


    //mesh
    export_PropertyFaces_Vector(m);
    export_PropertyVertices(m);
    export_PropertyVertices_Vector(m);
    export_PropertyEdges(m);
    export_PropertyEdges_Vector(m);
    export_HE_Vertex_Vector(m);
    export_HE_Vertex(m);
    export_HE_Edge_Vector(m);
    export_HE_Edge(m);
    export_HE_Face_Vector(m);
    export_HE_Face(m);
    export_HE_HalfEdge_Vector(m);
    export_HE_HalfEdge(m);

    //compute 
    export_ComputeMesh(m);

    //dumper
    export_DumperClass(m);
    
}
