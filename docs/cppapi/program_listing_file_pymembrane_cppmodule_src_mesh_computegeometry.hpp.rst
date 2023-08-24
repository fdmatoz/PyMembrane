
.. _program_listing_file_pymembrane_cppmodule_src_mesh_computegeometry.hpp:

Program Listing for File computegeometry.hpp
============================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_mesh_computegeometry.hpp>` (``pymembrane/cppmodule/src/mesh/computegeometry.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef __computegeometry_hpp__
   #define __computegeometry_hpp__
   
   #pragma once
   
   #include "../types/globaltypes.hpp"
   #include "../box/pbc.hpp"
   
   namespace pymemb
   {
       inline
       real3 unit_vector(const real3 &v)
       {
           real norm = sqrt(vdot(v,v));
           real3 v_unit = v;
           v_unit.x/=norm;
           v_unit.y/=norm;
           v_unit.z/=norm;
           return(v_unit);
       }
     
       inline 
       real3 vector_cross(const real3 &v1, const real3 &v2)
       {
           real3 v;
           vcross(v, v1, v2);
           return v;
       }
   
       inline 
       real3 vector_sum(const real3 &v1, const real3 &v2)
       {
           real3 v;
           v.x = v1.x + v2.x;
           v.y = v1.y + v2.y;
           v.z = v1.z + v2.z;
           return v;
       }
   
       inline 
       real3 vector_subtract(const real3 &v1, const real3 &v2)
       {
           real3 v;
           v.x = v1.x - v2.x;
           v.y = v1.y - v2.y;
           v.z = v1.z - v2.z;
           return v;
       }
   
       inline 
       real3 vector_subtract(const real3 &v1, const real3 &v2, const BoxType &box)
       {
           return (pymemb::minimum_image(v2, v1, box));
       }
   
       inline 
       void compute_form_factor_triangle(real *_metric, const real3 &r1, const real3 &r2, const real3 &r3, const BoxType &box)
       {
           real3 v12, v13;
           v12 = pymemb::minimum_image(r1, r2, box);
           v13 = pymemb::minimum_image(r1, r3, box);
           _metric[0] = vdot(v12, v12);
           _metric[1] = vdot(v12, v13);
           _metric[2] = vdot(v13, v13);
       }
   
       inline 
       void compute_form_factor_triangle(real *_metric, const real3 &r1, const real3 &r2, const real3 &r3)
       {
           real3 v12, v13;
           vsub(v12, r2, r1);
           vsub(v13, r3, r1);
           _metric[0] = vdot(v12, v12);
           _metric[1] = vdot(v12, v13);
           _metric[2] = vdot(v13, v13);
       }
   
       inline 
       real3 compute_normal_triangle(const real3 &r1, const real3 &r2, const real3 &r3, const BoxType &box)
       {
           real3 normal, v12, v13;
           v12 = pymemb::minimum_image(r1, r2, box);
           v13 = pymemb::minimum_image(r1, r3, box);
           vcross(normal, v12, v13);
           return normal;
       }
   
       inline 
       real3 compute_normal_triangle(const real3 &r1, const real3 &r2, const real3 &r3)
       {
           real3 normal, v12, v13;
           vsub(v12, r2, r1);
           vsub(v13, r3, r1);
           vcross(normal, v12, v13);
           return normal;
       }
   
       inline 
       real3 compute_normal_triangle_unit(const real3 &r1, const real3 &r2, const real3 &r3, const BoxType &box)
       {
           return (unit_vector(compute_normal_triangle(r1, r2, r3, box)));
       }
   
       inline 
       real3 compute_normal_triangle_unit(const real3 &r1, const real3 &r2, const real3 &r3)
       {
           return (unit_vector(compute_normal_triangle(r1, r2, r3)));
       }
   
       inline 
       real compute_area_triangle_from_vertex(const real3 &r1, const real3 &r2, const real3 &r3, const BoxType &box)
       {
           real3 normal = compute_normal_triangle(r1, r2, r3, box);
           return (0.5 * sqrt(vdot(normal, normal)));
       }
   
       inline 
       real compute_area_triangle_from_vertex(const real3 &r1, const real3 &r2, const real3 &r3)
       {
           real3 normal = compute_normal_triangle(r1, r2, r3);
           return (0.5 * sqrt(vdot(normal, normal)));
       }
   
       inline 
       real compute_area_triangle_from_metric(const real *_metric)
       {
           return (0.5 * sqrt(_metric[0] * _metric[2] - _metric[1] * _metric[1]));
       }
   
       inline 
       real compute_angle_vertex(const real3 &r1, const real3 &r2, const real3 &r3, const BoxType &box)
       {
           real3 v12, v13;
           v12 = pymemb::minimum_image(r1, r2, box);
           v13 = pymemb::minimum_image(r1, r3, box);
           real angle = acos(vdot(v12, v13) / sqrt(vdot(v12, v12) * vdot(v13, v13)));
           return angle;
       }
   
       inline 
       real compute_angle_vertex(const real3 &r1, const real3 &r2, const real3 &r3)
       {
           real3 v12, v13;
           vsub(v12, r2, r1);
           vsub(v13, r3, r1);
           real angle = acos(vdot(v12, v13) / sqrt(vdot(v12, v12) * vdot(v13, v13)));
           return angle;
       }
   
       inline bool is_equiangular(const real3 &r0, const real3 &r1, const real3 &r2, const real3 &r3, const BoxType &box)
       {
           // We use the notation of Brakke's paper
           // a;      edge length
           // b, c;   lengths of other two edges of triangle 1
           // d, e;   lengths of other two edges of triangle 2
           auto r02 = vector_subtract(r2, r0, box);
           auto a2 = vdot(r02, r02);
           
           auto r03 = vector_subtract(r3, r0, box);
           auto b2 = vdot(r03, r03);
           auto r23 = vector_subtract(r3, r2, box);
           auto c2 = vdot(r23, r23);
   
           auto r01 = vector_subtract(r1, r0, box);
           auto d2 = vdot(r01, r01);
           auto r21 = vector_subtract(r1, r2, box);
           auto e2 = vdot(r21, r21);
   
           if ((b2 + c2 - a2)/(sqrt(b2*c2)) + (d2 + e2 - a2)/sqrt(d2*e2) < 0.0)
               return false;
           else
               return true;
       }
   
       inline void arrange_vertices_by_smallest(int& v1, int& v2, int& v3)
       {
           int _v1, _v2, _v3;
           if (v1 < v2 && v1 < v3)
           {
               _v1 = v1;
               _v2 = v2;
               _v3 = v3;
   
           }
           else if (v2 < v1 && v2 < v3)
           {
               _v1 = v2;
               _v2 = v3;
               _v3 = v1;
           }
           else
           {
               _v1 = v3;
               _v2 = v1;
               _v3 = v2;
           }
           v1 = _v1;
           v2 = _v2;
           v3 = _v3;
       }
   
       inline 
       void compute_matrix_F(real *F, const real *g_reference_inv, const real *g_now)
       {
   
           /*
            _                                         _     _                     _     _      _
           |   g_reference_inv[0]  g_reference_inv[1]  |   |   g_now[0]  g_now[1]  |   |  1  0  |
       F = |                                           | x |                       | - |        |
           |_  g_reference_inv[1]  g_reference_inv[2] _|   |_  g_now[1]  g_now[2] _|   |_ 0  1 _|
       */
           F[0] = g_reference_inv[0] * g_now[0] + g_reference_inv[1] * g_now[1] - 1.0; //F11
           F[1] = g_reference_inv[0] * g_now[1] + g_reference_inv[1] * g_now[2];       //F12
           F[2] = g_reference_inv[1] * g_now[0] + g_reference_inv[2] * g_now[1];       //F21
           F[3] = g_reference_inv[1] * g_now[1] + g_reference_inv[2] * g_now[2] - 1.0; //F22
       }
   
       inline 
       real3 cmassT(const real3 &r1, const real3 &r2, const real3 &r3)
       {
           real3 vcm;
           vcm.x = (r1.x + r2.x + r3.x) / 3.0;
           vcm.y = (r1.y + r2.y + r3.y) / 3.0;
           vcm.z = (r1.z + r2.z + r3.z) / 3.0;
           return vcm;
       }
   
       inline 
       void RefMatrixFromCartesian(const real theta, const real *__restrict__ grefCart, real *grefCylin)
       {
           real a = sin(theta);
           real b = cos(theta);
           real grr = b * (b * grefCart[0] + a * grefCart[1]) + a * (b * grefCart[1] + a * grefCart[2]); //grr
           real grt = b * (b * grefCart[1] + a * grefCart[2]) - a * (b * grefCart[0] + a * grefCart[1]); //grt
           real gtt = b * (b * grefCart[2] - a * grefCart[1]) - a * (b * grefCart[1] - a * grefCart[0]); //gtt
           grefCylin[0] = grr;
           grefCylin[1] = grt;
           grefCylin[2] = gtt;
       }
   
       inline 
       void RefMatrixFromCylindrical(const real theta, const real *__restrict__ grefCylin, real *grefCart)
       {
           real a = sin(theta);
           real b = cos(theta);
           real g11 = b * (b * grefCylin[0] - a * grefCylin[1]) - a * (b * grefCylin[1] - a * grefCylin[2]);
           real g12 = a * (b * grefCylin[0] - a * grefCylin[1]) + b * (b * grefCylin[1] - a * grefCylin[2]);
           //real g21 = b*(a*grefCylin[0] + b*grefCylin[1]) - a*(a*grefCylin[1] + b*grefCylin[2]);
           real g22 = a * (a * grefCylin[0] + b * grefCylin[1]) + b * (a * grefCylin[1] + b * grefCylin[2]);
           grefCart[0] = g11;
           grefCart[1] = g12;
           grefCart[2] = g22;
       }
       
       inline bool need_wrapping(const real3 &r1, const real3 &r2, const BoxType &box)
       {
           real3 rij;
           vsub(rij, r2, r1);
           bool _need_wrapping = false;
           if (box.periodic.x)
           {
               if (rij.x > box.Lhi.x)
                   _need_wrapping = true;
               else if (rij.x < box.Llo.x)
                   _need_wrapping = true;
           }
           if (box.periodic.y)
           {
               if (rij.y > box.Lhi.y)
                   _need_wrapping = true;
               else if (rij.y < box.Llo.y)
                   _need_wrapping = true;
           }
           if (box.periodic.z)
           {
               if (rij.z > box.Lhi.z)
                   _need_wrapping = true;
               else if (rij.z < box.Llo.z)
                   _need_wrapping = true;
           }
           return _need_wrapping;
       }
   } 
   #endif
