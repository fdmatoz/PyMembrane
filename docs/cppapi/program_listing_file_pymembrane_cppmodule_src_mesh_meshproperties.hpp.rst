
.. _program_listing_file_pymembrane_cppmodule_src_mesh_meshproperties.hpp:

Program Listing for File meshproperties.hpp
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_mesh_meshproperties.hpp>` (``pymembrane/cppmodule/src/mesh/meshproperties.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #ifndef __MESHPROPERTIES_HPP__
   #define __MESHPROPERTIES_HPP__
   
   #include <string>
   #include <typeinfo>                 // operator typeid
   #include <iostream>
   #include <functional>
   #include <map>
   #include <vector>
   #include <string>
   
   #include "../types/globaltypes.hpp"
   
   #include <functional>
   
   template <class ContainerType>
   struct SetProperty
   {
       //region as an int and value as real
       virtual void set_property(ContainerType &, std::map<int, real> &) = 0;
       virtual void set_property(ContainerType &, std::map<int, int> &) = 0;
       virtual void set_property(ContainerType &, std::map<int, std::function<bool(real, real, real)>> &) = 0;
       virtual void set_property(ContainerType &, std::map<int, std::string> &) = 0;
       virtual void set_property(ContainerType &, std::map<int, std::function<real(real, real, real)>> &) = 0;
       virtual void set_property(ContainerType &, std::map<int, std::vector<real>> &) = 0;
       virtual void set_property(ContainerType &, std::map<std::string, std::string> &) = 0;
   };
   
   
   typedef struct { 
       real age;                   
       real g_reference0_growth[3];
       //*Metric growth*/
       real k11;                   // growth in the 11(xx, rr) direction 
       real k12;                   // growth in the 12(xy, rt) direction 
       real k22;                   // growth in the 12(yy, rt) direction 
   }PropertyFaces;
   
   typedef struct { 
       real3 v;                 
       real age;                
       real delaminated;        
       real radius;             
   }PropertyVertices;
   
   typedef struct { 
       real age;                
       real cot_alpha;          
       real cot_beta;           
   }PropertyEdges;
   
   template <class MeshType>
   struct SetRegionVertices: public SetProperty<MeshType>
   {  
       SetRegionVertices(MeshType& _mesh)
       { 
           auto _vertices = _mesh.get_vertices();
           for(auto i = 0; i< _vertices.size(); i++)
           { 
               _vertices[i].type = 0;  
           }
           _mesh.set_vertices(_vertices);
       }  
       void set_property(MeshType& _mesh, std::map<int, real>& region_value_map) 
       { 
           std::cerr<< " Set Region Vertices must be called with int: function(x,y,z)->bool or int" << "\n";
       }
       void set_property(MeshType& _mesh, std::map<int, int>& region_value_map) 
       { 
           auto _vertices = _mesh.get_vertices();
           for(auto items : region_value_map)
           {
               int vertex_type = items.first;
               auto  vertex_index =  items.second;
               if(vertex_index<_vertices.size())
                   _vertices[vertex_index].type = vertex_type;
           }
           _mesh.set_vertices(_vertices);
       }
       void set_property(MeshType& _mesh, std::map<int, std::function<real(real, real, real)>>&)
       {
           std::cerr<< " Set Region Vertices must be called with int: function(x,y,z)->bool or int" << "\n";
       }
       void set_property(MeshType& _mesh, std::map<int, std::vector<real>>& region_value_map)
       {
           std::cerr<< " Set Region Vertices must be called with int: function(x,y,z)->bool or int" << "\n";
       }
       void set_property(MeshType& _mesh, std::map<std::string, std::string>& region_value_map)
       {
           std::cerr<< " Set Region Vertices must be called with int: function(x,y,z)->bool or int" << "\n";
       }
       void set_property(MeshType& _mesh, std::map<int, std::function<bool(real, real, real)>>& region_value_map)
       {
           auto _vertices = _mesh.get_vertices();
           for(auto i = 0; i< _vertices.size(); i++)
           { 
               auto vertex = _vertices[i];
               real x = vertex.r.x;
               real y = vertex.r.y;
               real z = vertex.r.z;
               for(auto items : region_value_map)
               {
                   int region_type = items.first;
                   auto func =  items.second;
                   if(func(x,y,z)==true) _vertices[i].type = region_type;
               }  
           }
           _mesh.set_vertices(_vertices);
       }
   
       void set_property(MeshType& _mesh, std::map<int, std::string>& region_value_map)
       { 
           auto _vertices = _mesh.get_vertices();
           for(auto i = 0; i< _vertices.size(); i++)
           { 
               auto vertex = _vertices[i];
               for(auto items : region_value_map)
               {
                   int region_type = items.first;
                   auto boundary =  items.second;
                   if(boundary.compare("boundary")==true && vertex.boundary==true) _vertices[i].type = region_type;
               }  
           }
           _mesh.set_vertices(_vertices);
       }
   };
   
   template <class MeshType>
   struct SetRegionFaces: public SetProperty<MeshType>
   {    
       SetRegionFaces(MeshType& _mesh)
       { 
           auto _faces = _mesh.get_faces();
           for(auto i = 0; i< _faces.size(); i++)
           { 
               _faces[i].type = 0;
           }
           _mesh.set_faces(_faces);
       }
       void set_property(MeshType& _mesh, std::map<int, real>& region_value_map) 
       { 
           std::cerr<< " Set Region Faces must be called with int: function(x,y,z)->bool or int" << "\n";
       }
       void set_property(MeshType& _mesh, std::map<int, int>& region_value_map) 
       { 
           auto _faces = _mesh.get_faces();
           for(auto items : region_value_map)
           {
               int face_type = items.first;
               auto  face_index =  items.second;
               if(face_index<_faces.size())
                   _faces[face_index].type = face_type;
           }
           _mesh.set_faces(_faces);
       }
       void set_property(MeshType& _mesh, std::map<int, std::function<real(real, real, real)>>&)
       {
           std::cerr<< " Set Region Faces must be called with int: function(x,y,z)->bool or int" << "\n";
       }
       void set_property(MeshType& _mesh, std::map<int, std::vector<real>>& region_value_map)
       {
           std::cerr<< " Set Region Faces must be called with int: function(x,y,z)->bool or int" << "\n";
       }
       void set_property(MeshType& _mesh, std::map<std::string, std::string>& region_value_map)
       {
           std::cerr<< " Set Region Faces must be called with int: function(x,y,z)->bool or int" << "\n";
       }
       void set_property(MeshType& _mesh, std::map<int, std::function<bool(real, real, real)>>& region_value_map)
       {
           auto _faces = _mesh.get_faces();
           auto _vertices = _mesh.get_vertices();
           for(auto i = 0; i< _faces.size(); i++)
           { 
               auto face = _faces[i];
               int v1 = face.v1;
               int v2 = face.v2;
               int v3 = face.v3;
               real3 r1 = _vertices[v1].r;
               real3 r2 = _vertices[v2].r;
               real3 r3 = _vertices[v3].r;
               real x = (r1.x + r2.x + r3.x)/3.0;
               real y = (r1.y + r2.y + r3.y)/3.0;
               real z = (r1.z + r2.z + r3.z)/3.0;
               for(auto items : region_value_map)
               {
                   int region_type = items.first;
                   auto func =  items.second;
                   if(func(x,y,z)==true) _faces[i].type = region_type;
               }
           }
           _mesh.set_faces(_faces);
       }
   
       void set_property(MeshType& _mesh, std::map<int, std::string>& region_value_map)
       { 
           auto _faces = _mesh.get_faces();
           for(auto i = 0; i< _faces.size(); i++)
           { 
               auto face = _faces[i];
               for(auto items : region_value_map)
               {
                   int region_type = items.first;
                   auto boundary =  items.second;
                   if(boundary.compare("boundary")==true && face.boundary==true) _faces[i].type = region_type;
               }  
           }
           _mesh.set_faces(_faces);
       }
   };
   
   template <class MeshType>
   struct SetRegionEdges: public SetProperty<MeshType>
   {
       SetRegionEdges(MeshType& _mesh)
       {
           auto _edges = _mesh.get_edges();
           for(auto i = 0; i< _edges.size(); i++)
           { 
               _edges[i].type = 0;
           }
           _mesh.set_edges(_edges);
       }
   
       void set_property(MeshType& _mesh, std::map<int, real>& region_value_map) 
       { 
           std::cerr<< " Set Region Edges must be called with int: function(x,y,z)->bool or int" << "\n";
       }
       void set_property(MeshType& _mesh, std::map<int, int>& region_value_map) 
       { 
           auto _edges = _mesh.get_edges();
           for(auto items : region_value_map)
           {
               int edge_type = items.first;
               auto  edge_index =  items.second;
               if(edge_index<_edges.size())
                   _edges[edge_index].type = edge_type;
           }
           _mesh.set_edges(_edges);
       }
       void set_property(MeshType& _mesh, std::map<int, std::function<real(real, real, real)>>&)
       {
           std::cerr<< " Set Region Edges must be called with int: function(x,y,z)->bool or int" << "\n";
       }
       void set_property(MeshType& _mesh, std::map<int, std::vector<real>>& region_value_map)
       {
           std::cerr<< " Set Region Edges must be called with int: function(x,y,z)->bool or int" << "\n";
       }
       void set_property(MeshType& _mesh, std::map<std::string, std::string>& region_value_map)
       {
           std::cerr<< " Set Region Edges must be called with int: function(x,y,z)->bool or int" << "\n";
       }
       void set_property(MeshType& _mesh, std::map<int, std::function<bool(real, real, real)>>& region_value_map)
       {
           auto _edges = _mesh.get_edges();
           auto _vertices = _mesh.get_vertices();
           for(auto i = 0; i< _edges.size(); i++)
           { 
               auto edge = _edges[i];
               int v1 = edge.i;
               int v2 = edge.j;
               real3 r1 = _vertices[v1].r;
               real3 r2 = _vertices[v2].r;
               real x = (r1.x + r2.x)/2.0;
               real y = (r1.y + r2.y)/2.0;
               real z = (r1.z + r2.z)/2.0;
               
               for(auto items : region_value_map)
               {
                   int region_type = items.first;
                   auto func =  items.second;
                   if(func(x,y,z)==true) _edges[i].type = region_type;
               }
           }
           _mesh.set_edges(_edges);
       }
   
       void set_property(MeshType& _mesh, std::map<int, std::string>& region_value_map)
       { 
           auto _edges = _mesh.get_edges();
           for(auto i = 0; i< _edges.size(); i++)
           { 
               auto edge = _edges[i];
               for(auto items : region_value_map)
               {
                   int region_type = items.first;
                   auto boundary =  items.second;
                   if(boundary.compare("boundary")==true && edge.boundary==true) _edges[i].type = region_type;
               }
           }
           _mesh.set_edges(_edges);
       }
   };
   
   #endif
