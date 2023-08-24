#ifndef __constraintclass_HPP__
#define __constraintclass_HPP__

#include <memory>
#include <math.h>
#include <iostream>
#include "../utils/fromstring.hpp"
#include "../types/globaltypes.hpp"
#include "../system/systemclass.hpp"
#include <pybind11/pybind11.h>
namespace py = pybind11;

/*! Base class for a generic constraint
 *  \note This is a constraint such as fixed volume, or bond lengths.
*/
class ConstraintClass
{
public:
  ConstraintClass(SystemClass &system) : _system(system),
                                         m_tol(1e-6),
                                         m_target(0.0),
                                         m_value(0.0),
                                         m_type("EQUAL")
  {
  }
  virtual ~ConstraintClass() {}

  //! Enforce constraint
  virtual void enforce(void) {}

  //! Computes normal to the surface
  virtual void compute_normal(void) {}

  //! Computer gradient at a point
  virtual void compute_gradient(void) {}

  //! Compute reference gradient
  virtual void compute_ref_gradient(void) = 0;

  //! Return true if the constraint has been satisfied
  bool is_satisfied(void) { return (fabs(m_value - m_target) < m_tol); }

  //! Return true if constraint needs to be applied
  bool should_apply()
  {
    if (m_type.compare("EQUAL") == 0)
      return true;
    else if (m_type.compare("LESS") == 0 && m_value > m_target)
      return true;
    else if (m_type.compare("GREATER") == 0 && m_value > m_target)
      return true;
    else
      return false;
  }

  //! compute the value of the constraint, return the difference and the information
  std::map<std::string, std::string> info(void)
  {
    std::map<std::string, std::string> value;
    value["constraint"] = name;
    value["type"] = m_type;
    value["value"] = util::to_string(m_value);
    value["target"] = util::to_string(m_target);
    value["|value-target|"] = util::to_string_scientific(m_value - m_target);
    value["tol"] = util::to_string_scientific(m_tol);
    value["is_satisfy"] = util::to_string(this->is_satisfied());
    value["lambda"] = util::to_string(m_lambda);
    return value;
  }

  //! Get the Lagrange multiplier for a given constraint
  double get_LagrangeMultiplier(void) { return m_lambda; }
  void reset_LagrangeMultiplier(void) { m_lambda = 0.0; }

  void set_property_common(std::map<std::string, std::string> &region_map)
  {
    for (const auto &item : region_map)
    {
      auto propname = item.first;
      auto value = item.second;

      if (propname.compare("tol") == 0)
      {
        m_tol = util::from_string_double(value);
      }
      else if (propname.compare("EQUAL") == 0)
      {
        m_type = value;
      }
      else if (propname.compare("LESS") == 0)
      {
        m_type = value;
      }
      else if (propname.compare("GREATER") == 0)
      {
        m_type = value;
      }
      else
        this->print_warning_property_name(propname);
    }
  }

  virtual void set_default_properties(void) = 0;
  virtual void set_property(std::map<std::string, std::map<std::pair<std::string, std::string>, std::string>> &value_map) { this->print_warning_calling("map<string, map<pair<string, string>, string>> "); };
  virtual void set_property(std::map<std::string, std::map<std::string, std::string>> &value_map) { this->print_warning_calling("map<string, map<string, string>>"); };
  virtual void set_property(std::map<std::string, std::string> &value_map) { this->print_warning_calling("std::map<std::string, std::string>"); }
  void print_warning_calling_err(const std::string &message) { py::print("constraint ", name, " ", message); }
  void print_warning_calling(const std::string &message) { py::print("constraint ", name, " cannot be called with ", message); }
  void print_warning_property_name(const std::string &message) { py::print("parameter ", message, " is not part of ", name, " constraint"); }

protected:
  SystemClass &_system;
  std::string name;
  double m_tol;       //!< Tolerance for the constraint to be satisfied.
  double m_target;    //!< Targeted value for the constraint
  double m_value;     //!< Current value of the constrained quantity
  double m_lambda;    //!< Lagrange multiplier for a given constraint
  std::string m_type; //!< Constraint type (i.e. equality or inequality)
};

typedef std::shared_ptr<ConstraintClass> ConstraintClass_ptr;

#endif
