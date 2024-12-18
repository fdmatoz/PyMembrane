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
#ifndef __evolverclass_hpp__
#define __evolverclass_hpp__

#include <memory>
#include <map>
#include <iostream>

#include <pybind11/pybind11.h>
namespace py = pybind11;

#include "../system/systemclass.hpp"
#include "../potentials/computeforceclass.hpp"
#include "../integrators/integratorclass.hpp"
#include "../integrators/montecarlointegrator.hpp"
#include "../integrators/montecarlointegrator.hpp"
#include "../minimizer/minimizerclass.hpp"
#include "../constraints/constraintclass.hpp"

//here mesh potentials
#include "../potentials/potentialBending.hpp"
#include "../potentials/potentialBendingHelfrich.hpp"
#include "../potentials/potentialDihedralHarmonics.hpp"
#include "../potentials/potentialCauchyGreen.hpp"
#include "../potentials/potentialHarmonic.hpp"
#include "../potentials/potentialLimit.hpp"
#include "../potentials/potentialLineTension.hpp"
#include "../potentials/potentialConstantAreaTriangle.hpp"
#include "../potentials/potentialSubstrate.hpp"
#include "../potentials/potentialIsing.hpp"
#include "../potentials/potentialHarmonicDye.hpp"

class EvolverClass
{
public:
    EvolverClass(SystemClass &system) : _system(system)
    {
    }
    ~EvolverClass() {}

    template <typename T>
    void add_force(const std::string &name, T &parameters)
    {
        if (name.compare("Mesh>Cauchy-Green") == 0)
        {
            mesh_force_list[name] = std::make_unique<ComputeVertexCauchyGreenEnergy>(_system);
            mesh_force_list[name]->set_property(parameters);
        }
        else if (name.compare("Mesh>Harmonic") == 0)
        {
            mesh_force_list[name] = std::make_unique<ComputeVertexHarmonicEnergy>(_system);
            mesh_force_list[name]->set_property(parameters);
        }
        else if (name.compare("Mesh>Limit") == 0)
        {
            mesh_force_list[name] = std::make_unique<ComputeVertexLimitEnergy>(_system);
            mesh_force_list[name]->set_property(parameters);
        }
        else if (name.compare("Mesh>LineTension") == 0)
        {
            mesh_force_list[name] = std::make_unique<ComputeVertexLineTension>(_system);
            mesh_force_list[name]->set_property(parameters);
        }
        else if (name.compare("Mesh>Constant Area") == 0)
        {
            mesh_force_list[name] = std::make_unique<ComputeVertexConstantAreaTriangleEnergy>(_system);
            mesh_force_list[name]->set_property(parameters);
        }
        else if (name.compare("Mesh>Bending") == 0)
        {
            mesh_force_list[name] = std::make_unique<ComputeVertexBendingEnergy>(_system);
            mesh_force_list[name]->set_property(parameters);
        }
        else if (name.compare("Mesh>Bending>Helfrich") == 0)
        {
            mesh_force_list[name] = std::make_unique<ComputeVertexBendingHelfrichEnergy>(_system);
            mesh_force_list[name]->set_property(parameters);
        }
        else if (name.compare("Mesh>Bending>Dihedral") == 0)
        {
            mesh_force_list[name] = std::make_unique<ComputeVertexDihedralEnergy>(_system);
            mesh_force_list[name]->set_property(parameters);
        }
        else if (name.compare("Mesh>Substrate") == 0)
        {
            mesh_force_list[name] = std::make_unique<ComputeVertexSubstrateEnergy>(_system);
            mesh_force_list[name]->set_property(parameters);
        }
        else if (name.compare("Mesh>Ising") == 0)
        {
            mesh_force_list[name] = std::make_unique<ComputeVertexFerromagneticEnergy>(_system);
            mesh_force_list[name]->set_property(parameters);
        }
        else if (name.compare("Mesh>HarmonicDye") == 0)
        {
            mesh_force_list[name] = std::make_unique<ComputeVertexHarmonicSpinEnergy>(_system);
            mesh_force_list[name]->set_property(parameters);
        }
        else
            py::print(name, " potential not found");

        //update the force type
        for (const auto &force : mesh_force_list)
        {
            if ((force.second->get_type()).compare("vertex") == 0)
                has_vertex_forces = true;
            else
                has_vertex_forces = false;
            if ((force.second->get_type()).compare("edge") == 0)
                has_edge_forces = true;
            else
                has_edge_forces = false;
            if ((force.second->get_type()).compare("face") == 0)
                has_face_forces = true;
            else
                has_face_forces = false;
        }
    }

