/*!
 * \file montercarlo_vertex.hpp
 * \author Daniel Matoz , fdamatoz@gmail.com
 * \date 29-Dec-2018
 * \brief Declaration of IntegratorBrownianVertex class
 */

#ifndef __montercarlo_swap_edge_HPP__
#define __montercarlo_swap_edge_HPP__



#include <iostream>

#include "montecarlointegrator.hpp"
#include "../rng/rng.hpp"
#include "../types/globaltypes.hpp"
#include "../utils/fromstring.hpp"
#include "../system/systemclass.hpp"
/**
 *  @class MonteCarloIntegratorSwapEdge
 *  @brief Handles Metropolis type Monte Carlo algorithm.
 * 
 *  In this algorithm, one Monte Carlo step is considered as attempting a swap two edges at random.
 *  One sweep consists of attempting moves on each edge in the membrane.
 */
class MonteCarloIntegratorSwapEdge : public MonteCarloIntegrator
{
public:
  MonteCarloIntegratorSwapEdge(SystemClass &system, VertexCompute &potentials) : MonteCarloIntegrator(system, potentials)
  {
    m_name = "edge swap";
    m_type = "monte carlo";
    this->set_default_properties();
  }
  ~MonteCarloIntegratorSwapEdge() {}

  void set_default_properties(void) override
  {
    pymemb::vector<bool> _freezed_edge(NUM_TYPES_ALLOWED, false);
    freezed_edge = _freezed_edge;
    this->set_temperature(0.0);
    m_seed = 123456; ///default value
    m_rng = std::make_unique<RNG>(m_seed);
    stochastic_tunnelling = false;
    stochastic_gamma = 0.0;
  }

  using MonteCarloIntegrator::set_property;
  void set_property(std::map<std::string, std::string> &value_map)
  {
    for (const auto &item : value_map)
    {
      if (item.first.compare("T") == 0)
      {
        this->set_temperature(util::from_string_double(item.second));
        this->update_temperature_parameters();
      }
      else if (item.first.compare("gamma") == 0)
      {
        stochastic_tunnelling = true;
        stochastic_gamma = util::from_string_double(item.second);
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
      else if (item.first.compare("stochastic_tunnelling") == 0)
      {
        stochastic_tunnelling = util::from_string_bool(item.second);
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
  std::map<std::string, std::string> get_info(void) override
  {
    std::map<std::string, std::string> value;
    value["name"] = m_name;
    value["type"] = m_type;
    value["T"] = util::to_string(this->get_temperature());
    value["stochastic_tunnelling"] = util::to_string(stochastic_tunnelling);
    value["stochastic_gamma"] = util::to_string(stochastic_gamma);
    return value;
  }
  int integrate(void) override;

private:
  unsigned int m_seed;
  RNG_ptr m_rng;
  bool stochastic_tunnelling;
  double stochastic_gamma;
  pymemb::vector<bool> freezed_edge;
};

typedef std::shared_ptr<MonteCarloIntegratorSwapEdge> MonteCarloIntegratorSwapEdge_ptr;

#endif


