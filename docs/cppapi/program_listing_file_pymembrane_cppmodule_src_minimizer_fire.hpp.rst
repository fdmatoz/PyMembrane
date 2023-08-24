
.. _program_listing_file_pymembrane_cppmodule_src_minimizer_fire.hpp:

Program Listing for File fire.hpp
=================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_minimizer_fire.hpp>` (``pymembrane/cppmodule/src/minimizer/fire.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef __fire__hpp__
   #define __fire__hpp__
   
   #include <iostream>
   
   #include "minimizerclass.hpp"
   #include "../rng/rng.hpp"
   #include "../utils/fromstring.hpp"
   #include "../potentials/computeforceclass.hpp"
   #include "../evolver/evolverclass.hpp"
   
   class MinimizerMeshFIRE : public MinimizerClass
   {
   public:
     MinimizerMeshFIRE(SystemClass &system, EvolverClass &evolver) : MinimizerClass(system, evolver)
     {
       m_name = "fire";
       m_type = "minimizer";
       this->set_default_properties();
     }
     ~MinimizerMeshFIRE() {}
   
     void set_default_properties(void) override
     {
       m_nmin = 5;
       m_finc = 1.1;
       m_fdec = 0.5;
       m_alpha_start = 0.1;
       m_falpha = 0.99;
       m_ftol = 1e-1;
       m_etol = 1e-3;
       m_energy_total = 0.0;
       m_old_energy = 0.0;
       m_dE = 0.0;
       m_fnorm = 0.0;
       m_dt = 0.005;
       m_dT_max = m_dt;
       m_dT_set = m_dt / 10.0;
       m_run_minsteps = 10;
       m_converged = false;
       m_n_since_negative = m_nmin + 1;
       m_n_since_start = 0;
       m_alpha = m_alpha_start;
       m_max_iter = 10000;
     }
   
     void update_time_step_parameters(void)
     {
       m_dT_max = m_dt;
       m_dT_set = m_dt / 10.0;
     }
     using MinimizerClass::set_property;
     void set_property(std::map<std::string, std::string> &value_map) override
     {
       for (const auto &item : value_map)
       {
         if (item.first.compare("max_iter") == 0)
         {
           m_max_iter = util::from_string_int(item.second);
         }
         else if (item.first.compare("dt") == 0)
         {
           m_dt = util::from_string_double(item.second);
           this->update_time_step_parameters();
         }
         else if (item.first.compare("ftol") == 0)
         {
           m_ftol = util::from_string_double(item.second);
         }
         else if (item.first.compare("etol") == 0)
         {
           m_etol = util::from_string_double(item.second);
         }
         else if (item.first.compare("Finc") == 0)
         {
           auto double_val = util::from_string_double(item.second);
           if (!(double_val > 1.0))
           {
             std::string message = "Finc = " + util::to_string(double_val) + ". Fdec need to be greater than 1.0";
             this->print_warning_calling(message);
           }
           m_finc = util::from_string_double(item.second);
         }
         else if (item.first.compare("Fdec") == 0)
         {
           auto double_val = util::from_string_double(item.second);
           if (!(double_val < 1.0 && double_val >= 0.0))
           {
             std::string message = "Fdec = " + util::to_string(double_val) + ". Fdec need to be smaller than 1.0";
             this->print_warning_calling(message);
           }
           m_fdec = double_val;
         }
         else if (item.first.compare("AlphaStart") == 0)
         {
           auto double_val = util::from_string_double(item.second);
           if (!(double_val < 1.0 && double_val > 0.0))
           {
             std::string message = "AlphaStart = " + util::to_string(double_val) + ". AlphaStart need to be smaller than 1.0";
             this->print_warning_calling(message);
           }
           m_alpha_start = double_val;
         }
         else if (item.first.compare("Falpha") == 0)
         {
           auto double_val = util::from_string_double(item.second);
           if (!(double_val < 1.0 && double_val > 0.0))
           {
             std::string message = "Falpha = " + util::to_string(double_val) + ". Falpha need to be smaller than 1.0";
             this->print_warning_calling(message);
           }
           m_falpha = double_val;
         }
   
         else
           this->print_warning_property_name(item.first);
       }
     }
     bool is_satisfied(void) { return m_converged; }
     std::map<std::string, std::string> get_info(void) override
     {
       std::map<std::string, std::string> value;
       value["name"] = util::to_string(this->get_name());
       value["dt"] = util::to_string(m_dt);
       value["ftol"] = util::to_string(m_ftol);
       value["etol"] = util::to_string(m_etol);
       value["fnorm"] = util::to_string(m_fnorm);
       value["dE"] = util::to_string(m_dE);
       value["energy"] = util::to_string(m_energy_total);
       value["n_since_start"] = util::to_string(m_n_since_start);
       value["alpha"] = util::to_string(m_alpha);
       value["is_satisfy"] = (this->is_satisfied() ? "true" : "false");
       return value;
     }
   
     void reset(void);
     void poststep1(void);
     void poststep2(void);
     void poststep3(void);
     void minimize(void) override;
   
   private:
     unsigned int m_max_iter;
     unsigned int m_nmin, m_n_since_negative, m_n_since_start, m_run_minsteps;
     double m_finc, m_fdec, m_alpha_start, m_alpha, m_falpha, m_ftol, m_etol, m_energy_total, m_old_energy, m_dE, m_fnorm, m_dT_max, m_dT_set;
     bool m_converged;
     double m_dt;
   };
   
   #endif
