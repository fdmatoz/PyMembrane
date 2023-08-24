
.. _program_listing_file_pymembrane_cppmodule_src_dumper_dumper.hpp:

Program Listing for File dumper.hpp
===================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_dumper_dumper.hpp>` (``pymembrane/cppmodule/src/dumper/dumper.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   
   #ifndef __dumper_hpp__
   #define __dumper_hpp__
   
   #include <string>
   #include <vector>
   #include <map>
   #include "../types/globaltypes.hpp"
   
   class SystemClass;
   
   class DumperClass
   {
   public:
       DumperClass(SystemClass& system) : _system(system), vtkLegacy(true)
       {
       }
       void mesh_vertices_faces(const std::string &file_name);
       void mesh_edge_vtk(const std::string &file_name);
       void mesh_vtk(const std::string &file_name, bool periodic = false)
       {
           if(periodic)
               this->mesh_vtk_periodic(file_name);
           else
           {
               this->mesh_vertices_faces(file_name);
               this->mesh_edge_vtk(file_name);
           }
       }
       void mesh_vtk_periodic(const std::string &file_name);
       void mesh_ply(const std::string &file_name);
       void mesh_ply_periodic(const std::string &file_name){}
       
       void mesh_json(const std::string &file_name, const bool &ffaces, const bool &fvertices);
       void mesh_txt(const std::string &file_name, const bool &ffaces, const bool &fvertices);
       void setvtkLegacyFormat(const bool& flag)
       {
           vtkLegacy = flag;
       }
      
       void user_vertex_data(const std::string&name, std::vector<real>&data);
       void user_face_data(const std::string&name, std::vector<real>&data);
       void user_edge_data(const std::string&name, std::vector<real>&data);
   
       void user_vertex_data(const std::string&name, std::vector<realTensor>&data);
       void user_face_data(const std::string&name, std::vector<realTensor>&data);
       void user_edge_data(const std::string&name, std::vector<realTensor>&data);
       
   private:
       SystemClass& _system;
       bool vtkLegacy;
       
       std::map<std::string,std::vector<realTensor>> vertex_tensor_data_map;
       std::map<std::string,std::vector<realTensor>> face_tensor_data_map;
       std::map<std::string,std::vector<realTensor>> edge_tensor_data_map;
   
       std::map<std::string,std::vector<real>> vertex_data_map;
       std::map<std::string,std::vector<real>> face_data_map;
       std::map<std::string,std::vector<real>> edge_data_map;
       
   };
   
   
   #endif
