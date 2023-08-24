
.. _program_listing_file_pymembrane_cppmodule_src_mesh_halfedges.hpp:

Program Listing for File halfedges.hpp
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_mesh_halfedges.hpp>` (``pymembrane/cppmodule/src/mesh/halfedges.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef __HALFEDGES_HPP__
   #define __HALFEDGES_HPP__
   
   #include "../types/globaltypes.hpp"
   #include "meshproperties.hpp"
   
   template <typename PropertyVertices>
   struct HE_Vertex
   {
       // properties
       real3 r;          
       int id;           
       bool boundary;    
       int coordination; 
       inth3 cellId;     
       real3 normal;     
       // handle
       int _hedge; 
       // md properties
       int type;    
       real3 v;     
       real3 accel; 
   
       real3 forceC; 
       real3 forceD; 
       real3 forceE; 
       real energy;  
       real mass;    
       inth3 ip;     
       //
       real spin; 
       PropertyVertices _property;
   };
   using HE_VertexProp = HE_Vertex<PropertyVertices>;
   
   template <typename PropertyEdges>
   struct HE_Edge
   {
       int i, j;           
       int id;             
       bool boundary;      
       int face_k;         
       int face_l;         
       int v0, v1, v2, v3; 
       real l0;            
       // handle
       int _hedge; 
       // md
       int type;    
       real energy; 
       // properties
       PropertyEdges _property;
   };
   using HE_EdgeProp = HE_Edge<PropertyEdges>;
   
   template <typename PropertyFaces>
   struct HE_Face
   {
       // properties
       int id;       
       bool outer;   
       int nsides;   
       real3 normal; 
       real area;    
       real g_reference[3];     
       real g_reference_inv[3]; 
       real3 normal_reference;  
       real3 r12_reference;     
       real3 r13_reference;     
       int v1, v2, v3; 
       bool boundary;  
       // handle
       int _hedge; 
       // md
       int type;    
       real energy; 
       // properties
       PropertyFaces _property;
   };
   using HE_FaceProp = HE_Face<PropertyFaces>;
   template <typename PropertyEdges>
   struct HE_HalfEdge
   {
       // handle
       int index;     
       int vert_from; 
       int vert_to;   
       int edge;      
       int face;      
       int pair;      
       int next;      
       int prev;      
       bool boundary; 
       // properties
       PropertyEdges _property;
   };
   using HE_HalfEdgeProp = HE_HalfEdge<PropertyEdges>;
   
   struct triangle_type
   {
       int id;
       int v1, v2, v3;
       int orientation;
       real g_reference[3];      
       real3 normal_reference;   
       bool is_reference_normal; 
       bool is_reference_g;      
       int type;                 
       real energy;              
       PropertyFaces prop;
       bool operator<(const triangle_type &rhs) const { return (id < rhs.id); }
   };
   struct vertices_type
   {
       int id;
       real3 r;
       int type;     
       real3 forceC; 
       real3 forceD; 
       real3 forceE; 
       real energy;  
       real mass;    
       PropertyVertices prop;
       bool operator<(const vertices_type &rhs) const { return (id < rhs.id); }
   };
   #endif
   
