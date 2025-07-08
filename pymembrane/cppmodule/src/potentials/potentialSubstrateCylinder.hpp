/*!
* @file potentialSubstrateCylinder.hpp
* @brief ComputeVertexSubstrateCylinderEnergy class
* @author Martin Leocmach, martin.leocmach@gmail.com
* @date 01-April-2025
*/

#ifndef __potentialsubstratecylinder_HPP__
#define __potentialsubstratecylinder_HPP__

#include <math.h>
//host
#include "computeforceclass.hpp"

//

#include "../system/systemclass.hpp"
#include "../mesh/computegeometry.hpp"

/**
 * @class ComputeVertexSubstrateCylinderEnergy
 * @brief If a point is inside the cylinder, push it out
 */
class ComputeVertexSubstrateCylinderEnergy : public ComputeForceClass
{
public:
    ComputeVertexSubstrateCylinderEnergy(SystemClass &system) : ComputeForceClass(system)
    {
    m_name = "Substrate-Cylinder"; //!< potential name
    m_type = "vertex";  //!< potential type
    this->set_default_properties();
    }
    ~ComputeVertexSubstrateCylinderEnergy() {}

    void set_default_properties(void) override
    {
        pymemb::vector<real> n(NUM_TYPES_ALLOWED, 0.);
        m_nbcyl = n;
        m_nbcyl[0] = 1.0;
        pymemb::vector<real> a(NUM_TYPES_ALLOWED, 0.);
        m_axis = a;
        m_axis[1] = 1.0;
        pymemb::vector<real> p(NUM_TYPES_ALLOWED, 0.);
        m_position = a;
        pymemb::vector<real> r(NUM_TYPES_ALLOWED, 1.0);
        m_radius = r;
    }


  using ComputeForceClass::set_property;
  void set_property(std::map<std::string, std::map<std::string, std::string>> &region_map) override
  {
    for (const auto &item : region_map)
    {
      if (item.first.compare("nb_cylinder") == 0)
      {
        pymemb::vector<real> _n = util::from_dict_to_vector_types(pymemb::copy(m_axis), item.second);
        m_nbcyl = _n;
      }
      if (item.first.compare("axis") == 0)
      {
        pymemb::vector<real> _a = util::from_dict_to_vector_types(pymemb::copy(m_axis), item.second);
        m_axis = _a;
      }
      else if (item.first.compare("position") == 0)
      {
        pymemb::vector<real> _p = util::from_dict_to_vector_types(pymemb::copy(m_position), item.second);
        m_position = _p;
      }
      else if (item.first.compare("radius") == 0)
      {
        pymemb::vector<real> _r = util::from_dict_to_vector_types(pymemb::copy(m_radius), item.second);
        m_radius = _r;
      }
      else
        this->print_warning_property_name(item.first);
    }
  }

  std::map<std::string, std::string> get_info(void) override
  {
    std::map<std::string, std::string> value;
    value["nb_cyl"] = m_nbcyl[0];
    value["name"] = m_name;
    value["type"] = m_type;
    value["axis"] = util::to_string_vec(pymemb::vector<real>(m_axis.begin(), m_axis.begin()+2));
    value["position"] = util::to_string_vec(pymemb::vector<real>(m_position.begin(), m_position.begin()+2));
    value["radius"] = util::to_string_vec(m_radius);
    return value;
  }

  void compute_energy(void) override {}
  void compute(void) override;
  double compute_edge_energy(int) override {return 0.0;}
  double compute_vertex_energy(int) override {return 0.0;}

private:
    pymemb::vector<real> m_nbcyl;
    pymemb::vector<real> m_axis;    //using types 0, 1, 2 for x, y, z coordinates of the axis
    pymemb::vector<real> m_position;     //using types 0, 1, 2 for x, y, z coordinates of the position
    pymemb::vector<real> m_radius;
};
#endif