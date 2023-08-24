
.. _program_listing_file_pymembrane_cppmodule_src_integrators_integrator_verlet_vertex.hpp:

Program Listing for File integrator_verlet_vertex.hpp
=====================================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_integrators_integrator_verlet_vertex.hpp>` (``pymembrane/cppmodule/src/integrators/integrator_verlet_vertex.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

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
   #ifndef __integrator_verlet_vertex__hpp__
   #define __integrator_verlet_vertex__hpp__
   
   #include <iostream>
   
   #include "integratorclass.hpp"
   
   class IntegratorVelocityVerletMeshVertex : public IntegratorClass
   {
   public:
       IntegratorVelocityVerletMeshVertex(SystemClass &system) : IntegratorClass(system)
       {
           name = "brownian";
           type = "vertex";
           this->set_default_properties();
       }
       ~IntegratorVelocityVerletMeshVertex() {}
   
       void set_default_properties(void) override
       {
           m_limit_val = 0.1;
           m_has_limit = false;
           this->set_temperature(0.0);
           this->set_time_step(5e-3);
           this->update_temperature_parameters();
           this->update_time_step_parameters();
       }
   
       void update_temperature_parameters() override {}
       void update_time_step_parameters() override {}
   
       using IntegratorClass::set_property;
       void set_property(std::map<std::string, std::string> &value_map) override
       {
           for (const auto &item : value_map)
           {
               if (item.first.compare("T") == 0)
               {
                   this->set_temperature(util::from_string_double(item.second));
                   this->update_temperature_parameters();
               }
               else if (item.first.compare("limit") == 0)
               {
                   m_has_limit = util::from_string_bool(item.second);
               }
               else if (item.first.compare("limit_val") == 0)
               {
                   m_has_limit = true;
                   m_limit_val = util::from_string_double(item.second);
               }
               else
                   print_warning_property_name(item.first);
           }
       }
   
       void prestep(void) override;
   
       void poststep(void) override;
   
   private:
       real m_limit_val;    
       bool m_has_limit;    
   };
   
   #endif
