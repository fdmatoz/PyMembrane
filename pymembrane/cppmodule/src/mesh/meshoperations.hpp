 /*! 
 *  @details   This code will be part of an active membrane code.
 *  @author    Dr Daniel Alejandro Matoz Fernandez(fdamatoz@gmail.com).
 *  @date      2022-
 *  @copyright See the file "LICENSE" for the full license governing this code.
 */
#ifndef __meshoperations_hpp__
#define __meshoperations_hpp__

#include "../types/globaltypes.hpp"


class SystemClass;

class MeshOperations 
{
public:
    MeshOperations(SystemClass &system) : _system(system)
    {
    }
    ~MeshOperations(void) {}

    bool check_if_edge_can_flip(const int& flip_edge_index);
    void edge_flip(const int& flip_edge_index, const bool& flip_face_up);
    void equiangulation(void);
private:
    SystemClass& _system;

};

#endif
/*! @} */
