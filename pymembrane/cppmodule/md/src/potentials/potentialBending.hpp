/*!
* @file potentialbending.hpp
* @brief ComputeVertexBendingEnergy class
* @author Daniel Matoz, fdamatoz@gmail.com
* @date 20-Sept-2017
*/
#ifndef __POTENTIALBENDING_HPP__
#define __POTENTIALBENDING_HPP__

/** @addtogroup computeenergy Compute Vertex Bending Energy
 *  @brief ComputeVertexBendingEnergy definitions
 *  @{
 */
//host
#include "computeforceclass.hpp"

//
#include "../system/systemclass.hpp"
#include "../mesh/computegeometry.hpp"
/**
 * @class ComputeVertexBendingEnergy
 * @brief ComputeVertexBendingEnergy Compute the Seung-Nelson Bending Energy/Force
 */
class ComputeVertexBendingEnergy : public ComputeForceClass
{
public:
  ComputeVertexBendingEnergy(SystemClass &system) : ComputeForceClass(system)
  {
    m_name = "Bending"; //!< potential name
    m_type = "edge";    //!< potential type
    this->set_default_properties();
  }

  ~ComputeVertexBendingEnergy() {}

  void set_default_properties(void) override
  {
    pymemb::vector<real> _kappa(NUM_TYPES_ALLOWED, 1.0);
    m_kappa = _kappa;
  }

  using ComputeForceClass::set_property;
  void set_property(std::map<std::string, std::map<std::string, std::string>> &region_map) override
  {
    for (const auto &item : region_map)
    {
      if (item.first.compare("kappa") == 0)
      {
        pymemb::vector<real> _kappa = util::from_dict_to_vector_types(pymemb::copy(m_kappa), item.second);
        m_kappa = _kappa;
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
    value["kappa"] = util::to_string_vec(m_kappa);
    return value;
  }

  void compute_energy(void) override;
  void compute(void) override;
  real compute_edge_energy(int) override;
  real compute_vertex_energy(int) override;
  void compute_stress(void) override;
  void compute_atomic_stress(void) override;

private:
  pymemb::vector<real> m_kappa; //!< bending rigidity
};

#endif

