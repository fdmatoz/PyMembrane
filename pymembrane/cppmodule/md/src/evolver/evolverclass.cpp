#include <algorithm>
#include "evolverclass.hpp"

// operators over the meshes
#include "../mesh/meshoperators.hpp"

// here include all the hpp files of the mesh integrators
#include "../integrators/integrator_brownian_vertex.hpp"
#include "../integrators/integrator_verlet_vertex.hpp"
#include "../integrators/montecarlointegrator.hpp"
#include "../integrators/montercarlo_vertex.hpp"
#include "../integrators/montercarlo_swap_vertex.hpp"
#include "../integrators/montercarlo_swap_edge.hpp"
#include "../integrators/montercarlo_flip_edge.hpp"

// here we include all the Minimizers
#include "../minimizer/fire.hpp"

// contraints
#include "../constraints/constraintvolume.hpp"
#include "../constraints/constraintarea.hpp"

// compute
void EvolverClass::reset_mesh_forces(void)
{
    std::transform(_system.vertices.begin(), _system.vertices.end(), _system.vertices.begin(), pymemb::reset_vertex_forces());
}

// compute
void EvolverClass::reset_mesh_energy(void)
{
    std::transform(_system.vertices.begin(), _system.vertices.end(), _system.vertices.begin(), pymemb::reset_vertex_energy());
    std::transform(_system.edges.begin(), _system.edges.end(), _system.edges.begin(), pymemb::reset_edge_energy());
    std::transform(_system.faces.begin(), _system.faces.end(), _system.faces.begin(), pymemb::reset_face_energy());
}

void EvolverClass::compute_mesh_forces(void)
{
    for (const auto &force : mesh_force_list)
        force.second->compute();
}

void EvolverClass::compute_mesh_energy(void)
{
    for (const auto &force : mesh_force_list)
        force.second->compute_energy();
}

void EvolverClass::reset_mesh_stresses(void)
{
    if (has_face_forces == true)
        std::transform(_system.stress_group_faces.begin(), _system.stress_group_faces.end(), _system.stress_group_faces.begin(), pymemb::reset_tensor<realTensor>());
    if (has_vertex_forces == true)
        std::transform(_system.stress_group_vertices.begin(), _system.stress_group_vertices.end(), _system.stress_group_vertices.begin(), pymemb::reset_tensor<realTensor>());
    if (has_edge_forces == true)
        std::transform(_system.stress_group_edges.begin(), _system.stress_group_edges.end(), _system.stress_group_edges.begin(), pymemb::reset_tensor<realTensor>());
}

void EvolverClass::reset_mesh_atom_stresses(void)
{
    std::transform(_system.stress_virial_atom.begin(), _system.stress_virial_atom.end(), _system.stress_virial_atom.begin(), pymemb::reset_tensor<realTensor>());
}
void EvolverClass::compute_mesh_stresses(void)
{
    for (const auto &force : mesh_force_list)
        force.second->compute_stress();
}

void EvolverClass::compute_mesh_atom_stresses(void)
{
    for (const auto &force : mesh_force_list)
        force.second->compute_atomic_stress();
}

void EvolverClass::add_integrator(const std::string &name, std::map<std::string, std::string> &parameters)
{
    if (name.compare("Mesh>Brownian>vertex>move") == 0)
    {
        mesh_integrator_list[name] = std::make_shared<IntegratorBrownianMeshVertex>(_system);
        mesh_integrator_list[name]->set_property(parameters);
    }
    else if (name.compare("Mesh>VelocityVerlet>vertex>move") == 0)
    {
        mesh_integrator_list[name] = std::make_shared<IntegratorVelocityVerletMeshVertex>(_system);
        mesh_integrator_list[name]->set_property(parameters);
    }
    else if (name.compare("Mesh>MonteCarlo>vertex>move") == 0)
    {
        mesh_integrator_montecarlo_list[name] = std::make_shared<MonteCarloIntegratorVertex>(_system, mesh_force_list);
        mesh_integrator_montecarlo_list[name]->set_property(parameters);
    }

    else if (name.compare("Mesh>MonteCarlo>vertex>swap") == 0)
    {
        mesh_integrator_montecarlo_list[name] = std::make_shared<MonteCarloIntegratorSwapVertex>(_system, mesh_force_list);
        mesh_integrator_montecarlo_list[name]->set_property(parameters);
    }
    else if (name.compare("Mesh>MonteCarlo>edge>swap") == 0)
    {
        mesh_integrator_montecarlo_list[name] = std::make_shared<MonteCarloIntegratorSwapEdge>(_system, mesh_force_list);
        mesh_integrator_montecarlo_list[name]->set_property(parameters);
    }
    else if (name.compare("Mesh>MonteCarlo>edge>flip") == 0)
    {
        mesh_integrator_montecarlo_list[name] = std::make_shared<MonteCarloIntegratorFlipEdge>(_system, mesh_force_list);
        mesh_integrator_montecarlo_list[name]->set_property(parameters);
    }
    else
        py::print(name, " integrator not found");
}

