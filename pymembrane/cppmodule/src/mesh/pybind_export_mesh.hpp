#ifndef __pybind_export_mesh_hpp__
#define __pybind_export_mesh_hpp__

//#include "mesh.hpp"
#include "meshproperties.hpp"
//#include "material.hpp"
#include "computegeometry.hpp"

void export_PropertyFaces(py::module &m)
{
    py::class_<PropertyFaces>(m, "PropertyFaces")
        .def_readwrite("age", &PropertyFaces::age, "Material age")
        .def_readwrite("k11", &PropertyFaces::k11, "Material growth in the 11 <<metric>> direction")
        .def_readwrite("k12", &PropertyFaces::k12, "Material growth in the 12 <<metric>> direction")
        .def_readwrite("k22", &PropertyFaces::k22, "Material growth in the 22 <<metric>> direction");
}

void export_PropertyFaces_Vector(py::module &m)
{
    py::class_<std::vector<PropertyFaces>>(m, "PropertyFacesVector")
        .def(py::init<>())
        .def("clear", &std::vector<PropertyFaces>::clear)
        .def("pop_back", &std::vector<PropertyFaces>::pop_back)
        .def("append", (void (std::vector<PropertyFaces>::*)(const PropertyFaces &)) & std::vector<PropertyFaces>::push_back)
        .def("__len__", [](const std::vector<PropertyFaces> &v) { return v.size(); })
        .def("__iter__", [](std::vector<PropertyFaces> &v) { return py::make_iterator(v.begin(), v.end()); }, py::keep_alive<0, 1>())
        .def("__getitem__", [](const std::vector<PropertyFaces> &v, size_t i) { if (i >= v.size()) throw py::index_error(); return v[i]; })
        .def("__setitem__", [](std::vector<PropertyFaces> &v, size_t i, PropertyFaces &d) { if (i >= v.size()) throw py::index_error(); v[i] = d; });
}

void export_PropertyVertices(py::module &m)
{
    py::class_<PropertyVertices>(m, "PropertyVertices")
        .def_readonly("v",      &PropertyVertices::v, "Vertex velocity")
        .def_readwrite("age",   &PropertyVertices::age, "Material age")
        .def_readonly("radius", &PropertyVertices::radius, "Vertex radius");
}

void export_PropertyVertices_Vector(py::module &m)
{
    py::class_<std::vector<PropertyVertices>>(m, "PropertyVerticesVector")
        .def(py::init<>())
        .def("clear", &std::vector<PropertyVertices>::clear)
        .def("pop_back", &std::vector<PropertyVertices>::pop_back)
        .def("append", (void (std::vector<PropertyVertices>::*)(const PropertyVertices &)) & std::vector<PropertyVertices>::push_back)
        .def("__len__", [](const std::vector<PropertyVertices> &v) { return v.size(); })
        .def("__iter__", [](std::vector<PropertyVertices> &v) { return py::make_iterator(v.begin(), v.end()); }, py::keep_alive<0, 1>())
        .def("__getitem__", [](const std::vector<PropertyVertices> &v, size_t i) { if (i >= v.size()) throw py::index_error(); return v[i]; })
        .def("__setitem__", [](std::vector<PropertyVertices> &v, size_t i, PropertyVertices &d) { if (i >= v.size()) throw py::index_error(); v[i] = d; });
}

void export_PropertyEdges(py::module &m)
{
    py::class_<PropertyEdges>(m, "PropertyEdges")
        .def_readwrite("age",      &PropertyEdges::age, "Material age")
        .def_readonly("cot_alpha", &PropertyEdges::cot_alpha, "opposite cotangent angle of \vec{r0}-\vec{r1} and \vec{r2}-\vec{r1}")
        .def_readonly("cot_beta",  &PropertyEdges::cot_beta, "opposite cotangent angle of \vec{r0}-\vec{r3} and \vec{r2}-\vec{r3}");
}

