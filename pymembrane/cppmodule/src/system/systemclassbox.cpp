#include "systemclass.hpp"
#include "../box/pbc.hpp"

/**
 * @brief get the box
 * @param void
 * @return const BoxType&
 */
const BoxType &SystemClass::get_box(void) { return _box; }
/**
 * @brief Set the box object
 *
 * @param box
 */
void SystemClass::set_box(const BoxType &box) { _box = box; }
/**
 * @brief set the the box
 * @param double
 */
void SystemClass::set_box_length(const double &Lx, const double &Ly, const double &Lz)
{
    _box.L.x = Lx;
    _box.L.y = Ly;
    _box.L.z = Lz;
    _box.Llo.x = -0.5 * _box.L.x;
    _box.Lhi.x = 0.5 * _box.L.x;
    _box.Llo.y = -0.5 * _box.L.y;
    _box.Lhi.y = 0.5 * _box.L.y;
    _box.Llo.z = -0.5 * _box.L.z;
    _box.Lhi.z = 0.5 * _box.L.z;
}

void SystemClass::enforce_periodic_boundary_conditions(void)
{
    // Mesh
    for (int vindex = 0;
         vindex < Numvertices;
         vindex++)
    {
        pymemb::enforce_periodic(vertices[vindex].r, vertices[vindex].ip, _box);
    }
}

real3 SystemClass::minImage(const real3 &a, const real3 &b)
{
    return (pymemb::minimum_image(a, b, _box));
}

bool SystemClass::need_wrapping(const real3 &a, const real3 &b)
{
    return (pymemb::need_wrapping(a, b, _box));
}