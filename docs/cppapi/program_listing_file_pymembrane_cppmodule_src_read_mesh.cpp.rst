
.. _program_listing_file_pymembrane_cppmodule_src_read_mesh.cpp:

Program Listing for File read_mesh.cpp
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_pymembrane_cppmodule_src_read_mesh.cpp>` (``pymembrane/cppmodule/src/read_mesh.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "read_mesh.hpp"
   #include "mesh/computegeometry.hpp"
   #include "../external/json_lib/json.hpp"
   //operators
   #include "mesh/meshoperators.hpp"
   
   #include <pybind11/pybind11.h>
   namespace py = pybind11;
   
   void ReadMesh::__add_vertex(int id, real x, real y, real z, int type)
   {
       //properties
       HE_VertexProp vertex;
       vertex.id = vertices.size(); 
       vertex.r.x = x;              
       vertex.r.y = y;              
       vertex.r.z = z;              
       vertex.boundary = false;      
       vertex.coordination = -1;    
       vertex._hedge = -1;          
       vertex.type = type;
       vertex.ip.x = 0;
       vertex.ip.y = 0;
       vertex.ip.z = 0;
       vertex.normal.x = 0.;
       vertex.normal.y = 0.;
       vertex.normal.z = 0.;
       vertices.push_back(vertex);
   }
   void ReadMesh::__add_face(int id, std::vector<int> verts, int orientation, int type)
   {
       HE_FaceProp face; //Create new face
       face.id = id;
       face.outer = false;
       face.type = type;
       face.normal_reference.x = 0.0;
       face.normal_reference.y = 0.0;
       face.normal_reference.z = 1.0;
   
       int v1 = verts[0];
       int v2 = verts[1];
       int v3 = verts[2];
   
       if(orientation==-1)
       {
           int v_dummy = v2;
           v2 = v3;
           v3 = v_dummy;
       }
       verts[0] = v1;
       verts[1] = v2;
       verts[2] = v3;
       face.v1 = v1;
       face.v2 = v2;
       face.v3 = v3;
       //std::cout << v1 << " " << v2 << " " << v3 << std::endl;
   
       std::vector<HE_HalfEdgeProp> he_list;                         // Local storage for prev/next bookkeeping
   
       int N = verts.size();
       for (int i = 0; i < N; i++) // Loop over all vertices in the face
       {
           auto vi = verts[i];                                        //  First vertex
           auto vj = verts[(i + 1) % N];                              //  and its follower, that can be wrapped around
           //std::cout<< vi << " " << vj << std::endl;
           bool new_pair = false;                                     //  flag that checks if we created a new pair of half-edges
   
           HE_HalfEdgeProp he;     he.boundary=false;
           HE_HalfEdgeProp hep;    hep.boundary=false;
           std::pair<int,int> vij(vi,vj);
           std::pair<int,int> vji(vj,vi);
           if (hep_map.find(vij) == hep_map.end()) // if the half-edge does not exist
           {
               he.index = halfedges.size();        //create it
               hep.index = he.index + 1;           //and create its neighbour
               he.pair = hep.index;                //set them as pairs of each other
               hep.pair = he.index;
               hep.boundary = true;                //per construction, pair is boundary 
               hep_map[vji] = hep.index;
               new_pair = true;                    //set new_pair flag to True for post-processing*/
           }
           else                                    //if the half-edge exists retrieve it
           {
               he = halfedges[hep_map[vij]];
               he.boundary = false;                //per construction, it cannat be boundary 
               hep_map.erase(vij);
           }
           //Do the bookkeeping of the connectivity 
           he.vert_from = vi;                  
           he.vert_to = vj;                    
           he.face = face.id;                  
           he_list.push_back(he);
           if(i > 0)
           {
               if(he_list[i-1].index>=halfedges.size()) std::cout << "error here if(i > 0)" << std::endl;
               he.prev = he_list[i-1].index;
               halfedges[he_list[i-1].index].next = he.index;
           }        
           if(i == N-1)
           {
               if(he_list[0].index>=halfedges.size()) std::cout << "error here if(i == N-1)" << std::endl;
               he.next = he_list[0].index;
               halfedges[he_list[0].index].prev = he.index;
               face._hedge = he.index;
           }
           vertices[vi]._hedge = he.index;
           //Add new pair to the list of all half-edges
           if(new_pair)
           {
               halfedges.push_back(he);
               halfedges.push_back(hep);
           }
           else //Update the halfedge
           {
               halfedges[he.index] = he;
           }
           //construct the face and the vertices at the boundary 
           if(he.boundary == false)
               face.outer = false;
       }
       faces.push_back(face);
   }
   
   int ReadMesh::__add_edge(const HE_HalfEdgeProp& he, const HE_HalfEdgeProp& he_pair)
   {
       HE_EdgeProp edge;
       edge.id = edges.size();
       edge.i = he.vert_from;
       edge.j = he.vert_to;
       edge.boundary = false;
       if((he.boundary==true) || (he_pair.boundary==true)) 
       {
           edge.boundary = true; 
           vertices[edge.i].boundary = true;
           vertices[edge.j].boundary = true;
       }
       edge.face_k = he.face;             
       edge.face_l = he_pair.face;        
       edge._hedge = he.index;  
       edge.type = 0;
       edge.v0 = he.vert_to;
       edge.v2 = he.vert_from;
       int he_next = he.next;
       edge.v1 = halfedges[he_next].vert_to;
       int he_index_pair_next = he_pair.next;
       edge.v3 = halfedges[he_index_pair_next].vert_to;
       edges.push_back(edge);
       return(edge.id);
   }
   void ReadMesh::__build_boundary(void)
   {
       // After all inner faces have been adde, what is left in the hep_map dictionary
       // are boundary edges. 
       // We need to connect them to each other in the proper order
       close_surface = true;
       for(auto item : hep_map)
       {
           close_surface = false;
           auto vij = item.first;
           auto he = halfedges[hep_map[vij]];
           int i = vij.first;
           int j = vij.second;
           he.vert_from = i;
           he.vert_to = j; 
           he.face = -1;
           auto v = vertices[j];
           auto hev = halfedges[v._hedge];
           while(!hev.boundary)
           {
               hev = halfedges[halfedges[hev.prev].pair];
           }
           he.next = hev.index; 
           hev.prev = he.index;
           halfedges[he.index] = he;
           halfedges[hev.index] = hev;
       }
       Numhalfedges = halfedges.size();
   }
   
   void ReadMesh::__build_edges(void)
   {
       std::map<std::pair<int,int>, int> edges_list;
       for(int index=0;index<halfedges.size();index++)
       {
           auto he = halfedges[index];
           auto he_pair = halfedges[he.pair];
           std::pair<int, int> vij(he.vert_from, he.vert_to);
           std::pair<int, int> vji(he.vert_to, he.vert_from);
           
           if (edges_list.find(vij) == edges_list.end())   // if the edge does not exist
           {
               int edge_id = __add_edge(he, he_pair);
               edges_list[vij] = edge_id;
               edges_list[vji] = edge_id;
               halfedges[index].edge = edge_id;
           }
           else                                            //if the edge exists retrieve it
           {
               halfedges[index].edge = edges_list[vij];
           }
       }
   
   }
   
   void ReadMesh::__build_mesh(std::string &faces_file, std::string &vertices_file)
   {
       std::ifstream file_vertex(vertices_file, std::ios::in);
       if (file_vertex.good())
       {
           std::string str;
           int num_triangles = 0;
           while (getline(file_vertex, str))
           {
               std::istringstream ss(str);
               //std::cout<< str << "\n";
               real num;
               std::vector<real> aux;
               while (ss >> num)
               {
                   aux.push_back(num);
                   //std::cout<< num << "\t";
               }
               this->__add_vertex(aux[0], aux[1], aux[2], aux[3], aux[4]);
               //std::cout<< "vertex[" << vert.size()-1 << "] = < " << vert[vert.size()-1].x << "," << vert[vert.size()-1].y << "," << vert[vert.size()-1].z << " >\n";
           }
       }
       else
       {
           py::print("Error no vertex file is provided\n");
           exit(1);
       }
   
       std::ifstream file_triangles(faces_file, std::ios::in);
       if (file_triangles.good())
       {
           std::string str;
           int num_triangles = 0;
           while (getline(file_triangles, str))
           {
               std::istringstream ss(str);
               //std::cout<< str << "\n";
               int num;
               std::vector<int> aux;
               while (ss >> num)
               {
                   aux.push_back(num);
               }
               std::vector<int> triangle = {aux[1], aux[2], aux[3]};
               __add_face(aux[0], triangle, aux[4], aux[5]);
           }
       }
       else
       {
           py::print("Error no triangles file is provided\n");
           exit(1);
       }
       this->__build_boundary();
       this->__build_edges();
       std::transform(vertices.begin(), vertices.end(), vertices.begin(), pymemb::reset_vertex_forces());
       std::transform(vertices.begin(), vertices.end(), vertices.begin(), pymemb::reset_vertex_energy());
       std::transform(vertices.begin(), vertices.end(), vertices.begin(), pymemb::reset_vertex_velocities());
       std::transform(vertices.begin(), vertices.end(), vertices.begin(), pymemb::reset_vertex_acceleration());
       std::transform(edges.begin(), edges.end(), edges.begin(), pymemb::reset_edge_energy());
       std::transform(faces.begin(), faces.end(), faces.begin(), pymemb::reset_face_energy());
   }
   
   void ReadMesh::__json_mesh(std::string& json_file)
   {
      
   }
