
.. _program_listing_file_pymembrane_cppmodule_src_integrators_integrator_brownian_vertex.hpp:

Program Listing for File integrator_brownian_vertex.hpp
=======================================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_integrators_integrator_brownian_vertex.hpp>` (``pymembrane/cppmodule/src/integrators/integrator_brownian_vertex.hpp``)

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
   #ifndef __integrator_brownian_vertex__hpp__
   #define __integrator_brownian_vertex__hpp__
   
   
   #include <iostream>
   
   #include "integratorclass.hpp"
   #include "../rng/rng.hpp"
   
   class IntegratorBrownianMeshVertex : public IntegratorClass
   {
   public:
     IntegratorBrownianMeshVertex(SystemClass &system) : IntegratorClass(system)
     {
       name = "brownian";
       type = "vertex";
       this->set_default_properties();
     }
     ~IntegratorBrownianMeshVertex() {}
   
     void set_default_properties(void) override
     {
       m_gamma = 1.0;
       m_mu = 1.0 / m_gamma;
       this->set_temperature(0.0);
       this->set_time_step(5e-3);
       m_seed = 123456; 
       m_rng = std::make_unique<RNG>(m_seed);
       this->update_temperature_parameters();
       this->update_time_step_parameters();
     }
   
     void update_temperature_parameters() override
     {
       m_B = sqrt(2.0 * this->get_temperature() * m_mu);
     }
     void update_time_step_parameters() override
     {
       m_sqrt_dt = sqrt(this->get_time_step());
     }
   
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
         else if (item.first.compare("gamma") == 0)
         {
           m_gamma = util::from_string_double(item.second);
           m_mu = 1.0 / m_gamma;
           this->update_temperature_parameters();
         }
         else if (item.first.compare("seed") == 0)
         {
           m_seed = uint(util::from_string_int(item.second));
           m_rng = std::make_unique<RNG>(m_seed);
         }
         else
           print_warning_property_name(item.first);
       }
     }
   
     void prestep(void) override {}
   
     void poststep(void) override;
   
   private:
     real m_gamma;        
     real m_mu;           
     real m_B, m_sqrt_dt;   
     unsigned int m_seed; 
     RNG_ptr m_rng;           
   };
   
   #endif
   
