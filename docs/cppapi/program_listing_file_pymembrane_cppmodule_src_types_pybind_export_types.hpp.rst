
.. _program_listing_file_pymembrane_cppmodule_src_types_pybind_export_types.hpp:

Program Listing for File pybind_export_types.hpp
================================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_types_pybind_export_types.hpp>` (``pymembrane/cppmodule/src/types/pybind_export_types.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   /************************************************************************************
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
   #ifndef __pybind_export_types_hpp__
   #define __pybind_export_types_hpp__
   
   #include "globaltypes.hpp"
   #include "../mesh/computegeometry.hpp"
   #include "../utils/fromstring.hpp"
   
   /*  @Note In the same way that we define operators in c++ 
       (see system/particleoperators.hpp), in python is possible to define what is call
       magic methods https://rszalski.github.io/magicmethods/
       this methods are used to perfom operations in classes
       a trival example is given here for the real3 class
       (see types/pybind_export_types.hpp)
   */
   void export_real3(pybind11::module &m)
   {
       pybind11::class_<real3>(m, "real3")
           .def(pybind11::init<>())
           .def("__init__", [](real3 &instance, real x, real y, real z)
                {
                    new (&instance) real3();
                    instance.x = x;
                    instance.y = y;
                    instance.z = z;
                })
           .def("__repr__", [](const real3 &a)
                { return ("[" + util::to_string_with_precision(a.x, 15) + "," + util::to_string_with_precision(a.y, 15) + "," + util::to_string_with_precision(a.z, 15) + "]"); })
           .def_readwrite("x", &real3::x)
           .def_readwrite("y", &real3::y)
           .def_readwrite("z", &real3::z)
   
           .def("minimum_image", [](const real3 &a, const real3 &b, const BoxType &box)
                { return (pymemb::vector_subtract(a, b, box)); })
           //.def("need_wrapping", [](const real3 &a, const real3 &b, const BoxType &box)
           //     { return (pymemb::need_wrapping(a, b, box)); })
           .def("list", [](const real3 &a)
                {
                    std::vector<real> vector;
                    vector.push_back(a.x);
                    vector.push_back(a.y);
                    vector.push_back(a.z);
                    return (vector);
                })
   
           /*opeators*/
           .def(
               "norm", [](const real3 &a)
               { return (sqrt(vdot(a, a))); },
               pybind11::is_operator())
   
           .def(
               "__mul__", [](const real3 &a, const real3 &b)
               { return (vdot(a, b)); },
               pybind11::is_operator())
           .def(
               "__abs__", [](const real3 &a)
               { return (sqrt(vdot(a, a))); },
               pybind11::is_operator())
           .def(
               "__add__", [](const real3 &a, const real3 &b)
               {
                   real3 c;
                   vsum(c, a, b);
                   return (c);
               },
               pybind11::is_operator())
           .def(
               "__sub__", [](const real3 &a, const real3 &b)
               {
                   real3 c;
                   vsub(c, a, b);
                   return (c);
               },
               pybind11::is_operator())
           .def(
               "__matmul__", [](const real3 &a, const real3 &b)
               {
                   real3 c;
                   vcross(c, a, b);
                   return (c);
               },
               pybind11::is_operator())
           .def(
               "__neg__", [](const real3 &a)
               {
                   real3 c;
                   c.x = -a.x;
                   c.y = -a.y;
                   c.z = -a.z;
                   return (c);
               },
               pybind11::is_operator())
           .def(
               "__pow__", [](const real3 &a, const real &b)
               {
                   real3 c;
                   c.x = pow(a.x, b);
                   c.y = pow(a.y, b);
                   c.z = pow(a.z, b);
                   return (c);
               },
               pybind11::is_operator())
           .def(
               "__iadd__", [](const real3 &a, const real3 &b)
               {
                   real3 c;
                   vsum(c, a, b);
                   return (c);
               },
               pybind11::is_operator())
           .def(
               "__isub__", [](const real3 &a, const real3 &b)
               {
                   real3 c;
                   vsub(c, a, b);
                   return (c);
               },
               pybind11::is_operator())
           .def(
               "__imul__", [](const real3 &a, const real3 &b)
               { return (vdot(a, b)); },
               pybind11::is_operator())
           .def(
               "__scale__", [](const real3 &a, const real &b)
               {
                   real3 c = a;
                   c.x *= b;
                   c.y *= b;
                   c.z *= b;
                   return c;
               },
               pybind11::is_operator());
   }
   
   void export_inth3(pybind11::module &m)
   {
       pybind11::class_<inth3>(m, "inth3")
           .def(pybind11::init<>())
           .def("__init__", [](inth3 &instance, int x, int y, int z)
                {
                    new (&instance) inth3();
                    instance.x = x;
                    instance.y = y;
                    instance.z = z;
                })
           .def("__repr__", [](const inth3 &a)
                { return ("[" + util::to_string_with_precision(a.x) + "," + util::to_string_with_precision(a.y) + "," + util::to_string_with_precision(a.z) + "]"); })
           .def_readwrite("x", &inth3::x)
           .def_readwrite("y", &inth3::y)
           .def_readwrite("z", &inth3::z)
           /*opeators*/
           .def(
               "__mul__", [](const inth3 &a, const inth3 &b)
               { return (vdot(a, b)); },
               pybind11::is_operator())
           .def(
               "__abs__", [](const inth3 &a)
               { return (sqrt(vdot(a, a))); },
               pybind11::is_operator())
           .def(
               "__add__", [](const inth3 &a, const inth3 &b)
               {
                   inth3 c;
                   vsum(c, a, b);
                   return (c);
               },
               pybind11::is_operator())
           .def(
               "__sub__", [](const inth3 &a, const inth3 &b)
               {
                   inth3 c;
                   vsub(c, a, b);
                   return (c);
               },
               pybind11::is_operator())
           .def(
               "__matmul__", [](const inth3 &a, const inth3 &b)
               {
                   inth3 c;
                   vcross(c, a, b);
                   return (c);
               },
               pybind11::is_operator())
           .def(
               "__neg__", [](const inth3 &a)
               {
                   inth3 c;
                   c.x = -a.x;
                   c.y = -a.y;
                   c.z = -a.z;
                   return (c);
               },
               pybind11::is_operator())
           .def(
               "__pow__", [](const inth3 &a, const real &b)
               {
                   inth3 c;
                   c.x = pow(a.x, b);
                   c.y = pow(a.y, b);
                   c.z = pow(a.z, b);
                   return (c);
               },
               pybind11::is_operator())
           .def(
               "__iadd__", [](const inth3 &a, const inth3 &b)
               {
                   inth3 c;
                   vsum(c, a, b);
                   return (c);
               },
               pybind11::is_operator())
           .def(
               "__isub__", [](const inth3 &a, const inth3 &b)
               {
                   inth3 c;
                   vsub(c, a, b);
                   return (c);
               },
               pybind11::is_operator())
           .def(
               "__imul__", [](const inth3 &a, const inth3 &b)
               { return (vdot(a, b)); },
               pybind11::is_operator())
           .def(
               "__scale__", [](const inth3 &a, const int &b)
               {
                   inth3 c = a;
                   c.x *= b;
                   c.y *= b;
                   c.z *= b;
                   return c;
               },
               pybind11::is_operator());
   }
   
   void export_bool3(pybind11::module &m)
   {
       pybind11::class_<bool3>(m, "bool3")
           .def(pybind11::init<>())
           .def("__init__", [](bool3 &instance, bool x, bool y, bool z)
                {
                    new (&instance) bool3();
                    instance.x = x;
                    instance.y = y;
                    instance.z = z;
                })
           .def("__repr__", [](const bool3 &a)
                { return ("[" + util::to_string_with_precision(a.x) + "," + util::to_string_with_precision(a.y) + "," + util::to_string_with_precision(a.z) + "]"); })
           .def_readwrite("x", &bool3::x)
           .def_readwrite("y", &bool3::y)
           .def_readwrite("z", &bool3::z);
   }
   
   void export_realTensor(pybind11::module &m)
   {
       pybind11::class_<realTensor>(m, "realTensor")
           .def(pybind11::init<>())
           .def("__init__", [](realTensor &instance, real xx, real xy, real xz, real yx, real yy, real yz, real zx, real zy, real zz)
                {
                    new (&instance) realTensor();
                    instance.xx = xx;
                    instance.xy = xy;
                    instance.xz = xz;
                    instance.yx = yx;
                    instance.yy = yy;
                    instance.yz = yz;
                    instance.zx = zx;
                    instance.zy = zy;
                    instance.zz = zz;
                })
           .def("__repr__", [](const realTensor &a)
                {
                    std::string string_tensor;
                    string_tensor = "[[" + util::to_string_with_precision(a.xx, 15) + ",";
                    string_tensor += util::to_string_with_precision(a.xy, 15) + ",";
                    string_tensor += util::to_string_with_precision(a.xz, 15) + "],";
                    string_tensor += "[" + util::to_string_with_precision(a.yx, 15) + ",";
                    string_tensor += util::to_string_with_precision(a.yy, 15) + ",";
                    string_tensor += util::to_string_with_precision(a.yz, 15) + "],";
                    string_tensor += "[" + util::to_string_with_precision(a.zx, 15) + ",";
                    string_tensor += util::to_string_with_precision(a.zy, 15) + ",";
                    string_tensor += util::to_string_with_precision(a.zz, 15) + "]]";
   
                    return (string_tensor);
                })
   
           .def("trace", [](const realTensor &a)
                { return (a.xx + a.yy + a.zz); })
           .def("zero", [](realTensor &a)
                {
                    a.xx = 0.0;
                    a.xy = 0.0;
                    a.xz = 0.0;
                    a.yx = 0.0;
                    a.yy = 0.0;
                    a.yz = 0.0;
                    a.zx = 0.0;
                    a.zy = 0.0;
                    a.zz = 0.0;
                })
           .def(
               "__add__", [](const realTensor &a, const realTensor &b)
               {
                   realTensor c;
                   c.xx = a.xx + b.xx;
                   c.xy = a.xy + b.xy;
                   c.xz = a.xz + b.xz;
                   c.yx = a.yx + b.yx;
                   c.yy = a.yy + b.yy;
                   c.yz = a.yz + b.yz;
                   c.zx = a.zx + b.zx;
                   c.zy = a.zy + b.zy;
                   c.zz = a.zz + b.zz;
                   return (c);
               },
               pybind11::is_operator())
           .def(
               "__sub__", [](const realTensor &a, const realTensor &b)
               {
                   realTensor c;
                   c.xx = a.xx - b.xx;
                   c.xy = a.xy - b.xy;
                   c.xz = a.xz - b.xz;
                   c.yx = a.yx - b.yx;
                   c.yy = a.yy - b.yy;
                   c.yz = a.yz - b.yz;
                   c.zx = a.zx - b.zx;
                   c.zy = a.zy - b.zy;
                   c.zz = a.zz - b.zz;
                   return (c);
               },
               pybind11::is_operator())
           .def(
               "mul", [](const realTensor &a, const real &b)
               {
                   realTensor c = a;
                   c.xx *= b;
                   c.xy *= b;
                   c.xz *= b;
                   c.yx *= b;
                   c.yy *= b;
                   c.yz *= b;
                   c.zx *= b;
                   c.zy *= b;
                   c.zz *= b;
                   return (c);
               })
           .def(
               "div", [](const realTensor &a, const real &b)
               {
                   realTensor c = a;
                   c.xx /= b;
                   c.xy /= b;
                   c.xz /= b;
                   c.yx /= b;
                   c.yy /= b;
                   c.yz /= b;
                   c.zx /= b;
                   c.zy /= b;
                   c.zz /= b;
                   return (c);
               })
   
           .def_readwrite("xx", &realTensor::xx)
           .def_readwrite("xy", &realTensor::xy)
           .def_readwrite("xz", &realTensor::xz)
           .def_readwrite("yx", &realTensor::yx)
           .def_readwrite("yy", &realTensor::yy)
           .def_readwrite("yz", &realTensor::yz)
           .def_readwrite("zx", &realTensor::zx)
           .def_readwrite("zy", &realTensor::zy)
           .def_readwrite("zz", &realTensor::zz);
   }
   #endif
