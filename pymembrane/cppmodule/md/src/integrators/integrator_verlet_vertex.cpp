#include "integrator_verlet_vertex.hpp"

void IntegratorVelocityVerletMeshVertex::prestep(void)
{
    auto deltaT = this->get_time_step();

    for (int id = 0; id < _system.Numvertices; id++)
    {
        auto vertex = _system.vertices[id];

        // perform the first half step of velocity verlet
        // r(t+deltaT) = r(t) + v(t)*deltaT + (1/2)a(t)*deltaT^2
        // v(t+deltaT/2) = v(t) + (1/2)a*deltaT
        auto dx = vertex.v.x * deltaT + (1.0 / 2.0) * vertex.accel.x * deltaT * deltaT;
        auto dy = vertex.v.y * deltaT + (1.0 / 2.0) * vertex.accel.y * deltaT * deltaT;
        auto dz = vertex.v.z * deltaT + (1.0 / 2.0) * vertex.accel.z * deltaT * deltaT;

        // limit the movement of the particles
        if (m_has_limit)
        {
            auto len = sqrt(dx * dx + dy * dy + dz * dz);
            if (len > m_limit_val)
            {
                dx = dx / len * m_limit_val;
                dy = dy / len * m_limit_val;
                dz = dz / len * m_limit_val;
            }
        }

        vertex.r.x += dx;
        vertex.r.y += dy;
        vertex.r.z += dz;

        vertex.v.x += (1.0 / 2.0) * vertex.accel.x * deltaT;
        vertex.v.y += (1.0 / 2.0) * vertex.accel.y * deltaT;
        vertex.v.z += (1.0 / 2.0) * vertex.accel.z * deltaT;

        // particles may have been moved slightly outside the box by the above steps, wrap them back into place
        pymemb::enforce_periodic(vertex.r, vertex.ip, _system.get_box());

        // Update object
        _system.vertices[id] = vertex;
    }
}

void IntegratorVelocityVerletMeshVertex::poststep(void)
{
    auto deltaT = this->get_time_step();

    for (int id = 0; id < _system.Numvertices; id++)
    {
        auto vertex = _system.vertices[id];

        // perform the SECOND step of velocity verlet
        // v(t+deltaT) = v(t+deltaT/2) + 1/2 * a(t+deltaT)*deltaT

        // first, calculate acceleration from the net force
        auto minv = (1.0) / vertex.mass;
        vertex.accel.x = vertex.forceC.x * minv;
        vertex.accel.y = vertex.forceC.y * minv;
        vertex.accel.z = vertex.forceC.z * minv;

        // then, update the velocity
        vertex.v.x += (1.0 / 2.0) * vertex.accel.x * deltaT;
        vertex.v.y += (1.0 / 2.0) * vertex.accel.y * deltaT;
        vertex.v.z += (1.0 / 2.0) * vertex.accel.z * deltaT;

        // limit the movement of the particles
        if (m_has_limit)
        {
            auto vel = sqrt(vertex.v.x * vertex.v.x + vertex.v.y * vertex.v.y + vertex.v.z * vertex.v.z);
            if ((vel * deltaT) > m_limit_val)
            {
                vertex.v.x = vertex.v.x / vel * m_limit_val / deltaT;
                vertex.v.y = vertex.v.y / vel * m_limit_val / deltaT;
                vertex.v.z = vertex.v.z / vel * m_limit_val / deltaT;
            }
        }
        // Update object
        _system.vertices[id] = vertex;
    }
}