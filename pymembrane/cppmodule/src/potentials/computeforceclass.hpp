#ifndef __computeforceclass_hpp__
#define __computeforceclass_hpp__

/** @defgroup computeenergy Compute Vertex Energy
 *  @brief ComputeForceClass abstracts definitions
 *  @{
 */
#include <memory>
#include <map>
#include <iostream>

#include "../utils/fromdicttovec.hpp"
#include "../utils/fromstring.hpp"
#include "../types/globaltypes.hpp"
#include "../system/systemclass.hpp"
#include <pybind11/pybind11.h>
/**
 * @class ComputeForceClass
 * @brief ComputeForceClass abstract class for compute different potentials, forces and torques
 */

constexpr double BIG_VERTEX_ENERGY_LIMIT = 1e15;

class ComputeForceClass
{
public:
    /**
     * @brief ComputeForceClass constructor
     * @param SystemClass reference to the system
     */
    ComputeForceClass(SystemClass &system) : _system(system),
                                             NUM_TYPES_ALLOWED(10), 
                                             NUM_TYPES_PAIR(NUM_TYPES_ALLOWED * NUM_TYPES_ALLOWED + 1)
    {
    }
    /**
     * @brief ComputeForceClass Destructor
     */
    virtual ~ComputeForceClass() {}
    /**
     * @brief compute energy for the actual configuration
     * @param void
     * @return void
     */
    virtual void compute_energy(void) {}
    /**
     * @brief compute force for the actual configuration
     * @param void
     * @return void
     */
    virtual void compute(void) {}
    /**
     * @brief compute the energy in a given edge
     * @param edge
     * @return real
     */
    virtual real compute_edge_energy(int) { return 0.0; }
    /**
     * @brief compute the energy in a given face
     * @param face
     * @return real
     */
    virtual real compute_face_energy(int) { return 0.0; }
    /**
     * @brief compute the energy in a vertex edge
     * @param face
     * @return real
     */
    virtual real compute_vertex_energy(int) { return 0.0; };

    /**
     * @brief compute force for the actual configuration
     * @param void
     * @return void
     */
    virtual void compute_stress(void){};

    /**
     * @brief compute force for the actual configuration
     * @param void
     * @return void
     */
    virtual void compute_atomic_stress(void){};

    /**
     * @brief Get the name object
     * @return std::string
     */
    std::string get_name(void) { return m_name; }
    /**
     * @brief Get the type object
     * @return std::string
     */
    std::string get_type(void) { return m_type; }
    /**
     * @brief Get the type object
     * @return std::string
     */
    virtual std::map<std::string, std::string> get_info(void) = 0;
    /**
     * @brief Set property
     */
    virtual void set_default_properties(void) = 0;
    virtual void set_property(std::map<std::string, std::map<std::pair<std::string, std::string>, std::string>> &region_map) { this->print_warning_calling("map<string, map<pair<string, string>, string>> "); };
    virtual void set_property(std::map<std::string, std::map<std::string, std::string>> &region_map) { this->print_warning_calling("map<string, map<string, string>>"); };
    virtual void set_property(std::map<std::string, std::string> &region_map) { this->print_warning_calling("std::map<std::string, std::string>"); }
    void print_warning_calling(const std::string &message) { pybind11::print("potential ", m_name, " cannot be called with ", message); }
    void print_warning_property_name(const std::string &message) { pybind11::print("parameter ", message, " is not part of ", m_name, " potential"); }

protected:
    SystemClass &_system; //!< Reference to the system
    std::string m_name;   //!< Name declared for that potential
    std::string m_type;   //!< Potential type, active, torque, conservative, etc
    int NUM_TYPES_ALLOWED;
    int NUM_TYPES_PAIR;
};

typedef std::unique_ptr<ComputeForceClass> ComputeForceClass_ptr;
typedef std::map<std::string, ComputeForceClass_ptr> VertexCompute;

#endif
