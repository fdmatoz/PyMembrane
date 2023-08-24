
.. _program_listing_file_pymembrane_cppmodule_src_system_systemclassbox.cpp:

Program Listing for File systemclassbox.cpp
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_system_systemclassbox.cpp>` (``pymembrane/cppmodule/src/system/systemclassbox.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "systemclass.hpp"
   #include "../box/pbc.hpp"
   
   const BoxType &SystemClass::get_box(void) { return _box; }
   void SystemClass::set_box(const BoxType &box) { _box = box; }
   void SystemClass::set_box_length(const double &Lx, const double &Ly, const double &Lz)
   {
       _box.L.x = Lx;
       _box.L.y = Ly;
       _box.L.z = Lz;
       _box.Llo.x = -0.5 * _box.L.x;
       _box.Lhi.x = 0.5 * _box.L.x;
       _box.Llo.y = -0.5 * _box.L.y;
       _box.Lhi.y = 0.5 * _box.L.y;
       _box.Llo.z = -0.5 * _box.L.z;
       _box.Lhi.z = 0.5 * _box.L.z;
   }
   
   void SystemClass::enforce_periodic_boundary_conditions(void)
   {
       // Mesh
       for (int vindex = 0;
            vindex < Numvertices;
            vindex++)
       {
           pymemb::enforce_periodic(vertices[vindex].r, vertices[vindex].ip, _box);
       }
   }
   
   real3 SystemClass::minImage(const real3 &a, const real3 &b)
   {
       return (pymemb::minimum_image(a, b, _box));
   }
   
   bool SystemClass::need_wrapping(const real3 &a, const real3 &b)
   {
       return (pymemb::need_wrapping(a, b, _box));
   }
