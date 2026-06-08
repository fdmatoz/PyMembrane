#ifndef __meshoperators_hpp__
#define __meshoperators_hpp__

#include "halfedges.hpp"

namespace pymemb
{
    struct reset_vertex_external_forces
    {
        HE_VertexProp operator()(HE_VertexProp vertex)
        {
            HE_VertexProp v = vertex;
            v.forceE.x = 0.0;
            v.forceE.y = 0.0;
            v.forceE.z = 0.0;
            return v;
        }
    };

    struct reset_vertex_forces
    {

        HE_VertexProp operator()(HE_VertexProp vertex)
        {

            HE_VertexProp v = vertex;
            v.energy = 0.0;
            v.forceC.x = (real)0.0;
            v.forceC.y = (real)0.0;
            v.forceC.z = (real)0.0;
            v.forceD.x = (real)0.0;
            v.forceD.y = (real)0.0;
            v.forceD.z = (real)0.0;
            return v;
        }
    };

    struct reset_vertex_velocities
    {
        HE_VertexProp operator()(HE_VertexProp vertex)
        {

            HE_VertexProp v = vertex;

            v.v.x = 0.0;
            v.v.y = 0.0;
            v.v.z = 0.0;

            return v;
        }
    };

    struct reset_vertex_acceleration
    {

        HE_VertexProp operator()(HE_VertexProp vertex)
        {

            HE_VertexProp v = vertex;
            v.accel.x = 0.0;
            v.accel.y = 0.0;
            v.accel.z = 0.0;
            return v;
        }
    };

    struct reset_vertex_energy
    {

        HE_VertexProp operator()(HE_VertexProp vertex)
        {

            HE_VertexProp v = vertex;
            v.energy = 0.0;
            return v;
        }
    };


    struct reset_face_energy
    {

        HE_FaceProp operator()(HE_FaceProp face)
        {

            HE_FaceProp f = face;
            f.energy = 0.0;
            return f;
        }
    };

    struct reset_edge_energy
    {

        HE_EdgeProp operator()(HE_EdgeProp edge)
        {
            HE_EdgeProp e = edge;
            e.energy = 0.0;
            return e;
        }
    };

    template <class T>
    struct reset_tensor
    {
        T operator()(const T &tt)
        {
            T res = tt;
            res.xx = 0.0;
            res.xy = 0.0;
            res.xz = 0.0;
            res.yx = 0.0;
            res.yy = 0.0;
            res.yz = 0.0;
            res.zx = 0.0;
            res.zy = 0.0;
            res.zz = 0.0;
            return res;
        }
    };

    template <class T>
    struct reduce_tensor
    {
        T operator()(const T &a, const T &b) const
        {
            T res = a;
            res.xx += b.xx;
            res.xy += b.xy;
            res.xz += b.xz;
            res.yx += b.yx;
            res.yy += b.yy;
            res.yz += b.yz;
            res.zx += b.zx;
            res.zy += b.zy;
            res.zz += b.zz;
            return res;
        }
    };
} // namespace device
#endif
