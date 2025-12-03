/*!
 * \file montercarlo_vertex.hpp
 * \author Daniel Matoz , fdamatoz@gmail.com
 * \date 29-Dec-2018
 * \brief Declaration of IntegratorBrownianVertex class
 */

#ifndef __montercarlo_vertex_HPP__
#define __montercarlo_vertex_HPP__


#include <iostream>
#include "montecarlointegrator.hpp"
#include "../rng/rng.hpp"
#include "../types/globaltypes.hpp"
#include "../utils/fromstring.hpp"
#include "../system/systemclass.hpp"
/**
 *  @class MonteCarloIntegratorVertex
 *  @brief Handles Metropolis type Monte Carlo algorithm.
 * 
 *  In this algorithm, one Monte Carlo step is considered as attempting a move of a single vertex.
 *  One sweep consists of attempting moves on each vertex in the membrane.
 */
class MonteCarloIntegratorVertex : public MonteCarloIntegrator
{
public:
  /** @brief VertexIntegrator Constructor */
  MonteCarloIntegratorVertex(SystemClass &system, VertexCompute &potentials) : MonteCarloIntegrator(system, potentials)
  {
    m_type = "monte carlo";
    m_name = "vertex move";
    this->set_default_properties();
  }
  /** @brief destructor */
  ~MonteCarloIntegratorVertex() {}

  void set_default_properties(void) override
  {
    pymemb::vector<bool> _freezed_vertex(NUM_TYPES_ALLOWED, false);
    m_freezed_vertex = _freezed_vertex;
    this->set_temperature(0.0);
    m_seed = 123456; ///default value
    m_rng = std::make_unique<RNG>(m_seed);
    m_spherical_move = false;
    m_dx = 1e-2;
    m_dy = 1e-2;
    m_dz = 1e-2;
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
      else if (item.first.compare("dr") == 0)
      {
        m_dx = util::from_string_double(item.second);
        m_dy = util::from_string_double(item.second);
        m_dz = util::from_string_double(item.second);
      }
      else if (item.first.compare("drx") == 0)
      {
        m_dx = util::from_string_double(item.second);
      }
      else if (item.first.compare("dry") == 0)
      {
        m_dy = util::from_string_double(item.second);
      }
      else if (item.first.compare("drz") == 0)
      {
        m_dz = util::from_string_double(item.second);
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
      else if (item.first.compare("spherical_move") == 0)
      {
        m_spherical_move = util::from_string_bool(item.second);
      }
      else
        this->print_warning_property_name(item.first);
    }
  }

  void set_property(std::map<std::string, std::map<std::string, std::string>> &value_map) override
  {
    for (const auto &item : value_map)
    {
      if (item.first.compare("freeze") == 0)
      {
        pymemb::vector<bool> _freezed_vertex = util::from_dict_to_vector_types(pymemb::copy(m_freezed_vertex), item.second);
        m_freezed_vertex = _freezed_vertex;
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
    value["spherical_move"] = util::to_string(m_spherical_move);
    value["dx"] = util::to_string(m_dx);
    value["dy"] = util::to_string(m_dy);
    value["dz"] = util::to_string(m_dz);
    return value;
  }
  int integrate(void) override;

private:
  double m_dx, m_dy, m_dz;
  unsigned int m_seed; //!< random number seed;
  RNG_ptr m_rng;       //!< Random number generator
  bool m_spherical_move;
  pymemb::vector<bool> m_freezed_vertex;
};

typedef std::shared_ptr<MonteCarloIntegratorVertex> MonteCarloIntegratorVertex_ptr;

#endif


