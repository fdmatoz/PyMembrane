
.. _program_listing_file_pymembrane_cppmodule_src_rng_rng.hpp:

Program Listing for File rng.hpp
================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_rng_rng.hpp>` (``pymembrane/cppmodule/src/rng/rng.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   /************************************************************************************
   * MIT License                                                                       *
   *                                                                                   *
   * Copyright (c) 2023 Dr. Daniel Alejandro Matoz Fernandez                           *
   *                    Dr. Rastko Sknepnek, University of Dundee                      *
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
   #ifndef __rng_hpp__
   #define __rng_hpp__
   
   
   #include <random>
   #include <memory>
   
   class RNG
   {
   public:
     
     RNG(unsigned int seed) : m_generator(seed), m_uniform_distribution(0.0,1.0), m_normal_distribution(0.0,1.0) { }
     
     ~RNG() { }
     
     real drnd()
     {
       return m_uniform_distribution(m_generator);
     }
   
     real gauss_rng(real sigma=1.0, real mu = 0.0)
     {
       return (m_normal_distribution(m_generator)*sigma + mu);
     }
   
     int lrnd(int N)
     {
       return static_cast<int>(N*drnd());
     }
   
   private:
     
     std::mt19937_64 m_generator;  
     std::uniform_real_distribution<real> m_uniform_distribution;  // Uniform random numbers
     std::normal_distribution<real> m_normal_distribution; // Gaussian distribution zero mean, unit variance
     
   };
   
   
   typedef std::unique_ptr<RNG> RNG_ptr;  
   
   #endif
    
