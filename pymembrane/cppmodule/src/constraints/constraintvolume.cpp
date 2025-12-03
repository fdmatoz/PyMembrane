#include "constraintvolume.hpp"
#include "../mesh/computegeometry.hpp"

/*! Compute volume gradient on a vertex
  *  Volume gradient \f$ \nabla_i V = \frac 1 3 \sum_T A_T \vec n_T \f$,
  *  where \f$ A_T \f$ is area of triangle T, \f$ \vec n_T \f$ is the normal
  *  vector to that triangle, and the sum is carried over all triangles in
  *  the vertex star.
  */
void ConstraintVolume::compute_gradient(void)
{
    //Current volume
    m_value = 0.0;
    //Compute the volume and the sum=G(Q)M^{-1}G(q)
    double sum = 0.0;
    for (int vertex_index = 0; vertex_index<_system.Numvertices; vertex_index++)
    {
        //! get the triangle that this vertex is part of
        int he = _system.vertices[vertex_index]._hedge;
        int first = he;
        int face_index, he_pair, he_pair_next;
        int vf[3];
        int v, v1, v2;                     //index to the vertex which you are calculating the energy/force
        double Nx = 0.0, Ny = 0.0, Nz = 0.0; //! volume gradient
        real3 r, r1, r2;
        real3 nt; //! face Normal
        do
        {
            he_pair = _system.halfedges[he].pair;
            //! DO SOMETHING WITH THAT FACE
            face_index = _system.halfedges[he_pair].face;

            if (face_index != -1) //! Remember -1 is the virtual face outside of the mesh
            {
                vf[0] = _system.faces[face_index].v1;
                vf[1] = _system.faces[face_index].v2;
                vf[2] = _system.faces[face_index].v3;
                if (vertex_index == vf[0])
                {
                    v = vf[0];
                    v1 = vf[1];
                    v2 = vf[2];
                }
                else if (vertex_index == vf[1])
                {
                    v = vf[1];
                    v1 = vf[2];
                    v2 = vf[0];
                }
                else if (vertex_index == vf[2])
                {
                    v = vf[2];
                    v1 = vf[0];
                    v2 = vf[1];
                }

                //capture the vector positions
                r = _system.vertices[v].r;
                r1 = _system.vertices[v1].r;
                r2 = _system.vertices[v2].r;

                real angle = pymemb::compute_angle_vertex(r, r1, r2);
                angle = angle/(M_PI);

                //compute the volume in an atomic operation
                nt = pymemb::compute_normal_triangle(r, r1, r2);
                m_value+= (vdot(r, nt) / 6.0 )*angle;
                //old version:  m_value+= vdot(r, nt) / 6.0 / 3.0;

                //compute the volume gradient
                Nx += (r1.y * r2.z - r2.y * r1.z);
                Ny += (r2.x * r1.z - r1.x * r2.z);
                Nz += (r1.x * r2.y - r2.x * r1.y);
            }
            //! MOVE TO THE NEXT FACE
            he_pair_next = _system.halfedges[he_pair].next;
            he = he_pair_next;
        } while ((he != first));

        Nx /= 6.0;
        Ny /= 6.0;
        Nz /= 6.0;
        grad[vertex_index].x = Nx;
        grad[vertex_index].y = Ny;
        grad[vertex_index].z = Nz;
        sum+= Nx * ref_grad[vertex_index].x + Ny * ref_grad[vertex_index].y + Nz * ref_grad[vertex_index].z; //see definition above
    }
    if (sum > 0.0)
        m_lambda = (m_value - m_target) / sum;
    else
        m_lambda = 0.0;
    // std::cout<<"volume:"<<m_value<<" m_lambda:"<<m_lambda<<std::endl;
}

void ConstraintVolume::compute_ref_gradient(void)
{
    this->compute_gradient();
    ref_grad = grad;
}

void ConstraintVolume::enforce(void)
{
    this->compute_ref_gradient();
    for (int vertex_index = 0; vertex_index<_system.Numvertices; vertex_index++)
    {
        _system.vertices[vertex_index].r.x -= m_lambda * ref_grad[vertex_index].x;
        _system.vertices[vertex_index].r.y -= m_lambda * ref_grad[vertex_index].y;
        _system.vertices[vertex_index].r.z -= m_lambda * ref_grad[vertex_index].z;
        // particles may have been moved slightly outside the box by the above steps, wrap them back into place
        pymemb::enforce_periodic(_system.vertices[vertex_index].r, _system.vertices[vertex_index].ip, _system.get_box());
    }
}