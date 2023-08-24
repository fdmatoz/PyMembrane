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
        faces.clear();     //!< Faces list
        vertices.clear();  //!< Vertex list
        edges.clear();     //!< Edges list
        halfedges.clear(); //!< Half-edge list
        Numvertices = 0;   //!< Number of vertices
        Numedges = 0;      //!< Number of edges
        Numfaces = 0;      //!< Number of faces
        Numhalfedges = 0;  //!< Number of halfedges
    }

    ~ReadMesh() { this->clear(); }

    /**
     * @brief get the halfedges loaded in the system
     * @param void 
     * @return std::vector<HE_HalfEdgeProp> 
     */
    std::vector<HE_HalfEdgeProp> &get_halfedges(void) { return halfedges; }
    /**
     * @brief get the vertices loaded in the system
     * @param void 
     * @return std::vector<HE_VertexProp> 
     */
    std::vector<HE_VertexProp> &get_vertices(void) { return vertices; }
    /**
     * @brief get the edges loaded in the system
     * @param void 
     * @return std::vector<HE_EdgeProp>& 
     */
    std::vector<HE_EdgeProp> &get_edges(void) { return edges; }
    /**
     * @brief get the faces loaded in the system
     * @param void 
     * @return std::vector<HE_FaceProp>& 
     */
    std::vector<HE_FaceProp> &get_faces(void) { return faces; }

    int getNumvertices(void) { return Numvertices; }   //!< Number of vertices
    int getNumedges(void) { return Numedges; }         //!< Number of edges
    int getNumfaces(void) { return Numfaces; }         //!< Number of faces
    int getNumhalfedges(void) { return Numhalfedges; } //!< Number of halfedges
    bool is_close_surface() { return close_surface; }

private:
    /**
     * Rastko mesh builder
    */
    void __add_vertex(int, real, real, real, int);
    void __add_face(int, std::vector<int>, int, int);
    int __add_edge(const HE_HalfEdgeProp&, const HE_HalfEdgeProp&);
    void __build_boundary(void);
    void __build_edges(void);
    void __build_mesh(std::string &, std::string &);
    void __json_mesh(std::string &);
    std::map<std::pair<int, int>, int> hep_map;
    /**
    * MESH Parameters
    */
    std::vector<HE_FaceProp> faces;         //!< Faces list
    std::vector<HE_VertexProp> vertices;    //!< Vertex list
    std::vector<HE_EdgeProp> edges;         //!< Edges list
    std::vector<HE_HalfEdgeProp> halfedges; //!< Half-edge list

    int Numvertices;    //!< Number of vertices
    int Numedges;       //!< Number of edges
    int Numfaces;       //!< Number of faces
    int Numhalfedges;   //!< Number of halfedges
    bool close_surface; //!< Is a close surface
};
#endif