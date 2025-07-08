 /*! 
 *  @details   This code will be part of an active membrane code.
 *  @author    Dr Daniel Alejandro Matoz Fernandez(fdamatoz@gmail.com).
 *  @date      2022-
 *  @copyright See the file "LICENSE" for the full license governing this code.
 */
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
    bool edge_need_flip(int flip_edge_index);
    real compute_average_edge_length();
    void edge_flip(int flip_edge_index);
    void refine_mesh_edge_flip();
    void simplify_mesh(real collapse_threshold=0.5);
    void refine_mesh_edge_split(real split_threshhold=2.0);
    void adapt_mesh(real threshold=0.5);
    //void equiangulation(void);
    void compute_vertices_connexities();

private:
    void compute_vertex_connexity(int vertex_index);
    bool edge_need_collapse(int edge_index, real minimum);
    bool edge_can_collapse(int edge_index);
    void edge_case_collapse(int he01, int &del_e1, int &del_e2, int &del_f0, int &del_f1);
    void edge_collapse(int edge_index);
    void edge_collapse(int edge_index, int &del_vertex, int &del_e1, int &del_e2,
                        int &del_f0, int &del_f1);
    void edge_split(int edge_index, int new_vertex, int new_edge0, int new_edge1,
                    int new_edge2, int new_f0, int new_f1);
    int create_vertex(real mass=1.0);
    int create_edge();
    int create_face();
    void delete_vertex(int vertex_index);
    void delete_edge(int edge_index);
    void delete_hedge(int hedge_index);
    void delete_face(int face_index);

    SystemClass& _system;

};

#endif
/*! @} */
