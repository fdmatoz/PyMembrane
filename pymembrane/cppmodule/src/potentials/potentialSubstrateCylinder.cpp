#include "potentialSubstrateCylinder.hpp"

bool isOutsideCylinder(real3 vertex_position,
    real3 axis,
    real3 cylinder_position,
    real radius)
{
    real3 cv = pymemb::vector_subtract(vertex_position, cylinder_position);
    real x = pymemb::vector_dot(cv, axis);
    real3 proj;
    Xvec1(proj, x, axis);
    proj = pymemb::vector_sum(cylinder_position, proj);
    real value = pymemb::vector_squared_length(cv) - pymemb::vector_squared_length(proj) - radius*radius;
    return value >= 0;
}

void ProjectOnCylinderSurface(HE_VertexProp *vertices, int vertex_index, real radius, real3 proj)
{
    real3 fpush = pymemb::vector_subtract(vertices[vertex_index].r, proj);
    fpush = pymemb::unit_vector(fpush);
    Xvec1(fpush, radius, fpush);
    vertices[vertex_index].r = pymemb::vector_sum(proj, fpush);
    vertices[vertex_index].type = 2;
}

void ComputeVertexLimitForce_fn(int Numvertices,
                                HE_VertexProp *vertices,                
                                real3 axis,
                                real3 cylinder_position,
                                real radius,
                                int* outside_cyl_count)
{
    for (int vertex_index=0; vertex_index < Numvertices; vertex_index++)
    {
        real3 vertex_position = vertices[vertex_index].r;

        real3 cv = pymemb::vector_subtract(vertex_position, cylinder_position);
        real x = pymemb::vector_dot(cv, axis);
        real3 proj;
        Xvec1(proj, x, axis);
        proj = pymemb::vector_sum(cylinder_position, proj);
        real value = pymemb::vector_length(pymemb::vector_subtract(proj, vertex_position)) - (radius);

        if (value < 0 )
        {
            ProjectOnCylinderSurface(vertices, vertex_index, radius, proj);
        }
        else
        {   
            outside_cyl_count[vertex_index]++;
        }
    }
}

void ComputeVertexSubstrateCylinderEnergy::compute(void)
{
    real3 axis, position;
    std::vector<int> outside_cyl_count = std::vector<int>(_system.Numvertices, 0);
    for (int i=0; i<m_nbcyl[0]; i++)
    {
        axis.x = m_axis[i*3];
        axis.y = m_axis[i*3+1];
        axis.z = m_axis[i*3+2];
        position.x = m_position[i*3];
        position.y = m_position[i*3+1];
        position.z = m_position[i*3+2];

        ComputeVertexLimitForce_fn(_system.Numvertices,
                                    &_system.vertices[0],
                                    axis,
                                    position,
                                    m_radius[i],
                                    &outside_cyl_count[0]);
    }

    for (int vertex_index=0; vertex_index<_system.Numvertices; vertex_index++)
    {
        if (outside_cyl_count[vertex_index] == m_nbcyl[0])
        {
            if (_system.vertices[vertex_index].type == 2)
            {
                
                real dist_min = 10000;
                int closest_cyl = 0;
                real3 closest_proj;
                for (int i=0; i<m_nbcyl[0]; i++)
                {
                    axis.x = m_axis[i*3];
                    axis.y = m_axis[i*3+1];
                    axis.z = m_axis[i*3+2];
                    position.x = m_position[i*3];
                    position.y = m_position[i*3+1];
                    position.z = m_position[i*3+2];
                    real3 vertex_position = _system.vertices[vertex_index].r;

                    real3 cv = pymemb::vector_subtract(vertex_position, position);
                    real x = pymemb::vector_dot(cv, axis);
                    real3 proj;
                    Xvec1(proj, x, axis);
                    proj = pymemb::vector_sum(position, proj);
                    real dist = pymemb::vector_length(pymemb::vector_subtract(proj, vertex_position));
                    if (dist < dist_min)
                    {
                        dist_min = dist;
                        closest_cyl = i;
                        closest_proj = proj;
                    }
                }
                ProjectOnCylinderSurface(&_system.vertices[0], vertex_index, m_radius[closest_cyl], closest_proj);
            }
            else
                _system.vertices[vertex_index].type = 1;
        }
        else
            _system.vertices[vertex_index].type = 2;
    }
}