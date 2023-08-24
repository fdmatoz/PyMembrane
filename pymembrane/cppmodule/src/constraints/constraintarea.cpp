#include "constraintarea.hpp"
#include "../mesh/computegeometry.hpp"

void ConstraintArea::compute_gradient(void)
{
    //Current volume
    m_value = 0.0;
    //Compute the volume and the sum=G(Q)M^{-1}G(q)
    double sum = 0.0;
    for (int vertex_index = 0; vertex_index < _system.Numvertices; vertex_index++)
    {
        ///< get the triangle that this vertex is part of
        int he = _system.vertices[vertex_index]._hedge;
        int first = he;
        int face_index, he_pair, he_pair_next;
        int vf[3];
        real Fx = 0.0, Fy = 0.0, Fz = 0.0; // area gradient
        real A_T = 0.0;

        real g_now[3];
        real3 forceM11, forceM12;
        real3 r12, r13;

        do
        {
            he_pair = _system.halfedges[he].pair;
            
            face_index = _system.halfedges[he_pair].face;

            if (face_index != -1) // Remember -1 is the virtual face outside of the mesh
            {
                vf[0] = _system.faces[face_index].v1;
                vf[1] = _system.faces[face_index].v2;
                vf[2] = _system.faces[face_index].v3;

                // compute
                pymemb::compute_form_factor_triangle(g_now, _system.vertices[vf[0]].r, _system.vertices[vf[1]].r, _system.vertices[vf[2]].r);
                vf[2] = _system.faces[face_index].v3;
                A_T = pymemb::compute_area_triangle_from_metric(g_now);
                m_value += A_T;
                r12 = pymemb::vector_subtract(_system.vertices[vf[1]].r, _system.vertices[vf[0]].r);
                r13 = pymemb::vector_subtract(_system.vertices[vf[2]].r, _system.vertices[vf[0]].r);
                forceM11.x = (0.25 / A_T) * (g_now[2] * r12.x - g_now[1] * r13.x);
                forceM11.y = (0.25 / A_T) * (g_now[2] * r12.y - g_now[1] * r13.y);
                forceM11.z = (0.25 / A_T) * (g_now[2] * r12.z - g_now[1] * r13.z);

                forceM12.x = (0.25 / A_T) * (g_now[0] * r13.x - g_now[1] * r12.x);
                forceM12.y = (0.25 / A_T) * (g_now[0] * r13.y - g_now[1] * r12.y);
                forceM12.z = (0.25 / A_T) * (g_now[0] * r13.z - g_now[1] * r12.z);

                if (vertex_index == vf[0])
                {
                    //v1
                    Fx += -(forceM11.x + forceM12.x);
                    Fy += -(forceM11.y + forceM12.y);
                    Fz += -(forceM11.z + forceM12.z);
                }
                else if (vertex_index == vf[1])
                {
                    //v2
                    Fx += forceM11.x;
                    Fy += forceM11.y;
                    Fz += forceM11.z;
                }
                else if (vertex_index == vf[2])
                {
                    //v2
                    Fx += forceM12.x;
                    Fy += forceM12.y;
                    Fz += forceM12.z;
                }
            }
            
            he_pair_next = _system.halfedges[he_pair].next;
            he = he_pair_next;
        } while ((he != first));

        grad[vertex_index].x = Fx;
        grad[vertex_index].y = Fy;
        grad[vertex_index].z = Fz;
        sum+= Fx * ref_grad[vertex_index].x + Fy * ref_grad[vertex_index].y + Fz * ref_grad[vertex_index].z;
    }
    if (sum > 0.0)
        m_lambda = (m_value - m_target) / sum;
    else
        m_lambda = 0.0;
    // std::cout<<"volume:"<<m_value<<" m_lambda:"<<m_lambda<<std::endl;
}

void ConstraintArea::compute_ref_gradient(void)
{
    this->compute_gradient();
    ref_grad = grad;
}

void ConstraintArea::enforce(void)
{
    this->compute_ref_gradient();
    for (int vertex_index = 0; vertex_index < _system.Numvertices; vertex_index++)
    {
        _system.vertices[vertex_index].r.x -= m_lambda * ref_grad[vertex_index].x;
        _system.vertices[vertex_index].r.y -= m_lambda * ref_grad[vertex_index].y;
        _system.vertices[vertex_index].r.z -= m_lambda * ref_grad[vertex_index].z;
        pymemb::enforce_periodic(_system.vertices[vertex_index].r, _system.vertices[vertex_index].ip, _system.get_box());
    }
}