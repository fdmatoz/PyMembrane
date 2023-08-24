#include "dumper.hpp"

#include <memory>
#include "../mesh/computegeometry.hpp"
#include "../mesh/halfedges.hpp"
#include "../types/globaltypes.hpp"
#include "../types/pymembvector.hpp"
#include "../system/systemclass.hpp"
#include "../utils/tofile.hpp"
#include "../utils/fromstring.hpp"

void DumperClass::mesh_json(const std::string &file_name, const bool &ffaces, const bool &fvertices)
{
}

void DumperClass::mesh_txt(const std::string &file_name, const bool &ffaces, const bool &fvertices)
{
    if (ffaces)
    {
        std::ofstream file(file_name + "_faces.dat", std::ios::out);
        if (file.good())
        {
            for (const auto& face : _system.faces)
            {
                util::writeToFile(file, face.id, face.v1, face.v2, face.v3, 1, face.type);
                file << "\n";
            }
        }
        else
        {
            py::print("Error no faces file is provided\n");
        }
    }

    if (fvertices)
    {
        std::ofstream file(file_name + "_vertices.dat", std::ios::out);
        if (file.good())
        {
            for (const auto& vertex : _system.vertices)
            {
                util::writeToFile(file, 
                                  vertex.id, 
                                  util::to_string_with_precision(vertex.r.x, 12), 
                                  util::to_string_with_precision(vertex.r.y, 12), 
                                  util::to_string_with_precision(vertex.r.z, 12), 
                                  vertex.type);
                file << "\n";
            }
        }
        else
        {
            py::print("Error no vertices file is provided\n");
        }
    }
}
