#include "potentialSurfaceTension.hpp"

bool is_triple_line_vertex(const int vid,
                            HE_VertexProp *vertices,
                            HE_HalfEdgeProp *halfedges)
{
    if (vertices[vid].type != 2)
        return false;
    
    int he = vertices[vid]._hedge;
    int first = he;

    do
    {
        int v = halfedges[he].vert_to;
        if (vertices[v].type == 1)
            return true;
        he = halfedges[he].pair;
        he = halfedges[he].next;
    } while (he != first);

    return false;
}

void ComputeVertexSurfaceTensionForce_fn(const int vid,
                                        HE_VertexProp *vertices,
                                        HE_HalfEdgeProp *halfedges,
                                        real *m_gamma,
                                        real *curvature)
{  
    int he = vertices[vid]._hedge;
    int first = he;
    int type_0 = vertices[vid].type;
    real3 a = vertices[vid].r;
    real3 tension_force;
    tension_force.x = 0;
    tension_force.y = 0;
    tension_force.z = 0;

    real gamma_type = m_gamma[type_0];
    if (type_0 == 1)
    {
        do
        {
            int v1 = halfedges[he].vert_to;
            int he_next = halfedges[he].next;
            int v2 = halfedges[he_next].vert_to;
            real3 b = vertices[v1].r;
            real3 c = vertices[v2].r;

            real3 normal = pymemb::compute_normal_triangle_unit(a, b, c);
            real area = pymemb::compute_area_triangle_from_vertex(a, b, c);
            real angle = pymemb::compute_angle_vertex(a, b, c);
            angle = angle/M_PI;
            Xvec1(normal, angle*area, normal);

            tension_force = pymemb::vector_sum(tension_force, normal);

            he = halfedges[he].pair;
            he = halfedges[he].next;
        } while (he != first);

        vertices[vid].forceC.x += tension_force.x*(-curvature[vid])*gamma_type;
        vertices[vid].forceC.y += tension_force.y*(-curvature[vid])*gamma_type;
        vertices[vid].forceC.z += tension_force.z*(-curvature[vid])*gamma_type;
    }

    else if (is_triple_line_vertex(vid, vertices, halfedges))
    {
        real3 triple_line_force;
        triple_line_force.x = 0;
        triple_line_force.y = 0;
        triple_line_force.z = 0;
        real triple_line_length = 0.0;
        real3 line_normal, plane_normal;
        line_normal.x = 0;
        line_normal.y = 0;
        line_normal.z = 0;
        plane_normal.x = 0;
        plane_normal.y = 0;
        plane_normal.z = 0;
        real3 tangent;
        tangent.x = 0;
        tangent.y = 0;
        tangent.z = 0;
        do
        {
            int v1 = halfedges[he].vert_to;
            int he_next = halfedges[he].next;
            int v2 = halfedges[he_next].vert_to;
            real3 b = vertices[v1].r;
            real3 c = vertices[v2].r;

            if (is_triple_line_vertex(v1, vertices, halfedges))
            {
                real length = pymemb::vector_length(pymemb::vector_subtract(a, b))/2;
                triple_line_length += length;
                real3 p;
                p.x = (a.x + b.x)/2;
                p.y = (a.y + b.y)/2;
                p.z = (a.z + b.z)/2;
                real3 normal;
                if (vertices[v2].type == 2)
                    normal = pymemb::unit_vector(pymemb::vector_subtract(p, c));
                else
                {
                    int he_pair = halfedges[he].pair;
                    he_pair = halfedges[he_pair].next;
                    real3 d = vertices[halfedges[he_pair].vert_to].r;
                    normal = pymemb::unit_vector(pymemb::vector_subtract(p, d));
                }
                Xvec1(normal, length, normal);
                line_normal = pymemb::vector_sum(line_normal, normal);
            }

            if (vertices[v1].type == 1 || vertices[v2].type == 1)
            {
                real area = pymemb::compute_area_triangle_from_vertex(a, b, c);
                real3 p = pymemb::vector_sum(b, c);
                Xvec1(p, 0.5, p);
                real3 ap = pymemb::vector_subtract(p, a);
                Xvec1(ap, area, ap);    //mesh_op.refine_mesh_edge_flip();
                //mesh_op.adapt_mesh();
                tangent = pymemb::vector_sum(tangent, ap);
            }
            else
            {
                real3 ab = pymemb::vector_subtract(b, a);
                real3 ac = pymemb::vector_subtract(c, a);
                real3 triangle_normal = pymemb::vector_cross(ab, ac);
                plane_normal = pymemb::vector_sum(plane_normal, triangle_normal);
            }

            he = halfedges[he].pair;
            he = halfedges[he].next;
        } while (he != first);

        tangent = pymemb::unit_vector(tangent);
        Xvec1(tangent, triple_line_length, tangent);
        plane_normal = pymemb::unit_vector(plane_normal);
        real tproj = pymemb::vector_dot(tangent, plane_normal);
        real3 tminus;
        Xvec1(tminus, tproj, plane_normal);
        tangent = pymemb::vector_subtract(tangent, tminus);
        
        line_normal = pymemb::unit_vector(line_normal);  
        Xvec1(line_normal, triple_line_length, line_normal);

        vertices[vid].forceC.x += tangent.x*m_gamma[1] + line_normal.x*m_gamma[2];
        vertices[vid].forceC.y += tangent.y*m_gamma[1] + line_normal.y*m_gamma[2];
        vertices[vid].forceC.z += tangent.z*m_gamma[1] + line_normal.z*m_gamma[2];
    }
}

real ComputeVertexSurfaceTension::compute_face_energy(int face_id)
{
    int v1 = _system.faces[face_id].v1;
    int v2 = _system.faces[face_id].v2;
    int v3 = _system.faces[face_id].v3;

    real area = pymemb::compute_area_triangle_from_vertex(_system.vertices[v1].r, _system.vertices[v2].r, _system.vertices[v3].r);

    if (_system.vertices[v1].type == 1 || _system.vertices[v2].type == 1 || _system.vertices[v3].type == 1 )
    {
        return area * m_gamma[1];
    }

    return -m_gamma[2] * area;
}

void ComputeVertexSurfaceTension::compute_energy(void)
{
    for (int face_id=0; face_id <_system.Numfaces; face_id++)
    {
        _system.faces[face_id].energy = compute_face_energy(face_id);
    }
}

void ComputeVertexSurfaceTension::compute(void)
{
    pymemb::vector<real> curvature = _system.compute_mesh.meancurvature();
    //pymemb::vector<real> curvature = _system.compute_mesh.gaussiancurvature();

    for(int i=0; i<_system.Numvertices; i++) {
        ComputeVertexSurfaceTensionForce_fn(i, &_system.vertices[0],
                                            &_system.halfedges[0],
                                            &m_gamma[0],
                                        &curvature[0]);
    }
}