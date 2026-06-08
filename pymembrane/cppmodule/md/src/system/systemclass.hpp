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
#ifndef __systemclass_hpp__
#define __systemclass_hpp__

#include <algorithm>

#include "../types/pymembvector.hpp"
#include "../box/box.hpp"
#include "../box/pbc.hpp"

//meshes
#include "../mesh/halfedges.hpp"
#include "../mesh/computegeometry.hpp"
#include "../mesh/meshoperators.hpp"
#include "read_mesh.hpp"

//compute
#include "../compute/computemesh.hpp"

//dumper
#include "../dumper/dumper.hpp"
#include <pybind11/pybind11.h>
namespace py = pybind11;

/**
 * @class SystemClass
 * @brief SystemClass is the base class which is the core of PyMembrane
 * 
 */

class SystemClass
{
public:
    SystemClass() : compute_mesh(*this), dumper(*this)
    {
        Numhalfedges = 0; //!< Number of halfedges
        Numvertices = 0;  //!< Number of vertices
        Numedges = 0;     //!< Number of edges
        Numfaces = 0;     //!< Number of faces
    }
    /**
     * Create a particles from given vectors
    */
    SystemClass(const BoxType &box) : compute_mesh(*this), dumper(*this), _box(box)
    {
        Numhalfedges = 0; //!< Number of halfedges
        Numvertices = 0;  //!< Number of vertices
        Numedges = 0;     //!< Number of edges
        Numfaces = 0;     //!< Number of faces
    }
    ~SystemClass() {}

    std::string get_mesh_info(void)
    {
        std::string info = "Mesh\n";
        info += " Numvertices = " + std::to_string(vertices.size()) + "\n";
        info += " NumFaces = " + std::to_string(faces.size()) + "\n";
        info += " NumEdges = " + std::to_string(edges.size()) + "\n";
        info += " NumHEdges = " + std::to_string(halfedges.size()) + "\n";
        return info;
    }
    /*
     * Box functions
     */
    const BoxType &get_box(void);
    void set_box(const BoxType& box);
    void set_box_length(const double& Lx, const double& Ly, const double& Lz);
    real3 minImage(const real3 &a, const real3 &b);
    bool need_wrapping(const real3 &a, const real3 &b);
    void enforce_periodic_boundary_conditions(void);

