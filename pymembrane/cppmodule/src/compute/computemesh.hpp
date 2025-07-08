#ifndef __computemesh_hpp__
#define __computemesh_hpp__

#include <memory>
#include <map>

#include "../types/globaltypes.hpp"
#include "../types/pymembvector.hpp"

class SystemClass; // forward declaration
class EvolverClass;

/**
 * @class ComputeMesh
 * @brief Class responsible for various mesh computations.
 *
 * This class offers a variety of mesh-related calculations, ranging from vertex normals to mesh energy and stresses.
 * It operates on a provided system of type `SystemClass`.
 */

class ComputeMesh
{
public:
    /**
     * @brief Constructor that initializes the compute mesh with a system.
     * @param system Reference to the system on which the computations will be made.
     */
    ComputeMesh(SystemClass &system) : _system(system) {}

    /**
     * @brief Computes the normals for each vertex in the mesh.
     * @param vertex_normal_angle_weight Flag to determine if the vertex normal should be angle weighted. Default is false.
     */
    void compute_vertex_normals(bool vertex_normal_angle_weight = false);

    /// Computes the normals for each face in the mesh.
    void compute_face_normals(void);

    /**
     * @brief Computes the length of each edge in the mesh.
     * @return A vector containing lengths for each edge.
     */
    pymemb::vector<real> compute_edge_lengths(void);

    /// Computes the Gaussian curvature at the vertex.
    real gaussiancurvature_vertex(const int &vertex_index);

    /// Computes the mean curvature at the vertex.
    real meancurvature_vertex(const int &vertex_index);
    /// Computes the Gaussian curvature for the mesh.
    pymemb::vector<real> gaussiancurvature(void);

    /// Computes the mean curvature for the mesh.
    pymemb::vector<real> meancurvature(void);

    std::map<std::string, pymemb::vector<real>> compute_mesh_curvature(void);
    /**
     * @brief Computes metrics related to each face in the mesh.
     * @return A vector containing metrics for each face.
     */
    pymemb::vector<pymemb::vector<real>> compute_face_metric(void);

    /// Computes the total volume of the mesh.
    real compute_mesh_volume(void);

    /**
     * @brief Computes the area of each face in the mesh.
     * @return A vector containing areas for each face.
     */
    pymemb::vector<real> compute_face_area(void);

    /// Computes the total surface area of the mesh.
    real compute_mesh_area(void);

    /// Computes the surface area for a given type of vertex
    real compute_area_typed(int type=1);

    /**
     * @brief Computes the area associated with each vertex in the mesh.
     * @return A vector containing areas for each vertex.
     */
    pymemb::vector<real> compute_vertex_area(void);

    /// @name Energy and Force Measures
    /// These functions compute various energy and force measures for the mesh.

    /**
     * @brief Computes the total energy of the mesh.
     * @param evolver Reference to the evolver object.
     * @return A map containing different energy components and their respective values.
     */
    std::map<std::string, real> compute_mesh_energy(EvolverClass &);

    /**
     * @brief Computes forces on each vertex in the mesh.
     * @param evolver Reference to the evolver object.
     * @return A vector containing force vectors for each vertex.
     */
    pymemb::vector<real3> compute_vertex_forces(EvolverClass &);

    /// @name Energy and Force Measures
    /// Provides methods to compute various energy, force, and stress-related properties for the mesh.
    /**
     * @brief Computes the stress tensors for the mesh.
     * @param evolver Reference to the evolver object.
     * @param flag see cpp for more detail.
     * @return Stress tensor for the mesh.
     */
    realTensor compute_stresses(EvolverClass &, const bool &);

    /**
     * @brief Retrieves the stress tensors for the mesh.
     * @param evolver Reference to the evolver object.
     * @param flag see cpp for more detail.
     * @return Stress tensor for the mesh.
     */
    realTensor get_stresses(EvolverClass &, const bool &);

    /**
     * @brief Computes the pressure for the mesh using the provided evolver.
     * @param evolver Reference to the evolver object.
     * @return Vector containing pressure values.
     */
    std::vector<real> compute_pressure(EvolverClass &);

    /**
     * @brief Computes the kinetic energy tensor for the mesh.
     * @return Kinetic energy tensor for the mesh.
     */
    realTensor compute_kinetic_energy_tensor(void);
    /**
     * @brief Computes the total kinetic energy of the mesh.
     * @return Total kinetic energy of the mesh.
     */
    real compute_kinetic_energy(void);

    /**
     * @brief Computes the temperature based on the current state of the mesh.
     * @return Temperature of the mesh.
     */
    real compute_temperature(void);

    /**
     * @brief Computes the virial stresses for the mesh.
     * @param evolver Reference to the evolver object guiding the mesh's evolution.
     * @param computeForAllAtoms Flag to determine whether to compute stresses for all atoms or a subset. Replace with actual flag description.
     * @return Virial stress tensor for the mesh.
     */
    realTensor compute_stresses_virial(EvolverClass &evolver, const bool &computeForAllAtoms);

    /**
     * @brief Computes the stresses for each atom in the mesh.
     * @param evolver Reference to the evolver object guiding the mesh's evolution.
     * @param computeForAllAtoms Flag to determine whether to compute stresses for all atoms or a subset. Replace with actual flag description.
     * @return Vector of stress tensors, one for each atom.
     */
    std::vector<realTensor> compute_stresses_atom(EvolverClass &evolver, const bool &computeForAllAtoms);

private:
    SystemClass &_system;
};

#endif
