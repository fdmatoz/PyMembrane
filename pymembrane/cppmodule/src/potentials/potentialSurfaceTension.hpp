/*!
* @file potentialSurfaceTension.hpp
* @brief ComputeVertexSurfaceTension class
* @author Martin Leocmach, martin.leocmach@gmail.com
* @date 11-March-2025
*/

#ifndef __POTENTIALSURFACETENSION
#define __POTENTIALSURFACETENSION

/** @addtogroup computeenergy Compute surface tension energy
 *  @brief ComputeVertexSurfaceTension definitions 
 */

#include "computeforceclass.hpp"

#include "../system/systemclass.hpp"
#include "../mesh/computegeometry.hpp"

/**
 * @class ComputeVertexSurfaceTension
 * @brief Implements the surface tension to add to vertex energy
 *
 *
 */

class ComputeVertexSurfaceTension : public ComputeForceClass
{
public:
    ComputeVertexSurfaceTension(SystemClass &system) : ComputeForceClass(system)
    {
        m_name = "Surface Tension";
        m_type = "vertex";
        this->set_default_properties();
    }

    ~ComputeVertexSurfaceTension() {}

    void set_default_properties(void)override
    {
        std::vector<double> _gamma(NUM_TYPES_ALLOWED, 0.0);
        m_gamma = _gamma;
    }

    using ComputeForceClass::set_property;
    void set_property(std::map<std::string, std::map<std::string, std::string>> &region_map) override
    {
        for(const auto &item : region_map)
        {
            if (item.first.compare("gamma") == 0)
            {
                pymemb::vector<real> _gamma = util::from_dict_to_vector_types(pymemb::copy(m_gamma), item.second);
                m_gamma = _gamma;
            }
            else
                this->print_warning_property_name(item.first);
        }
    }

    // Is second set_property overload necessary here?
    // if yes TODO

    std::map<std::string, std::string> get_info(void) override
    {
        std::map<std::string, std::string> value;
        value["name"] = m_name;
        value["type"] = m_type;
        value["gamma"] = util::to_string_vec(m_gamma);
        return value;
    }

    void compute_energy(void) override;
    // if necessary TODO : add compute_edge_energy
    real compute_face_energy(int) override;
    void compute(void) override;

private:
    pymemb::vector<real> m_gamma; //!< list of tension coefficient, one per type
};


#endif