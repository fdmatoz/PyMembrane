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

#ifndef __pybind_export_evolver_hpp__
#define __pybind_export_evolver_hpp__

#include "evolverclass.hpp"

void export_EvolverClass(py::module &m)
{
    py::class_<EvolverClass>(m, "Evolver")
        .def(py::init<SystemClass &>())
        //forces
        .def("add_force", &EvolverClass::add_force<std::map<std::string, std::map<std::string, std::string>>>, "add force method")
        .def("add_force", &EvolverClass::add_force<std::map<std::string, std::map<std::pair<std::string, std::string>, std::string>>>, "add force method")
        .def("delete_force", &EvolverClass::delete_force, "delete force method")
        .def("reset_mesh_forces", &EvolverClass::reset_mesh_forces, "reset the mesh forces")
        .def("compute_mesh_forces", &EvolverClass::compute_mesh_forces, "compute the mesh forces")
        .def("reset_mesh_energy", &EvolverClass::reset_mesh_energy, "reset the mesh energy")
        .def("compute_mesh_energy", &EvolverClass::compute_mesh_energy, "compute the mesh energy")
        .def("reset_mesh_stresses", &EvolverClass::reset_mesh_stresses, "reset the mesh energy")
        .def("compute_mesh_stresses", &EvolverClass::compute_mesh_stresses, "compute the mesh energy")
        .def("reset_mesh_atom_stresses", &EvolverClass::reset_mesh_atom_stresses, "reset the mesh energy")
        .def("compute_mesh_atom_stresses", &EvolverClass::compute_mesh_atom_stresses, "compute the mesh energy")
        //constraints
        .def("add_constraint", &EvolverClass::add_constraints, "add contraint method")
        .def("delete_constraints", &EvolverClass::delete_constraints, "delete contraint method")
        .def("enforce_constraints", &EvolverClass::enforce_mesh_constraints, "enforce contraint method")
        //minimizer
        .def("add_minimizer", &EvolverClass::add_minimizer, "add minimizer method")
        .def("delete_minimizer", &EvolverClass::delete_minimizer, "delete minimizer method")
        .def("minimize", &EvolverClass::minimize, "minimize the mesh")
        //evolve & integrators
        .def("add_integrator", &EvolverClass::add_integrator, "add integrator method")
        .def("delete_integrator", &EvolverClass::delete_integrator, "delete integrator method")
        .def("set_time_step", &EvolverClass::set_time_step, "set the time step for Molecular Dynamics methods")
        .def("set_global_temperature", &EvolverClass::set_global_temperature, "set the temperature in the whole system")
        .def("evolveMD", &EvolverClass::evolve_mesh_md, "evolve the mesh following Monte Carlos methods loaded", py::arg("steps")=1)
        .def("evolveMC", &EvolverClass::evolve_mesh_mc, "evolve the mesh following Molecular Dynamics methods loaded", py::arg("steps")=1)
        .def("evolve_mesh_prestep", (void (EvolverClass::*)(void)) & EvolverClass::evolve_mesh_prestep)
        .def("evolve_mesh_prestep", (void (EvolverClass::*)(const std::string &)) & EvolverClass::evolve_mesh_prestep)
        .def("evolve_mesh_poststep", (void (EvolverClass::*)(void)) & EvolverClass::evolve_mesh_poststep)
        .def("evolve_mesh_poststep", (void (EvolverClass::*)(const std::string &)) & EvolverClass::evolve_mesh_poststep)
         //properties
        .def("set", &EvolverClass::set_property<std::map<std::string, std::map<std::pair<std::string, std::string>, std::string>>>, "set/modify properties for integrartors/forces/minimizers/contraints")
        .def("set", &EvolverClass::set_property<std::map<std::string, std::map<std::string, std::string>>>, "set/modify properties for integrartors/forces/minimizers/contraints")
        .def("set", &EvolverClass::set_property<std::map<std::string, std::string>>, "set/modify properties for integrartors/forces/minimizers/contraints")
        //common
        .def("get_force_info", &EvolverClass::get_force_info, "get information about potentials and integrators")
        .def("get_integrator_info", &EvolverClass::get_integrator_info, "get information about integrators and integrators")
        .def("get_minimizer_info", &EvolverClass::get_minimizer_info, "get information about minimizer")
        ;
}
#endif