void export_PropertyEdges_Vector(py::module &m)
{
    py::class_<std::vector<PropertyEdges>>(m, "PropertyEdgesVector")
        .def(py::init<>())
        .def("clear", &std::vector<PropertyEdges>::clear)
        .def("pop_back", &std::vector<PropertyEdges>::pop_back)
        .def("append", (void (std::vector<PropertyEdges>::*)(const PropertyEdges &)) & std::vector<PropertyEdges>::push_back)
        .def("__len__", [](const std::vector<PropertyEdges> &v) { return v.size(); })
        .def("__iter__", [](std::vector<PropertyEdges> &v) { return py::make_iterator(v.begin(), v.end()); }, py::keep_alive<0, 1>())
        .def("__getitem__", [](const std::vector<PropertyEdges> &v, size_t i) { if (i >= v.size()) throw py::index_error(); return v[i]; })
        .def("__setitem__", [](std::vector<PropertyEdges> &v, size_t i, PropertyEdges &d) { if (i >= v.size()) throw py::index_error(); v[i] = d; });
}

void export_HE_Vertex(py::module &m)
{
    py::class_<HE_VertexProp>(m, "Vertex")
        .def_readwrite("id", &HE_VertexProp::id, "Vertex id")
        .def_readwrite("r", &HE_VertexProp::r, "Vertex position")
        .def_readonly("v", &HE_VertexProp::v, "Vertex velocity")
        .def_readwrite("boundary", &HE_VertexProp::boundary, "Vertex True if it is at the boundary")
        .def_readwrite("coordination", &HE_VertexProp::coordination, "Vertex Coordination")
        .def_readwrite("normal", &HE_VertexProp::normal, "normal")
        .def_readonly("_hedge", &HE_VertexProp::_hedge, "halfedge index")
        .def_readonly("forceC", &HE_VertexProp::forceC, "Conservative Force")
        .def_readonly("forceD", &HE_VertexProp::forceD, "Dissipative Force")
        .def_readonly("energy", &HE_VertexProp::energy, "Conservative Energy")
        .def_readwrite("mass", &HE_VertexProp::mass, "mass")
        .def_readwrite("type", &HE_VertexProp::type, "type")
        .def_readonly("energy", &HE_VertexProp::energy, "Conservative Energy")
        .def_readwrite("property", &HE_VertexProp::_property, "Properties");
}

void export_HE_Vertex_Vector(py::module &m)
{
    py::class_<std::vector<HE_VertexProp>>(m, "VertexVector")
        .def(py::init<>())
        .def("clear", &std::vector<HE_VertexProp>::clear)
        .def("pop_back", &std::vector<HE_VertexProp>::pop_back)
        .def("append", (void (std::vector<HE_VertexProp>::*)(const HE_VertexProp &)) & std::vector<HE_VertexProp>::push_back)
        .def("__len__", [](const std::vector<HE_VertexProp> &v) { return v.size(); })
        .def("__iter__", [](std::vector<HE_VertexProp> &v) { return py::make_iterator(v.begin(), v.end()); }, py::keep_alive<0, 1>())
        .def("__getitem__", [](const std::vector<HE_VertexProp> &v, size_t i) { if (i >= v.size()) throw py::index_error(); return v[i]; })
        .def("__setitem__", [](std::vector<HE_VertexProp> &v, size_t i, HE_VertexProp &d) { if (i >= v.size()) throw py::index_error(); v[i] = d; });
}

void export_HE_Edge(py::module &m)
{
    py::class_<HE_EdgeProp>(m, "Edge")
        .def_readonly("id", &HE_EdgeProp::id, "id")
        .def_readonly("i", &HE_EdgeProp::i, "one of the conected vertices")
        .def_readonly("j", &HE_EdgeProp::j, "one of the conected vertices")
        .def_readonly("boundary", &HE_EdgeProp::boundary, "True if it is at the boundary")
        .def_readonly("face_k", &HE_EdgeProp::face_k, "index to one of the faces shared by this edge -1 if is outer face") //!< index to one of the faces shared by this edge -1 if is outer face
        .def_readonly("face_l", &HE_EdgeProp::face_l, "index to one of the faces shared by this edge -1 if is outer face") //!< index to one of the faces shared by this edge -1 if is outer face
        .def_readonly("v0", &HE_EdgeProp::v0, "vertex around the edge")
        .def_readonly("v1", &HE_EdgeProp::v1, "vertex around the edge")
        .def_readonly("v2", &HE_EdgeProp::v2, "vertex around the edge")
        .def_readonly("v3", &HE_EdgeProp::v3, "vertex around the edge")          //!< indices to the vertices around that edge
        .def_readonly("_hedge", &HE_EdgeProp::_hedge, "halfedge index")
        .def_readwrite("type", &HE_EdgeProp::type, "type")
        .def_readonly("energy", &HE_EdgeProp::energy, "Conservative Energy")
        .def_readwrite("property", &HE_EdgeProp::_property, "Properties");
}

