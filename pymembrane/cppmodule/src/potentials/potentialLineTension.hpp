/*!
* @file potentialharmonic.hpp
* @brief ComputeVertexLineTension class
* @author Daniel Matoz, fdamatoz@gmail.com
* @date 20-Sept-2017
*/

#ifndef __POTENTIALLINETENSION_HPP__
#define __POTENTIALLINETENSION_HPP__

/** @addtogroup computeenergy Compute Harmonic Edge Energy
 *  @brief ComputeVertexLineTension definitions
 *  @{
 */
//host
#include "computeforceclass.hpp"

//
#include "../system/systemclass.hpp"
#include "../mesh/computegeometry.hpp"

/**
 *  @class ComputeVertexLineTension
 *  @brief Implements the line tension contribution to the edge energy.
 * 
 * This is the most basic form of the line tension energy.
 * The line tension energy \( E \) associated with a vertex is given by:
 * \f[
 * E = \sum_{\text{neighboring edges}} \gamma_{\text{type}} \times s
 * \f]
 * 
 * Where:
 * - \(\gamma_{\text{type}}\) is determined by:
 *   - Averaging the \(\gamma\) values of the two vertices forming the edge if \texttt{flag\_avg} is true: 
 *     \f[ \gamma_{\text{type}} = 0.5 \times (\gamma[\text{type\_0}] + \gamma[\text{type\_1}]) \f]
 *   - Taking the \(\gamma\) value of the query vertex if \texttt{flag\_avg} is false: \(\gamma_{\text{type}} = \gamma[\text{type\_0}]\)
 * 
 * - \(s\) is either:
 *   - The Euclidean distance between the two vertices if \texttt{flag\_scale} is true: \f[ s = ||\textbf{r}_{\text{query\_vertex}} - \textbf{r}_{\text{neighboring\_vertex}}|| \f]
 *   - 1.0 if \texttt{flag\_scale} is false.
 * 
 */
class ComputeVertexLineTension : public ComputeForceClass
{
public:
  ComputeVertexLineTension(SystemClass &system) : ComputeForceClass(system)
  {
    m_name = "Line Tension"; //!< potential name
    m_type = "vertex";       //!< potential type
    this->set_default_properties();
  }

  ~ComputeVertexLineTension() {}

  void set_default_properties(void) override
  {
    std::vector<double> _gamma(NUM_TYPES_ALLOWED, 0.0);
    m_gamma = _gamma;
    m_flag_avg = false;
    m_flag_scale = false;
  }

  using ComputeForceClass::set_property;
  void set_property(std::map<std::string, std::map<std::string, std::string>> &region_map) override
  {
    for (const auto &item : region_map)
    {
      if (item.first.compare("gamma") == 0)
      {
        pymemb::vector<real> _gamma = util::from_dict_to_vector_types(pymemb::copy(m_gamma), item.second);
        m_gamma = _gamma;
      }
      else
        this->print_warning_property_name(item.first);
    }
  }

  void set_property(std::map<std::string, std::string> &region_map) override
  {
    for (const auto &item : region_map)
    {
      if (item.first.compare("avg") == 0)
        m_flag_avg = util::from_string_bool(item.second);
      else if (item.first.compare("scale") == 0)
        m_flag_scale = util::from_string_bool(item.second);
      else
        this->print_warning_property_name(m_name);
    }
  }

  std::map<std::string, std::string> get_info(void) override
  {
    std::map<std::string, std::string> value;
    value["name"] = m_name;
    value["type"] = m_type;
    value["gamma"] = util::to_string_vec(m_gamma);
    value["avg"] = util::to_string(m_flag_avg);
    value["scale"] = util::to_string(m_flag_scale);
    return value;
  }

  void compute_energy(void) override;
  real compute_edge_energy(int) override;
  real compute_vertex_energy(int) override;

private:
  pymemb::vector<real> m_gamma; //!< spring constant
  bool m_flag_avg;            //!< if is true then use a simple algebraic betwen type is use it
  bool m_flag_scale;
};

#endif

