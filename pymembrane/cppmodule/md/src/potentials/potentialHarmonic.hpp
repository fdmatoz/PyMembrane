#ifndef __POTENTIALHARMONIC_HPP__
#define __POTENTIALHARMONIC_HPP__


#include "computeforceclass.hpp"
#include "../system/systemclass.hpp"
#include "../mesh/computegeometry.hpp"

/**
 *  @class ComputeVertexHarmonicEnergy
 *  @brief Implements methods to compute the harmonic contribution to the edge energy.
 * 
 *  Calculates harmonic energy of the edge using:
 *  \f$ E_{harm} = \frac{1}{2} m_k \left( l - l_0 \right) \f$ where \f$ m_k \f$ is the spring constant,
 *  \f$ l_0 \f$ is length of unstretched bond and \f$ l \f$ is current edge length.
 */

class ComputeVertexHarmonicEnergy : public ComputeForceClass
{
public:
  ComputeVertexHarmonicEnergy(SystemClass &system) : ComputeForceClass(system)
  {
    m_name = "Harmonic"; //!< potential name
    m_type = "edge";     //!< potential type
    this->set_default_properties();
  }

  ~ComputeVertexHarmonicEnergy() {}

  void set_default_properties(void) override
  {
    pymemb::vector<real> _k(NUM_TYPES_ALLOWED, 0.0);
    m_k = _k;
    pymemb::vector<real> _l0(NUM_TYPES_ALLOWED, 1.0);
    m_l0 = _l0;
  }

  using ComputeForceClass::set_property;
  void set_property(std::map<std::string, std::map<std::string, std::string>> &region_map) override
  {
    for (const auto &item : region_map)
    {
      if (item.first.compare("k") == 0)
      {
        pymemb::vector<real> _k = util::from_dict_to_vector_types(pymemb::copy(m_k), item.second);
        m_k = _k;
      }
      else if (item.first.compare("l0") == 0)
      {
        pymemb::vector<real> _l0 = util::from_dict_to_vector_types(pymemb::copy(m_l0), item.second);
        m_l0 = _l0;
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
    value["k"] = util::to_string_vec(m_k);
    value["l0"] = util::to_string_vec(m_l0);
    return value;
  }

  void compute_energy(void) override;
  void compute(void) override;
  real compute_edge_energy(int) override;
  real compute_vertex_energy(int) override;
  void compute_stress(void) override;
  void compute_atomic_stress(void) override;

private:
  pymemb::vector<real> m_k;  //!< spring constant
  pymemb::vector<real> m_l0; //!< rest length
};

#endif