void export_HE_Edge_Vector(py::module &m)
{
    py::class_<std::vector<HE_EdgeProp>>(m, "EdgeVector")
        .def(py::init<>())
        .def("clear", &std::vector<HE_EdgeProp>::clear)
        .def("pop_back", &std::vector<HE_EdgeProp>::pop_back)
        .def("append", (void (std::vector<HE_EdgeProp>::*)(const HE_EdgeProp &)) & std::vector<HE_EdgeProp>::push_back)
        .def("__len__", [](const std::vector<HE_EdgeProp> &v) { return v.size(); })
        .def("__iter__", [](std::vector<HE_EdgeProp> &v) { return py::make_iterator(v.begin(), v.end()); }, py::keep_alive<0, 1>())
        .def("__getitem__", [](const std::vector<HE_EdgeProp> &v, size_t i) { if (i >= v.size()) throw py::index_error(); return v[i]; })
        .def("__setitem__", [](std::vector<HE_EdgeProp> &v, size_t i, HE_EdgeProp &d) { if (i >= v.size()) throw py::index_error(); v[i] = d; });
}

void export_HE_Face(py::module &m)
{
    py::class_<HE_FaceProp>(m, "Face")
        .def_readwrite("id", &HE_FaceProp::id, "face id")                             //!< face id
        .def_readwrite("outer", &HE_FaceProp::outer, "if True, face is a ghost outer face")                       //!< if true, face is a ghost outer face
        .def_readwrite("nsides", &HE_FaceProp::nsides, "number of sides face has")                     //!< number of sides face has
        .def_readwrite("normal", &HE_FaceProp::normal, "normal to that face")                     //!< normal to that face
        .def_readwrite("reference_normal", &HE_FaceProp::normal_reference, "reference normal to that face") //!< normal to that face
        .def_readwrite("area", &HE_FaceProp::area, "area of the face")                         //!< area of the triangle
        .def_readwrite("boundary", &HE_FaceProp::boundary, "True if it is at the boundary")
        .def("refmetric", [](HE_FaceProp &face) 
        { 
            std::vector<real> metric; 
            metric.push_back(face.g_reference[0]); 
            metric.push_back(face.g_reference[1]); 
            metric.push_back(face.g_reference[2]); 
            return metric; 
        }, "get the reference metric")
        .def("set_refmetric", [](HE_FaceProp &face, std::vector<real> metric) {
            if (metric.size() < 3)
                py::print( "error metric must be [g11, g12, g22]\n");
            else
            {
                face.g_reference[0] = metric[0];
                face.g_reference[1] = metric[1];
                face.g_reference[2] = metric[2];
                real det = face.g_reference[0]*face.g_reference[2] - face.g_reference[1]*face.g_reference[1];
                face.g_reference_inv[0] = face.g_reference[2]/det;
                face.g_reference_inv[1] = -face.g_reference[1]/det;
                face.g_reference_inv[2] = face.g_reference[0]/det;
            }
        }, "set the reference metric to [g11, g12, g22]")
        .def("set_refmetric", [](HE_FaceProp &face, const std::vector<real3>& vertices) {
            if (vertices.size() < 3)
                py::print(  "error vertices must be [v1, v2, v3]\n");
            else
            {
                real metric[3];
                pymemb::compute_form_factor_triangle(metric, vertices[0], vertices[1], vertices[2]);
                face.g_reference[0] = metric[0];
                face.g_reference[1] = metric[1];
                face.g_reference[2] = metric[2];
                real det = face.g_reference[0]*face.g_reference[2] - face.g_reference[1]*face.g_reference[1];
                face.g_reference_inv[0] = face.g_reference[2]/det;
                face.g_reference_inv[1] = -face.g_reference[1]/det;
                face.g_reference_inv[2] = face.g_reference[0]/det;
            }
        }, "set the reference metric to [v1, v2, v3] vertices with v1 being real3")
        .def("set_refmetric", [](HE_FaceProp &face, const std::vector<real3>& vertices, const BoxType _box) {
            if (vertices.size() < 3)
                py::print( "error vertices must be [v1, v2, v3]\n");
            else
            {
                real metric[3];
                pymemb::compute_form_factor_triangle(metric, vertices[0], vertices[1], vertices[2], _box);
                face.g_reference[0] = metric[0];
                face.g_reference[1] = metric[1];
                face.g_reference[2] = metric[2];
                real det = face.g_reference[0]*face.g_reference[2] - face.g_reference[1]*face.g_reference[1];
                face.g_reference_inv[0] = face.g_reference[2]/det;
                face.g_reference_inv[1] = -face.g_reference[1]/det;
                face.g_reference_inv[2] = face.g_reference[0]/det;
            }
        }, "set the reference metric to [v1, v2, v3] vertices with v1 being real3 and a Box considering periodic boundary conditions")
        .def("metric", [](HE_FaceProp &face, const std::vector<real3>& vertices) {
            std::vector<real> metric(3, -1.0);
            if (vertices.size() < 3)
                py::print(  "error vertices must be [v1, v2, v3]\n");
            else
            {
                pymemb::compute_form_factor_triangle(&metric[0], vertices[0], vertices[1], vertices[2]);
            }
            return metric;
        }, "get the metric")
        .def_readwrite("v1", &HE_FaceProp::v1, "one of vertices that define the face")
        .def_readwrite("v2", &HE_FaceProp::v2, "one of vertices that define the face")
        .def_readwrite("v3", &HE_FaceProp::v3, "one of vertices that define the face")
        .def_readonly("_hedge", &HE_FaceProp::_hedge, "one of the half-edges bordering the face") //!< HANDLE INDEX OF: one of the half-edges bordering the face
        .def_readwrite("type", &HE_FaceProp::type, "types")
        .def_readonly("energy", &HE_FaceProp::energy, "Conservative Energy")
        .def_readwrite("property", &HE_FaceProp::_property, "properties");
}

