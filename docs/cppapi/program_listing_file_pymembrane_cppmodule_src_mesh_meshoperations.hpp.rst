
.. _program_listing_file_pymembrane_cppmodule_src_mesh_meshoperations.hpp:

Program Listing for File meshoperations.hpp
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_mesh_meshoperations.hpp>` (``pymembrane/cppmodule/src/mesh/meshoperations.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #ifndef __meshoperations_hpp__
   #define __meshoperations_hpp__
   
   #include "../types/globaltypes.hpp"
   
   
   class SystemClass;
   
   class MeshOperations 
   {
   public:
       MeshOperations(SystemClass &system) : _system(system)
       {
       }
       ~MeshOperations(void) {}
   
       bool check_if_edge_can_flip(const int& flip_edge_index);
       void edge_flip(const int& flip_edge_index, const bool& flip_face_up);
       void equiangulation(void);
   private:
       SystemClass& _system;
   
   };
   
   #endif