    void delete_force(const std::string &name)
    {
        auto it_mesh_force_list = mesh_force_list.find(name);
        if (it_mesh_force_list != mesh_force_list.end())
            mesh_force_list.erase(it_mesh_force_list);

        //update the force type
        for (const auto &force : mesh_force_list)
        {
            if ((force.second->get_type()).compare("vertex") == 0)
                has_vertex_forces = true;
            else
                has_vertex_forces = false;
            if ((force.second->get_type()).compare("edge") == 0)
                has_edge_forces = true;
            else
                has_edge_forces = false;
            if ((force.second->get_type()).compare("face") == 0)
                has_face_forces = true;
            else
                has_face_forces = false;
        }
    }

    template <typename T>
    void set_property(const std::string &name, T &parameters)
    {
        auto it_mesh_force_list = mesh_force_list.find(name);
        if (it_mesh_force_list != mesh_force_list.end())
        {
            mesh_force_list[name]->set_property(parameters);
        }

        auto it_mesh_integrator_list = mesh_integrator_list.find(name);
        if (it_mesh_integrator_list != mesh_integrator_list.end())
        {
            mesh_integrator_list[name]->set_property(parameters);
        }

        auto it_mesh_integrator_montecarlo_list = mesh_integrator_montecarlo_list.find(name);
        if (it_mesh_integrator_montecarlo_list != mesh_integrator_montecarlo_list.end())
        {
            mesh_integrator_montecarlo_list[name]->set_property(parameters);
        }

        auto it_mesh_minimizer_list = mesh_minimizer_list.find(name);
        if (it_mesh_minimizer_list != mesh_minimizer_list.end())
        {
            mesh_minimizer_list[name]->set_property(parameters);
        }

        auto it_mesh_constraints_list = mesh_constraints_list.find(name);
        if (it_mesh_constraints_list != mesh_constraints_list.end())
        {
            mesh_constraints_list[name]->set_property(parameters);
        }
    }

    std::vector<std::map<std::string, std::string>> get_force_info(void)
    {
        std::vector<std::map<std::string, std::string>> forces_info;
        for (const auto &force : mesh_force_list)
            forces_info.push_back(force.second->get_info());

        return forces_info;
    }
    std::map<std::string, std::map<std::string, std::string>> get_minimizer_info(void)
    {
        std::map<std::string, std::map<std::string, std::string>> minimizers_info;
        for (const auto &minimizer : mesh_minimizer_list)
            minimizers_info[minimizer.first] = minimizer.second->get_info();

        return minimizers_info;
    }

    std::map<std::string, std::map<std::string, std::string>> get_integrator_info(void)
    {
        std::map<std::string, std::map<std::string, std::string>> integrators_info;
        for (const auto &integrator : mesh_integrator_list)
           integrators_info[integrator.first] = integrator.second->get_info();

        for (const auto &integrator : mesh_integrator_montecarlo_list)
           integrators_info[integrator.first] = integrator.second->get_info();

        return integrators_info;
    }

    //Potentials
    void reset_mesh_forces(void);
    void compute_mesh_forces(void);
    void reset_mesh_energy(void);
    void compute_mesh_energy(void);
    //Integrators
    void add_integrator(const std::string &, std::map<std::string, std::string> &);
    void delete_integrator(const std::string &);
    void set_time_step(const std::string &);
    void set_global_temperature(const std::string &);
    //Minimizers
    void add_minimizer(const std::string &, std::map<std::string, std::string> &);
    void delete_minimizer(const std::string &);
    void minimize(void);
    //Constraints
    void add_constraints(const std::string &, std::map<std::string, std::string> &);
    void enforce_mesh_constraints(void);
    void delete_constraints(const std::string &);
    void set_max_iter_mesh_contraint(const std::string &);
    int max_iter_mesh_contraint = 100;
    //stress
    void reset_mesh_stresses(void);
    void compute_mesh_stresses(void);
    void reset_mesh_atom_stresses(void);
    void compute_mesh_atom_stresses(void);
    //Evolve
    void evolve_mesh_md(const int &);
    std::map<std::string, int> evolve_mesh_mc(const int &);
    void evolve_mesh_prestep(const std::string &);
    void evolve_mesh_poststep(const std::string &);
    void evolve_mesh_prestep(void);
    void evolve_mesh_poststep(void);


    //Auxiliary functions for speed-up
    bool has_vertex_forces;
    bool has_face_forces;
    bool has_edge_forces;
    //private:
    SystemClass &_system;                                                            //!< reference to system class where the box and particles are stored
    std::map<std::string, ComputeForceClass_ptr> mesh_force_list;                    //!< list of all the pointer to the forces
    std::map<std::string, IntegratorClass_ptr> mesh_integrator_list;                 //!< list of all the pointer to the md integrators
    std::map<std::string, MonteCarloIntegrator_ptr> mesh_integrator_montecarlo_list; //!< list of all the pointer to the mc intergators
    std::map<std::string, ConstraintClass_ptr> mesh_constraints_list;                //!< list of all the pointer to the constrainnts
    std::map<std::string, MinimizerClass_ptr> mesh_minimizer_list;                   //!< list of all the pointer to the constrainnts
};

#endif