void export_HE_Face_Vector(py::module &m)
{
    py::class_<std::vector<HE_FaceProp>>(m, "FaceVector")
        .def(py::init<>())
        .def("clear", &std::vector<HE_FaceProp>::clear)
        .def("pop_back", &std::vector<HE_FaceProp>::pop_back)
        .def("append", (void (std::vector<HE_FaceProp>::*)(const HE_FaceProp &)) & std::vector<HE_FaceProp>::push_back)
        .def("__len__", [](const std::vector<HE_FaceProp> &v) { return v.size(); })
        .def("__iter__", [](std::vector<HE_FaceProp> &v) { return py::make_iterator(v.begin(), v.end()); }, py::keep_alive<0, 1>())
        .def("__getitem__", [](const std::vector<HE_FaceProp> &v, size_t i) { if (i >= v.size()) throw py::index_error(); return v[i]; })
        .def("__setitem__", [](std::vector<HE_FaceProp> &v, size_t i, HE_FaceProp &d) { if (i >= v.size()) throw py::index_error(); v[i] = d; });
}

void export_HE_HalfEdge(py::module &m)
{
    py::class_<HE_HalfEdgeProp>(m, "HalfEdge")
        .def_readonly("index", &HE_HalfEdgeProp::index, "vertex at the beginning of the half-edge")          //!< HANDLE INDEX OF: vertex at the beginning of the half-edge
        .def_readwrite("vert_from", &HE_HalfEdgeProp::vert_from, "vertex at the beginning of the half-edge") //!< HANDLE INDEX OF: vertex at the beginning of the half-edge
        .def_readwrite("vert_to", &HE_HalfEdgeProp::vert_to, "vertex at the end of the half-edge")     //!< HANDLE INDEX OF: vertex at the end of the half-edge
        .def_readwrite("edge", &HE_HalfEdgeProp::edge, "edge this he is part of")           //!< HANDLE INDEX OF: edge this he is part of
        .def_readwrite("face", &HE_HalfEdgeProp::face, "face the half-edge borders")           //!< HANDLE INDEX OF: face the half-edge borders
        .def_readwrite("pair", &HE_HalfEdgeProp::pair, "oppositely oriented adjacent half-edge")           //!< HANDLE INDEX OF: oppositely oriented adjacent half-edge
        .def_readwrite("next", &HE_HalfEdgeProp::next, "next half-edge around the face")           //!< HANDLE INDEX OF: next half-edge around the face
        .def_readwrite("prev", &HE_HalfEdgeProp::prev, "previous half-edge around the face")           //!< HANDLE INDEX OF: previous half-edge around the face
        .def_readwrite("boundary", &HE_HalfEdgeProp::boundary, " if true, the halfedge is a boundary");   //!< if true, the halfedge is a boundary
        //.def_readwrite("property", &HE_HalfEdgeProp::_property);
}

