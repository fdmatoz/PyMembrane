
.. _program_listing_file_pymembrane_cppmodule_src_types_pymembvector.hpp:

Program Listing for File pymembvector.hpp
=========================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_types_pymembvector.hpp>` (``pymembrane/cppmodule/src/types/pymembvector.hpp``)

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
   #ifndef __pymembvector_hpp__
   #define __pymembvector_hpp__
   
   #include <vector>
   
   namespace pymemb
   {
       template <typename T>
       using vector = std::vector<T>;
   
       template <typename T>
       const std::vector<T> copy(std::vector<T>& vec){return vec;}
   } // namespace host
   
   
   #endif
