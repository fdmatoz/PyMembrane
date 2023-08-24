
.. _program_listing_file_pymembrane_cppmodule_src_read_mesh.hpp:

Program Listing for File read_mesh.hpp
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_read_mesh.hpp>` (``pymembrane/cppmodule/src/read_mesh.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef __READ_MESH_HPP__
   #define __READ_MESH_HPP__
   
   #include <fstream>
   #include <iostream>
   #include <sstream>
   
   #include "types/globaltypes.hpp"
   #include "mesh/halfedges.hpp"
   
   class ReadMesh
   {
   public:
       ReadMesh()
       {
           clear();
       }
       ReadMesh(std::string &json_file)
       {
           this->__json_mesh(json_file);
       }
       ReadMesh(std::string &faces_file, std::string &vertices_file)
       {
           this->__build_mesh(faces_file, vertices_file);
       }
   
       ReadMesh(std::map<std::string, std::string> files)
       {
   
           std::string faces_file;
           std::string vertices_file;
   
           //its seems that py::dict doesn't have .get() or find() methods
           // next workaround
           for (auto item : files)
           {
               auto key = std::string(item.first);
               auto value = std::string(item.second);
               if (key.compare("faces") == 0)
                   faces_file = value;
               else if (key.compare("vertices") == 0)
                   vertices_file = value;
           }
           //std::cout<< faces_file << " " << vertices_file << "\n";
           this->__build_mesh(faces_file, vertices_file);
       }
       void clear(void)
       {
           faces.clear();     
           vertices.clear();  
           edges.clear();     
           halfedges.clear(); 
           Numvertices = 0;   
           Numedges = 0;      
           Numfaces = 0;      
           Numhalfedges = 0;  
       }
   
       ~ReadMesh() { this->clear(); }
   
       std::vector<HE_HalfEdgeProp> &get_halfedges(void) { return halfedges; }
       std::vector<HE_VertexProp> &get_vertices(void) { return vertices; }
       std::vector<HE_EdgeProp> &get_edges(void) { return edges; }
       std::vector<HE_FaceProp> &get_faces(void) { return faces; }
   
       int getNumvertices(void) { return Numvertices; }   
       int getNumedges(void) { return Numedges; }         
       int getNumfaces(void) { return Numfaces; }         
       int getNumhalfedges(void) { return Numhalfedges; } 
       bool is_close_surface() { return close_surface; }
   
   private:
       void __add_vertex(int, real, real, real, int);
       void __add_face(int, std::vector<int>, int, int);
       int __add_edge(const HE_HalfEdgeProp&, const HE_HalfEdgeProp&);
       void __build_boundary(void);
       void __build_edges(void);
       void __build_mesh(std::string &, std::string &);
       void __json_mesh(std::string &);
       std::map<std::pair<int, int>, int> hep_map;
       std::vector<HE_FaceProp> faces;         
       std::vector<HE_VertexProp> vertices;    
       std::vector<HE_EdgeProp> edges;         
       std::vector<HE_HalfEdgeProp> halfedges; 
   
       int Numvertices;    
       int Numedges;       
       int Numfaces;       
       int Numhalfedges;   
       bool close_surface; 
   };
   #endif