    /**
     * @brief read the mesh from a file
     * @param std::map<std::string, std::string>& files 
     */
    void read_mesh_from_files(std::map<std::string, std::string> &files)
    {
        ReadMesh reader(files);
        pymemb::vector<HE_Face<PropertyFaces>> _faces = reader.get_faces();             //!< Faces list
        pymemb::vector<HE_Vertex<PropertyVertices>> _vertices = reader.get_vertices();  //!< Vertex list
        pymemb::vector<HE_Edge<PropertyEdges>> _edges = reader.get_edges();             //!< Edges list
        pymemb::vector<HE_HalfEdge<PropertyEdges>> _halfedges = reader.get_halfedges(); //!< Half-edge list
        close_surface = reader.is_close_surface();
        this->set(_faces, _vertices, _edges, _halfedges);
        //the mesh is no copy to host
        copy_in_host = false;
        compute_mesh.compute_vertex_normals();
        compute_mesh.compute_face_normals();
    }
    void read_mesh_from_json(std::string &json_file)
    {
        ReadMesh reader(json_file);
        pymemb::vector<HE_Face<PropertyFaces>> _faces = reader.get_faces();             //!< Faces list
        pymemb::vector<HE_Vertex<PropertyVertices>> _vertices = reader.get_vertices();  //!< Vertex list
        pymemb::vector<HE_Edge<PropertyEdges>> _edges = reader.get_edges();             //!< Edges list
        pymemb::vector<HE_HalfEdge<PropertyEdges>> _halfedges = reader.get_halfedges(); //!< Half-edge list
        close_surface = reader.is_close_surface();
        this->set(_faces, _vertices, _edges, _halfedges);
        //the mesh is no copy to host
        copy_in_host = false;
        compute_mesh.compute_vertex_normals();
        compute_mesh.compute_face_normals();
    }
    /**
     * @brief get the halfedges loaded in the system
     * @param void 
     * @return pymemb::vector<HE_HalfEdge<PropertyEdges>> 
     */
    pymemb::vector<HE_HalfEdge<PropertyEdges>> get_halfedges(void) { return (pymemb::copy(halfedges)); }
    /**
     * @brief get the vertices loaded in the system
     * @param void 
     * @return pymemb::vector<HE_Vertex<PropertyVertices>> 
     */
    pymemb::vector<HE_Vertex<PropertyVertices>> get_vertices(void) { return (pymemb::copy(vertices)); }
    /**
     * @brief get the edges loaded in the system
     * @param void 
     * @return pymemb::vector<HE_Edge<PropertyEdges>> 
     */
    pymemb::vector<HE_Edge<PropertyEdges>> get_edges(void) { return (pymemb::copy(edges)); }
    /**
     * @brief get the faces loaded in the system
     * @param void 
     * @return pymemb::vector<HE_Face<PropertyFaces>>
     */
    pymemb::vector<HE_Face<PropertyFaces>> get_faces(void) { return (pymemb::copy(faces)); }
    /**
     * @brief set the halfedges loaded in the system
     * @param pymemb::vector<HE_HalfEdge<PropertyEdges>>& _halfedges
     * @return void
     */
    void set_halfedges(pymemb::vector<HE_HalfEdge<PropertyEdges>> &_halfedges)
    {
        halfedges = _halfedges;
        Numhalfedges = _halfedges.size();
    }
    /**
     * @brief get the vertices loaded in the system
     * @param pymemb::vector<HE_Vertex<PropertyVertices>>& _vertices 
     * @return  
     */
    void set_vertices(pymemb::vector<HE_Vertex<PropertyVertices>> &_vertices)
    {
        vertices = _vertices;
        Numvertices = _vertices.size();
        compute_mesh.compute_vertex_normals();
        compute_mesh.compute_face_normals();
    }
    /**
     * @brief get the edges loaded in the system
     * @param pymemb::vector<HE_Edge<PropertyEdges>>& _edges 
     * @return  
     */
    void set_edges(pymemb::vector<HE_Edge<PropertyEdges>> &_edges)
    {
        edges = _edges;
        Numedges = _edges.size();
        //compute_mesh.compute_vertex_normals();
        //compute_mesh.compute_face_normals();
    }
    /**
     * @brief get the faces loaded in the system
     * @param void 
     * @return pymemb::vector<HE_Face<PropertyFaces>>
     */
    void set_faces(pymemb::vector<HE_Face<PropertyFaces>> &_faces)
    {
        faces = _faces;
        Numfaces = _faces.size();
        compute_mesh.compute_vertex_normals();
        compute_mesh.compute_face_normals();
    }
    void set(pymemb::vector<HE_Face<PropertyFaces>> &_faces,         //!< Faces list
             pymemb::vector<HE_Vertex<PropertyVertices>> &_vertices, //!< Vertex list
             pymemb::vector<HE_Edge<PropertyEdges>> &_edges,         //!< Edges list
             pymemb::vector<HE_HalfEdge<PropertyEdges>> &_halfedges  //!< Half-edge list
    )
    {
        Numhalfedges = _halfedges.size(); //!< Number of halfedges
        Numvertices = _vertices.size();   //!< Number of vertices
        Numedges = _edges.size();         //!< Number of edges
        Numfaces = _faces.size();         //!< Number of faces

        halfedges = _halfedges;
        vertices = _vertices;
        edges = _edges;
        faces = _faces;

        pybind11::print("Mesh");
        pybind11::print(" Numvertices ", vertices.size());
        pybind11::print(" NumFaces ", faces.size());
        pybind11::print(" NumEdges ", edges.size());
        pybind11::print(" NumHEdges ", halfedges.size());
        
        //init the stresses
        this->init_stresses();
    }
    //other useful functions
    pymemb::vector<int> get_edge_neighbours_host(int edge_index)
    {
        pymemb::vector<int> edge_index_vec(5);
        int he0 = edges[edge_index]._hedge;
        int he0_next = halfedges[he0].next;
        int he0_prev = halfedges[he0].prev;
        int he0_pair = halfedges[he0].pair;
        int he0_pair_next = halfedges[he0_pair].next;
        int he0_pair_prev = halfedges[he0_pair].prev;
        edge_index_vec[0] = edge_index;
        edge_index_vec[1] = halfedges[he0_next].edge;
        edge_index_vec[2] = halfedges[he0_prev].edge;
        edge_index_vec[3] = halfedges[he0_pair_next].edge;
        edge_index_vec[4] = halfedges[he0_pair_prev].edge;
        return edge_index_vec;
    }
    //return compute mesh
    const ComputeMesh &get_compute_mesh(void) { return compute_mesh; }
    const DumperClass &get_dumper(void) { return dumper; }

