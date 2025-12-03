#include "potentialSubstrate.hpp"

void ComputeVertexLimitForce_fn(int Numvertices,
                                HE_VertexProp *vertices,
                                real *_kz1,
                                real *_kz2)
{
    for (int vertex_index = 0; vertex_index < Numvertices; vertex_index++)
    {
        int type = vertices[vertex_index].type;
        auto r = vertices[vertex_index].r;

        auto fval = 0.0;

        if (r.z >= 0.5)
        {
            //fval = -_kz1[type] * r.z;
            vertices[vertex_index].r.z = 0.5;
            vertices[vertex_index].type = 2;
        }
        else
        {
            //fval = -_kz2[type] * r.z;
            if (r.z < 0.45)
                vertices[vertex_index].type = 1;

        }
        //vertices[vertex_index].forceC.z += fval;
    }
}
void ComputeVertexSubstrateEnergy::compute(void)
{

    ComputeVertexLimitForce_fn(_system.Numvertices,
                               &_system.vertices[0],
                               &m_kz1[0],
                               &m_kz2[0]);
}