void EvolverClass::delete_integrator(const std::string &name)
{
    auto it_mesh_integrator_list = mesh_integrator_list.find(name);
    if (it_mesh_integrator_list != mesh_integrator_list.end())
        mesh_integrator_list.erase(it_mesh_integrator_list);

    auto it_mesh_integrator_montecarlo_list = mesh_integrator_montecarlo_list.find(name);
    if (it_mesh_integrator_montecarlo_list != mesh_integrator_montecarlo_list.end())
        mesh_integrator_montecarlo_list.erase(it_mesh_integrator_montecarlo_list);
}

void EvolverClass::add_minimizer(const std::string &name, std::map<std::string, std::string> &parameters)
{
    if (name.compare("Mesh>Fire") == 0)
    {
        mesh_minimizer_list[name] = std::make_shared<MinimizerMeshFIRE>(_system, *this);
        mesh_minimizer_list[name]->set_property(parameters);
    }
    else
        py::print(name, " minimizer not found");
}

void EvolverClass::delete_minimizer(const std::string &name)
{
    auto it_mesh_minimizer_list = mesh_minimizer_list.find(name);
    if (it_mesh_minimizer_list != mesh_minimizer_list.end())
        mesh_minimizer_list.erase(it_mesh_minimizer_list);
}

void EvolverClass::minimize()
{
    for (auto minimizer : mesh_minimizer_list)
        minimizer.second->minimize();
}

void EvolverClass::set_time_step(const std::string &value)
{
    for (auto integrator : mesh_integrator_list)
        integrator.second->set_time_step(util::from_string_double(value));
}

void EvolverClass::set_global_temperature(const std::string &value)
{
    for (auto integrator : mesh_integrator_list)
        integrator.second->set_temperature(util::from_string_double(value));

    for (auto integrator : mesh_integrator_montecarlo_list)
        integrator.second->set_temperature(util::from_string_double(value));
}
void EvolverClass::evolve_mesh_md(const int &mdsteps)
{
    for (auto step = 0; step < mdsteps; step++)
    {
        // Perform the preintegration step, i.e., step before forces and torques are computed
        this->evolve_mesh_prestep();

        // Reset all forces and toques
        this->reset_mesh_forces();

        // Compute all forces and torques
        this->compute_mesh_forces();

        // Perform the second step of integration
        this->evolve_mesh_poststep();
    }
}

std::map<std::string, int> EvolverClass::evolve_mesh_mc(const int &mcs)
{
    std::map<std::string, int> accepted_moves;
    for (auto drv : mesh_integrator_montecarlo_list)
        accepted_moves[drv.first] = 0;

    for (auto step = 0; step < mcs; step++)
    {
        for (auto drv : mesh_integrator_montecarlo_list)
        {
            if ((step % (drv.second->get_integrate_every())) == 0)
                accepted_moves[drv.first] += drv.second->integrate();
        }
    }
    return accepted_moves;
}

void EvolverClass::evolve_mesh_prestep(void)
{
    for (auto integrator : mesh_integrator_list)
        integrator.second->prestep();
}

void EvolverClass::evolve_mesh_poststep(void)
{
    for (auto integrator : mesh_integrator_list)
        integrator.second->poststep();
}

void EvolverClass::evolve_mesh_prestep(const std::string &name)
{
    mesh_integrator_list[name]->prestep();
}

void EvolverClass::evolve_mesh_poststep(const std::string &name)
{
    mesh_integrator_list[name]->poststep();
}

void EvolverClass::add_constraints(const std::string &name, std::map<std::string, std::string> &parameters)
{
    if (name.compare("Mesh>Volume") == 0)
    {
        mesh_constraints_list[name] = std::make_shared<ConstraintVolume>(_system);
        for (const auto &item : parameters)
        {
            if (item.first.compare("max_iter") == 0)
                max_iter_mesh_contraint = util::from_string_int(item.second);
        }
        mesh_constraints_list[name]->set_property(parameters);
    }
    else if (name.compare("Mesh>Area") == 0)
    {
        mesh_constraints_list[name] = std::make_shared<ConstraintArea>(_system);
        for (const auto &item : parameters)
        {
            if (item.first.compare("max_iter") == 0)
                max_iter_mesh_contraint = util::from_string_int(item.second);
        }
        mesh_constraints_list[name]->set_property(parameters);
    }
}

void EvolverClass::delete_constraints(const std::string &name)
{
    auto it_mesh_constraints_list = mesh_constraints_list.find(name);
    if (it_mesh_constraints_list != mesh_constraints_list.end())
        mesh_constraints_list.erase(it_mesh_constraints_list);
}

void EvolverClass::set_max_iter_mesh_contraint(const std::string &value)
{
    max_iter_mesh_contraint = util::from_string_int(value);
}

void EvolverClass::enforce_mesh_constraints(void)
{
    bool satisfied;
    for (auto constraint : mesh_constraints_list)
        constraint.second->compute_ref_gradient();
    for (int iter = 0; iter < max_iter_mesh_contraint; iter++)
    {
        satisfied = true;
        for (auto constraint : mesh_constraints_list)
        {
            if (constraint.second->should_apply() == true)
            {
                constraint.second->compute_gradient();
                if (constraint.second->is_satisfied() == false)
                    satisfied = false;
            }
            else
                constraint.second->reset_LagrangeMultiplier();
        }
        if (satisfied)
            break;
        for (auto constraint : mesh_constraints_list)
        {
            if (constraint.second->should_apply() == true)
                constraint.second->enforce();
        }
    }
}