void export_HE_HalfEdge_Vector(py::module &m)
{
    py::class_<std::vector<HE_HalfEdgeProp>>(m, "HalfEdgeVector")
        .def(py::init<>())
        .def("clear", &std::vector<HE_HalfEdgeProp>::clear)
        .def("pop_back", &std::vector<HE_HalfEdgeProp>::pop_back)
        .def("append", (void (std::vector<HE_HalfEdgeProp>::*)(const HE_HalfEdgeProp &)) & std::vector<HE_HalfEdgeProp>::push_back)
        .def("__len__", [](const std::vector<HE_HalfEdgeProp> &v) { return v.size(); })
        .def("__iter__", [](std::vector<HE_HalfEdgeProp> &v) {
                return py::make_iterator(v.begin(), v.end());
            },
            py::keep_alive<0, 1>())
        .def("__getitem__", [](const std::vector<HE_HalfEdgeProp> &v, size_t i) {
            if (i >= v.size())
                throw py::index_error();
            return v[i];
        })
        .def("__setitem__", [](std::vector<HE_HalfEdgeProp> &v, size_t i, HE_HalfEdgeProp &d) {
            if (i >= v.size())
                throw py::index_error();
            v[i] = d;
        });
}

void export_ReadMesh(py::module &m)
{
    py::class_<ReadMesh>(m, "ReadMesh")
        .def(py::init<std::string &,
                      std::string &>(),
             py::arg().noconvert(), // no convert the arguments and
             py::arg().noconvert(),
             "Mesh reader, param1=faces file path, param2= vertices file path")
        .def(py::init<std::map<std::string, std::string>>(),
             "Mesh reader, faces: faces file path, vertices: vertices file path")
        .def("clear", &ReadMesh::clear,
             "Clear whole vectors in reading. Caution this clear the vectors in the readmesh<class> but not the mesh itself")
        .def("getVertices", &ReadMesh::get_vertices, py::return_value_policy::reference, "get the vertices")
        .def("getEdges", &ReadMesh::get_edges, py::return_value_policy::reference, "get the edges")
        .def("getFaces", &ReadMesh::get_faces, py::return_value_policy::reference, "get the faces")
        .def("getHalfedges", &ReadMesh::get_halfedges, py::return_value_policy::reference, "get the halfedges")
        .def("getNumVertices", &ReadMesh::getNumvertices, "get the number of vertices")
        .def("getNumEdges", &ReadMesh::getNumedges, "get the number of edges")
        .def("getNumFaces", &ReadMesh::getNumfaces, "get the number of faces")
        .def("getNumHalfedges", &ReadMesh::getNumhalfedges, "get the number of halfedges");
}

#endif
