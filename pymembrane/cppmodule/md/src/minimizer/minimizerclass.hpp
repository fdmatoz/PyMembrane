/************************************************************************************
* MIT License                                                                       *
*                                                                                   *
* Copyright (c) 2023 Dr. Daniel Alejandro Matoz Fernandez                           *
*               fdamatoz@gmail.com                                                  *
* Permission is hereby granted, free of charge, to any person obtaining a copy      *
* of this software and associated documentation files (the "Software"), to deal     *
* in the Software without restriction, including without limitation the rights      *
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell         *
* copies of the Software, and to permit persons to whom the Software is             *
* furnished to do so, subject to the following conditions:                          *
*                                                                                   *
* The above copyright notice and this permission notice shall be included in all    *
* copies or substantial portions of the Software.                                   *
*                                                                                   *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR        *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,          *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE       *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER            *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,     *
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     *
* SOFTWARE.                                                                         *
*************************************************************************************/
#ifndef __minimizerclass__hpp__
#define __minimizerclass__hpp__

#include <memory>
#include <iostream>

#include "../types/globaltypes.hpp"
#include "../utils/fromstring.hpp"
#include "../system/systemclass.hpp"
#include "../potentials/computeforceclass.hpp"
#include <pybind11/pybind11.h>
namespace py = pybind11;

class EvolverClass; //forward declaration

/**
 * @class MinimizerClass
 * @brief Abstract class for adding minimizers
 */
class MinimizerClass
{
public:
   /**
   * @brief MinimizerClass Constructor
   * 
   * @param potentials pointer to the loaded potentials
   * @param param pointer to the integrator parameters
   */
   MinimizerClass(SystemClass &system, EvolverClass &evolver) : _system(system), _evolver(evolver)
   {
   }
   /**
     * @brief MinimizerClass Destructor
     */
   virtual ~MinimizerClass()
   {
   }
   virtual void minimize(void) = 0;
   /**
     * @brief Get the name object
     * @return std::string 
     */
   std::string get_name(void) { return m_name; }
   /**
     * @brief Get the type object
     * @return std::string 
     */
   std::string get_type(void) { return m_type; }

   virtual std::map<std::string, std::string> get_info(void) = 0;

   virtual void set_default_properties(void) = 0;
   virtual void set_property(std::map<std::string, std::map<std::pair<std::string, std::string>, std::string>> &value_map) { this->print_warning_calling("map<string, map<pair<string, string>, string>> "); };
   virtual void set_property(std::map<std::string, std::map<std::string, std::string>> &value_map) { this->print_warning_calling("map<string, map<string, string>>"); };
   virtual void set_property(std::map<std::string, std::string> &value_map) { this->print_warning_calling("std::map<std::string, std::string>"); }
   void print_warning_calling(const std::string &message) { pybind11::print("minimizer ", m_name, " cannot be called with ", message); }
   void print_warning_property_name(const std::string &message) { pybind11::print("parameter ", message, " is not part of ", m_name, "-", m_type, " integrator"); }

protected:
   SystemClass &_system; //!< reference to system class where the box and particles are stored
   EvolverClass &_evolver;
   std::string m_name; //!< Integrator name
   std::string m_type; //!< integrator type, active, torque, etc
};

typedef std::shared_ptr<MinimizerClass> MinimizerClass_ptr;

#endif


