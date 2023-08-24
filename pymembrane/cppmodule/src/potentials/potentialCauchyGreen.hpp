/*!
* @file potentialCauchyGreen.hpp
* @brief ComputeVertexCauchyGreenEnergy class
* @author Daniel Matoz, fdamatoz@gmail.com
* @date 20-Sept-2017
*/

#ifndef __POTENTIALCAUCHYGREEN_HPP__
#define __POTENTIALCAUCHYGREEN_HPP__


#include "computeforceclass.hpp"
#include "../system/systemclass.hpp"
#include "../mesh/computegeometry.hpp"

/**
 * @class ComputeVertexCauchyGreenEnergy
 * @brief Compute Cauchy-Green stretching energy for a triangle
 *  Stretching energy density is calculated as
 * 
 *  \f$ E=\frac{k}{2}\frac{1}{1+\nu}\left(Tr\left(C^2\right) + \frac{\nu}{1-\nu}\left(Tr C\right)^2\right)  \f$,
 *   where \f$ k \f$ is the elastic constant, \f$ \nu \f$ if the Poisson ratio, and \f$ C = \frac{1}{2}\left(FQ-I\right) \f$, with
 *   \f$ F \f$ being Gram matrix for strained triangle, \f$ Q \f$ being the inverse of the Gram matrix for the unstretched 
 *   triangle and \f$ I \f$ is the identity matrix. 
 */
class ComputeVertexCauchyGreenEnergy : public ComputeForceClass
{
public:
  ComputeVertexCauchyGreenEnergy(SystemClass &system) : ComputeForceClass(system)
  {
    m_name = "Cauchy-Green"; //!< potential name
    m_type = "face";        //!< potential type
    this->set_default_properties();
  }

  ~ComputeVertexCauchyGreenEnergy() {}

  void set_default_properties(void) override
  {
    pymemb::vector<real> _E(NUM_TYPES_ALLOWED, 1.0);
    Y = _E;
    pymemb::vector<real> _nu(NUM_TYPES_ALLOWED, (1.0 / 3.0));
    nu = _nu;
    pymemb::vector<real> _h(NUM_TYPES_ALLOWED, 1.0);
    h = _h;
  }

  using ComputeForceClass::set_property;
  void set_property(std::map<std::string, std::map<std::string, std::string>> &region_map) override
  {
    for (const auto &item : region_map)
    {
      if (item.first.compare("E") == 0)
      {
        pymemb::vector<real> _E = util::from_dict_to_vector_types(pymemb::copy(Y), item.second);
        Y = _E;
      }
      else if (item.first.compare("nu") == 0)
      {
        pymemb::vector<real> _nu = util::from_dict_to_vector_types(pymemb::copy(nu), item.second);
        nu = _nu;
      }
      else if (item.first.compare("h") == 0)
      {
        pymemb::vector<real> _h = util::from_dict_to_vector_types(pymemb::copy(h), item.second);
        h = _h;
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
    value["E"] = util::to_string_vec(Y);
    value["nu"] = util::to_string_vec(nu);
    value["h"] = util::to_string_vec(h);
    return value;
  }

  void compute_energy(void) override;
  void compute(void) override;
  real compute_edge_energy(int) override;
  real compute_vertex_energy(int) override;
  void compute_stress(void) override;
  void compute_atomic_stress(void) override;

private:
  pymemb::vector<real> Y;  //!< Young's modulus
  pymemb::vector<real> nu; //!< Poison's ratio
  pymemb::vector<real> h;  //!< Thickness of the film
};

#endif