    void init_stresses(void)
    {
        stress_group_faces.resize(Numfaces);
        stress_group_vertices.resize(Numvertices);
        stress_group_edges.resize(Numedges);
        stress_virial_atom.resize(Numvertices);
        stress_kinetic_atom.resize(Numvertices);
        std::transform(stress_group_faces.begin(), stress_group_faces.end(), stress_group_faces.begin(), pymemb::reset_tensor<realTensor>());
        std::transform(stress_group_vertices.begin(), stress_group_vertices.end(), stress_group_vertices.begin(), pymemb::reset_tensor<realTensor>());
        std::transform(stress_group_edges.begin(), stress_group_edges.end(), stress_group_edges.begin(), pymemb::reset_tensor<realTensor>());
        std::transform(stress_virial_atom.begin(), stress_virial_atom.end(), stress_virial_atom.begin(), pymemb::reset_tensor<realTensor>());
        std::transform(stress_kinetic_atom.begin(), stress_kinetic_atom.end(), stress_kinetic_atom.begin(), pymemb::reset_tensor<realTensor>());
    }
    /**
     * @brief get the STRESSES in the vertices loaded in the system
     * @param void 
     * @return pymemb::vector<HE_Edge<PropertyEdges>> 
     */
    pymemb::vector<realTensor> get_stress_vertices(void) { return (pymemb::copy(stress_group_vertices)); }
    /**
     * @brief get the STRESSES in the edges loaded in the system
     * @param void 
     * @return pymemb::vector<HE_Edge<PropertyEdges>> 
     */
    pymemb::vector<realTensor> get_stress_edges(void) { return (pymemb::copy(stress_group_edges)); }
    /**
     * @brief get the faces loaded in the system
     * @param void 
     * @return pymemb::vector<HE_Face<PropertyFaces>>
     */
    pymemb::vector<realTensor> get_stress_faces(void) { return (pymemb::copy(stress_group_faces)); }
    /**
     * @brief get the faces loaded in the system
     * @param void 
     * @return pymemb::vector<HE_Face<PropertyFaces>>
     */
    pymemb::vector<realTensor> get_stress_virial(void) { return (pymemb::copy(stress_virial_atom)); }
    /**
     * @brief get the faces loaded in the system
     * @param void 
     * @return pymemb::vector<HE_Face<PropertyFaces>>
     */
    pymemb::vector<realTensor> get_stress_kinetic(void) { return (pymemb::copy(stress_kinetic_atom)); }
    //private:
    //< mesh
    pymemb::vector<HE_Face<PropertyFaces>> faces;         //!< Faces list
    pymemb::vector<HE_Vertex<PropertyVertices>> vertices; //!< Vertex list
    pymemb::vector<HE_HalfEdge<PropertyEdges>> halfedges; //!< Half-edge list
    pymemb::vector<HE_Edge<PropertyEdges>> edges;         //!< Edges list

    //<group of stresses
    pymemb::vector<realTensor> stress_group_faces;
    pymemb::vector<realTensor> stress_group_vertices;
    pymemb::vector<realTensor> stress_group_edges;
    pymemb::vector<realTensor> stress_virial_atom;
    pymemb::vector<realTensor> stress_kinetic_atom;

    int Numvertices;    //!< Number of vertices
    int Numedges;       //!< Number of edges
    int Numfaces;       //!< Number of faces
    int Numhalfedges;   //!< Number of halfedges
    bool close_surface; //!< Mesh is a close surface
    bool copy_in_host;
    //compute
    ComputeMesh compute_mesh;
    //dumper
    DumperClass dumper;

private:
    BoxType _box;
};

#endif