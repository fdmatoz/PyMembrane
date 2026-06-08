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
#ifndef __pybind_export_box_hpp__
#define __pybind_export_box_hpp__

#include "box.hpp"

void export_BoxType(py::module &m)
{
    py::class_<BoxType>(m, "Box")
        .def("__init__", [](BoxType &self,
                            real Lx,
                            real Ly,
                            real Lz) {
            new (&self) BoxType();
            self.L.x = fabs(Lx);
            self.L.y = fabs(Ly);
            self.L.z = fabs(Lz);
            self.Llo.x = -0.5 * self.L.x;
            self.Lhi.x = 0.5 * self.L.x;
            self.Llo.y = -0.5 * self.L.y;
            self.Lhi.y = 0.5 * self.L.y;
            self.Llo.z = -0.5 * self.L.z;
            self.Lhi.z = 0.5 * self.L.z;
            self.periodic.x = false;
            self.periodic.y = false;
            self.periodic.z = false;
        })
        .def("__init__", [](BoxType &self, std::pair<real, real> Lxpair, std::pair<real, real> Lypair, std::pair<real, real> Lzpair) {
            new (&self) BoxType();
            real Lxlo = std::get<0>(Lxpair);
            real Lxhi = std::get<1>(Lxpair);
            real Lylo = std::get<0>(Lypair);
            real Lyhi = std::get<1>(Lypair);
            real Lzlo = std::get<0>(Lzpair);
            real Lzhi = std::get<1>(Lzpair);
            assert(Lxlo < Lxhi);
            assert(Lylo < Lyhi);
            assert(Lzlo < Lzhi);
            self.L.x = Lxhi - Lxlo;
            self.L.y = Lyhi - Lylo;
            self.L.z = Lzhi - Lzlo;
            self.Llo.x = Lxlo;
            self.Lhi.x = Lxhi;
            self.Llo.y = Lylo;
            self.Lhi.y = Lyhi;
            self.Llo.z = Lzlo;
            self.Lhi.z = Lzhi;
            self.periodic.x = true;
            self.periodic.y = true;
            self.periodic.z = true;
        })
        .def("__init__", [](BoxType &self, real Lx, real Ly, real Lz, bool periodic_x, bool periodic_y, bool periodic_z) {
            new (&self) BoxType();
            self.L.x = fabs(Lx);
            self.L.y = fabs(Ly);
            self.L.z = fabs(Lz);
            self.Llo.x = -0.5 * self.L.x;
            self.Lhi.x = 0.5 * self.L.x;
            self.Llo.y = -0.5 * self.L.y;
            self.Lhi.y = 0.5 * self.L.y;
            self.Llo.z = -0.5 * self.L.z;
            self.Lhi.z = 0.5 * self.L.z;
            self.periodic.x = periodic_x;
            self.periodic.y = periodic_y;
            self.periodic.z = periodic_z;
        })
        .def("__init__", [](BoxType &self, real Lx, real Ly, real Lz, bool periodic) {
            new (&self) BoxType();
            self.L.x = fabs(Lx);
            self.L.y = fabs(Ly);
            self.L.z = fabs(Lz);
            self.Llo.x = -0.5 * self.L.x;
            self.Lhi.x = 0.5 * self.L.x;
            self.Llo.y = -0.5 * self.L.y;
            self.Lhi.y = 0.5 * self.L.y;
            self.Llo.z = -0.5 * self.L.z;
            self.Lhi.z = 0.5 * self.L.z;
            self.periodic.x = periodic;
            self.periodic.y = periodic;
            self.periodic.z = periodic;
        })
        .def("__init__", [](BoxType &self, std::pair<bool, std::pair<real, real>> Lxpair, std::pair<bool, std::pair<real, real>> Lypair, std::pair<bool, std::pair<real, real>> Lzpair) {
            new (&self) BoxType();
            real Lxlo = std::get<0>(std::get<1>(Lxpair));
            real Lxhi = std::get<1>(std::get<1>(Lxpair));
            real Lylo = std::get<0>(std::get<1>(Lypair));
            real Lyhi = std::get<1>(std::get<1>(Lypair));
            real Lzlo = std::get<0>(std::get<1>(Lzpair));
            real Lzhi = std::get<1>(std::get<1>(Lzpair));
            assert(Lxlo < Lxhi);
            assert(Lylo < Lyhi);
            assert(Lzlo < Lzhi);
            self.L.x = Lxhi - Lxlo;
            self.L.y = Lyhi - Lylo;
            self.L.z = Lzhi - Lzlo;
            self.Llo.x = Lxlo;
            self.Lhi.x = Lxhi;
            self.Llo.y = Lylo;
            self.Lhi.y = Lyhi;
            self.Llo.z = Lzlo;
            self.Lhi.z = Lzhi;
            self.periodic.x = std::get<0>(Lxpair);
            self.periodic.y = std::get<0>(Lypair);
            self.periodic.z = std::get<0>(Lzpair);
        })
        .def("__repr__", [](const BoxType &self) {
            auto return_val = "<box Lx = " + std::to_string(self.L.x) + " Ly = " + std::to_string(self.L.y) + " Lz = " + std::to_string(self.L.z) + " \n";
            return_val+= "<box Lx = (" + std::to_string(self.Llo.x) + ", " + std::to_string(self.Lhi.x) + ")\n";
            return_val+= "<box Ly = (" + std::to_string(self.Llo.y) + ", " + std::to_string(self.Lhi.y) + ")\n";
            return_val+= "<box Lz = (" + std::to_string(self.Llo.z) + ", " + std::to_string(self.Lhi.z) + ")\n";
            return_val += "periodic_Lx = " + std::to_string(self.periodic.x) + " periodic_Ly = " + std::to_string(self.periodic.y) + " periodic_Lz = " + std::to_string(self.periodic.z) + " >";
            return (return_val);
        })
        .def_readwrite("Lhi", &BoxType::Lhi, "Upper box boundary")
        .def_readwrite("Llo", &BoxType::Llo, "Lower box boundary")
        .def_readwrite("L", &BoxType::L, "Box length")
        .def_readwrite("periodic", &BoxType::periodic, "Periodicity")
        //Functions over the box
        .def("volume", [](const BoxType &self) { return self.L.x * self.L.y * self.L.z; }, "Volume of the box")
        .def("scale", [](BoxType &self, const double &scalex, const double &scaley, const double &scalez) {
            self.L.x *= fabs(scalex);
            self.L.y *= fabs(scaley);
            self.L.z *= fabs(scalez);
            self.Llo.x = -0.5 * self.L.x;
            self.Lhi.x = 0.5 * self.L.x;
            self.Llo.y = -0.5 * self.L.y;
            self.Lhi.y = 0.5 * self.L.y;
            self.Llo.z = -0.5 * self.L.z;
            self.Lhi.z = 0.5 * self.L.z;
        }, "Scale the box by a factors scalex, scaley, scalez")
        ;
}


#endif