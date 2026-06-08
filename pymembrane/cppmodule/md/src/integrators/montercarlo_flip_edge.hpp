/*!
 * \file montercarlo_vertex.hpp
 * \author Daniel Matoz , fdamatoz@gmail.com
 * \date 29-Dec-2018
 * \brief Declaration of IntegratorBrownianVertex class
 */

#ifndef __montercarlo_flip_edge_HPP__
#define __montercarlo_flip_edge_HPP__



#include <iostream>

#include "montecarlointegrator.hpp"
#include "../rng/rng.hpp"
#include "../types/globaltypes.hpp"
#include "../utils/fromstring.hpp"
#include "../system/systemclass.hpp"
/**
 *  @class MonteCarloIntegratorFlipEdge
 *  @brief Handles Metropolis type Monte Carlo algorithm.
 * 
 *  In this algorithm, one Monte Carlo step is considered as attempting flip an edge.
 *  One sweep consists of attempting moves on each edge in the membrane.
 */
class MonteCarloIntegratorFlipEdge : public MonteCarloIntegrator
{
public:
    MonteCarloIntegratorFlipEdge(SystemClass &system, VertexCompute &potentials) : MonteCarloIntegrator(system, potentials)
    {
        name = "edge flip";
        type = "monte carlo";
        this->set_default_properties();
    }
    ~MonteCarloIntegratorFlipEdge() {}
    void set_default_properties(void) override
    {
        pymemb::vector<bool> _freezed_edge(NUM_TYPES_ALLOWED, false);
        freezed_edge = _freezed_edge;
        this->set_temperature(0.0);
        m_seed = 123456; ///default value
        m_rng = std::make_unique<RNG>(m_seed);
    }

    using MonteCarloIntegrator::set_property;
    void set_property(std::map<std::string, std::string> &value_map) override
    {
        for (const auto &item : value_map)
        {
            if (item.first.compare("T") == 0)
            {
                this->set_temperature(util::from_string_double(item.second));
                this->update_temperature_parameters();
            }
            else if (item.first.compare("seed") == 0)
            {
                m_seed = uint(util::from_string_int(item.second));
                m_rng = std::make_unique<RNG>(m_seed);
            }
            else if (item.first.compare("every step") == 0)
            {
                this->set_integrate_every(util::from_string_int(item.second));
            }
            else
                print_warning_property_name(item.first);
        }
    }

    void set_property(std::map<std::string, std::map<std::string, std::string>> &value_map) override
    {
        for (const auto &item : value_map)
        {
        if (item.first.compare("freeze") == 0)
        {
            pymemb::vector<bool> _freezed_edge = util::from_dict_to_vector_types(pymemb::copy(freezed_edge), item.second);
            freezed_edge = _freezed_edge;
        }
        else
            this->print_warning_property_name(item.first);
        }
    }

    bool edge_flip(int, bool);
    int integrate(void) override;

private:
    double m_dx, m_dy, m_dz;
    unsigned int m_seed;
    RNG_ptr m_rng;
    pymemb::vector<bool> freezed_edge;
};

typedef std::shared_ptr<MonteCarloIntegratorFlipEdge> MonteCarloIntegratorFlipEdge_ptr;

#endif


