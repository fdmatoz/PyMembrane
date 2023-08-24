/*!
* @file potentiallimit.hpp
* @brief ComputeVertexSubstrateEnergy class
* @author Daniel Matoz, fdamatoz@gmail.com
* @date 20-Sept-2017
*/

#ifndef __potentialsubstrate_HPP__
#define __potentialsubstrate_HPP__

/** @addtogroup computeenergy Compute tethering potential Energy
 *  @brief ComputeVertexSubstrateEnergy definitions
 *  @{
 */
#include <math.h>
//host
#include "computeforceclass.hpp"

//

#include "../system/systemclass.hpp"
#include "../mesh/computegeometry.hpp"

/**
 * @class ComputeVertexSubstrateEnergy
 * @brief ComputeVertexSubstrateEnergy the tethering potential, if the edge length is larger than lmax then the energy is equal to inf
 */
 
/**
 * @brief Example class that acts on the edges 
 * 
 */
class ComputeVertexSubstrateEnergy : public ComputeForceClass
{
public:
  ComputeVertexSubstrateEnergy(SystemClass &system) : ComputeForceClass(system)
  {
    m_name = "Substrate-harmonic"; //!< potential name
    m_type = "vertex";  //!< potential type
    this->set_default_properties();
  }
  ~ComputeVertexSubstrateEnergy() {}

  void set_default_properties(void) override
  {
    pymemb::vector<real> _k(NUM_TYPES_ALLOWED, 0.0);
    m_kz1 = _k;
    m_kz2 = _k;
  }

  using ComputeForceClass::set_property;
  void set_property(std::map<std::string, std::map<std::string, std::string>> &region_map) override
  {
    for (const auto &item : region_map)
    {
      if (item.first.compare("kz+") == 0)
      {
        pymemb::vector<real> _k = util::from_dict_to_vector_types(pymemb::copy(m_kz1), item.second);
        m_kz1 = _k;
      }
      else if (item.first.compare("kz-") == 0)
      {
        pymemb::vector<real> _k = util::from_dict_to_vector_types(pymemb::copy(m_kz2), item.second);
        m_kz2 = _k;
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
    value["kz+"] = util::to_string_vec(m_kz1);
    value["kz-"] = util::to_string_vec(m_kz2);
    return value;
  }

  void compute_energy(void) override {}
  void compute(void) override;
  double compute_edge_energy(int) override {return 0.0;}
  double compute_vertex_energy(int) override {return 0.0;}

private:
  pymemb::vector<real> m_kz1; //!< maximum edge allowed
  pymemb::vector<real> m_kz2; //!< maximum edge allowed
};

#endif

