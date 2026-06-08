# Copyright 2022 Daniel Matoz, University of Warsaw
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
# documentation files (the "Software"), to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
# and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or substantial portions
# of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
# TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
# CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.

from __future__ import annotations

import json
from pathlib import Path


_TENSOR_COMPONENTS = ("xx", "xy", "xz", "yx", "yy", "yz", "zx", "zy", "zz")


def _component(value, name, default=0.0):
    return getattr(value, name, default)


def _vector_tuple(value):
    if value is None:
        return (0.0, 0.0, 0.0)
    return (_component(value, "x"), _component(value, "y"), _component(value, "z"))


def _tensor_tuple(value):
    if value is None:
        return tuple(0.0 for _ in _TENSOR_COMPONENTS)
    return tuple(_component(value, name) for name in _TENSOR_COMPONENTS)


def _with_suffix(filename, suffix):
    path = Path(filename)
    if path.suffix == suffix:
        return path
    return path.with_name(path.name + suffix)


def _face_normal_and_area(p1, p2, p3):
    ax = p2[0] - p1[0]
    ay = p2[1] - p1[1]
    az = p2[2] - p1[2]
    bx = p3[0] - p1[0]
    by = p3[1] - p1[1]
    bz = p3[2] - p1[2]
    nx = ay * bz - az * by
    ny = az * bx - ax * bz
    nz = ax * by - ay * bx
    norm = (nx * nx + ny * ny + nz * nz) ** 0.5
    if norm <= 0.0:
        return (0.0, 0.0, 0.0), 0.0
    return (nx / norm, ny / norm, nz / norm), 0.5 * norm


def _coerce_rows(data):
    rows = []
    for value in data:
        if hasattr(value, "xx"):
            rows.append(_tensor_tuple(value))
        elif hasattr(value, "x") and hasattr(value, "y") and hasattr(value, "z"):
            rows.append(_vector_tuple(value))
        elif isinstance(value, (list, tuple)):
            rows.append(tuple(value))
        else:
            rows.append((value,))
    return rows


class Dumper:
    def __init__(self, system):
        self.system = system
        self._vtk_legacy = True
        self._vertex_user_data = {}
        self._face_user_data = {}
        self._edge_user_data = {}

    def setvtkLegacyFormat(self, flag=True):
        self._vtk_legacy = bool(flag)

    def _periodic_delta(self, ri, rj):
        box = self.system.box
        dx = _component(rj, "x") - _component(ri, "x")
        dy = _component(rj, "y") - _component(ri, "y")
        dz = _component(rj, "z") - _component(ri, "z")
        wrapped = False

        if bool(_component(box.periodic, "x")):
            if dx > _component(box.Lhi, "x"):
                dx -= _component(box.L, "x")
                wrapped = True
            elif dx < _component(box.Llo, "x"):
                dx += _component(box.L, "x")
                wrapped = True

        if bool(_component(box.periodic, "y")):
            if dy > _component(box.Lhi, "y"):
                dy -= _component(box.L, "y")
                wrapped = True
            elif dy < _component(box.Llo, "y"):
                dy += _component(box.L, "y")
                wrapped = True

        if bool(_component(box.periodic, "z")):
            if dz > _component(box.Lhi, "z"):
                dz -= _component(box.L, "z")
                wrapped = True
            elif dz < _component(box.Llo, "z"):
                dz += _component(box.L, "z")
                wrapped = True

        return (dx, dy, dz), wrapped

    def _mesh_data(self, periodic=False):
        vertices = list(self.system.vertices)
        faces = list(self.system.faces)
        points = [_vector_tuple(vertex.r) for vertex in vertices]
        point_sources = list(range(len(vertices)))
        vertex_type = [getattr(vertex, "type", 0) for vertex in vertices]
        polygons = []
        face_normals = []
        face_areas = []

        for face in faces:
            v1 = face.v1
            v2 = face.v2
            v3 = face.v3

            if periodic:
                # Match the old public signature. We only duplicate wrapped face
                # vertices needed to draw a face consistently across PBCs.
                p1 = points[v1]
                r1 = vertices[v1].r

                delta12, wrapped12 = self._periodic_delta(r1, vertices[v2].r)
                if wrapped12:
                    points.append((p1[0] + delta12[0], p1[1] + delta12[1], p1[2] + delta12[2]))
                    point_sources.append(face.v2)
                    vertex_type.append(-1)
                    v2 = len(points) - 1

                delta13, wrapped13 = self._periodic_delta(r1, vertices[v3].r)
                if wrapped13:
                    points.append((p1[0] + delta13[0], p1[1] + delta13[1], p1[2] + delta13[2]))
                    point_sources.append(face.v3)
                    vertex_type.append(-1)
                    v3 = len(points) - 1

            polygons.append((v1, v2, v3))
            normal, area = _face_normal_and_area(points[v1], points[v2], points[v3])
            face_normals.append(normal)
            face_areas.append(area)

        return {
            "vertices": vertices,
            "faces": faces,
            "points": points,
            "point_sources": point_sources,
            "polygons": polygons,
            "vertex_type": vertex_type,
            "face_normals": face_normals,
            "face_areas": face_areas,
        }

    def _write_scalar(self, out, name, dtype, values):
        out.write(f"SCALARS {name} {dtype} 1\n")
        out.write("LOOKUP_TABLE default\n")
        for value in values:
            out.write(f"{value}\n")

    def _write_vectors(self, out, name, values):
        out.write(f"VECTORS {name} double\n")
        for x, y, z in values:
            out.write(f"{x} {y} {z}\n")

    def _write_field(self, out, name, values):
        width = len(values[0])
        out.write(f"FIELD FieldData 1\n{name} {width} {len(values)} double\n")
        for row in values:
            out.write(" ".join(str(value) for value in row) + "\n")

    def _write_user_arrays(self, out, storage, indices=None):
        for name, rows in storage.items():
            values = rows if indices is None else [rows[index] for index in indices]
            if not values:
                continue
            width = len(values[0])
            if width == 1:
                self._write_scalar(out, name, "double", [row[0] for row in values])
            elif width == 3:
                self._write_vectors(out, name, values)
            else:
                self._write_field(out, name, values)

    def vtk(self, filename, periodic=False):
        data = self._mesh_data(periodic=periodic)
        path = _with_suffix(filename, ".vtk")

        with path.open("w", encoding="utf-8") as out:
            out.write("# vtk DataFile Version 3.0\n")
            out.write("PyMembrane mesh\n")
            out.write("ASCII\n")
            out.write("DATASET POLYDATA\n")
            out.write(f"POINTS {len(data['points'])} double\n")
            for point in data["points"]:
                out.write(f"{point[0]} {point[1]} {point[2]}\n")

            out.write(f"POLYGONS {len(data['polygons'])} {len(data['polygons']) * 4}\n")
            for v1, v2, v3 in data["polygons"]:
                out.write(f"3 {v1} {v2} {v3}\n")

            out.write(f"POINT_DATA {len(data['points'])}\n")
            self._write_scalar(out, "vertex_type", "int", data["vertex_type"])
            self._write_user_arrays(out, self._vertex_user_data, data["point_sources"])

            out.write(f"CELL_DATA {len(data['polygons'])}\n")
            self._write_scalar(out, "face_type", "int", [getattr(face, "type", 0) for face in data["faces"]])
            self._write_scalar(out, "face_area", "double", data["face_areas"])
            self._write_vectors(out, "face_normal", data["face_normals"])
            self._write_user_arrays(out, self._face_user_data)

        return path

    def edge_vtk(self, filename):
        vertices = list(self.system.vertices)
        edges = list(self.system.edges)
        path = _with_suffix(filename, ".vtk")

        with path.open("w", encoding="utf-8") as out:
            out.write("# vtk DataFile Version 3.0\n")
            out.write("PyMembrane edges\n")
            out.write("ASCII\n")
            out.write("DATASET POLYDATA\n")
            out.write(f"POINTS {len(vertices)} double\n")
            for vertex in vertices:
                x, y, z = _vector_tuple(vertex.r)
                out.write(f"{x} {y} {z}\n")

            out.write(f"LINES {len(edges)} {len(edges) * 3}\n")
            for edge in edges:
                out.write(f"2 {edge.i} {edge.j}\n")

            out.write(f"CELL_DATA {len(edges)}\n")
            self._write_scalar(out, "edge_type", "int", [getattr(edge, "type", 0) for edge in edges])
            self._write_user_arrays(out, self._edge_user_data)

        return path

    def json(self, filename, faces=True, vertices=True):
        path = _with_suffix(filename, ".json")
        payload = {}

        box = self.system.box
        payload["box"] = {
            "L": list(_vector_tuple(box.L)),
            "Lhi": list(_vector_tuple(box.Lhi)),
            "Llo": list(_vector_tuple(box.Llo)),
            "periodic": [bool(_component(box.periodic, axis)) for axis in ("x", "y", "z")],
        }

        if vertices:
            payload["vertices"] = [
                {
                    "id": getattr(vertex, "id", index),
                    "r": list(_vector_tuple(vertex.r)),
                    "type": getattr(vertex, "type", 0),
                    "boundary": bool(getattr(vertex, "boundary", False)),
                }
                for index, vertex in enumerate(self.system.vertices)
            ]

        if faces:
            payload["faces"] = [
                {
                    "id": getattr(face, "id", index),
                    "v": [face.v1, face.v2, face.v3],
                    "type": getattr(face, "type", 0),
                    "boundary": bool(getattr(face, "boundary", False)),
                }
                for index, face in enumerate(self.system.faces)
            ]

        with path.open("w", encoding="utf-8") as out:
            json.dump(payload, out, indent=2)

        return path

    def ply(self, filename):
        vertices = list(self.system.vertices)
        faces = list(self.system.faces)
        path = _with_suffix(filename, ".ply")

        with path.open("w", encoding="utf-8") as out:
            out.write("ply\n")
            out.write("format ascii 1.0\n")
            out.write(f"element vertex {len(vertices)}\n")
            out.write("property double x\n")
            out.write("property double y\n")
            out.write("property double z\n")
            out.write(f"element face {len(faces)}\n")
            out.write("property list uchar int vertex_indices\n")
            out.write("end_header\n")
            for vertex in vertices:
                x, y, z = _vector_tuple(vertex.r)
                out.write(f"{x} {y} {z}\n")
            for face in faces:
                out.write(f"3 {face.v1} {face.v2} {face.v3}\n")

        return path

    def txt(self, filename, faces=True, vertices=True):
        path = _with_suffix(filename, ".txt")

        with path.open("w", encoding="utf-8") as out:
            if vertices:
                out.write("# vertices\n")
                for index, vertex in enumerate(self.system.vertices):
                    x, y, z = _vector_tuple(vertex.r)
                    out.write(f"{index} {x} {y} {z}\n")
            if faces:
                out.write("# faces\n")
                for index, face in enumerate(self.system.faces):
                    out.write(f"{index} {face.v1} {face.v2} {face.v3}\n")

        return path

    def obj(self, filename):
        vertices = list(self.system.vertices)
        faces = list(self.system.faces)
        path = _with_suffix(filename, ".obj")

        with path.open("w", encoding="utf-8") as out:
            out.write("# PyMembrane Wavefront OBJ\n")
            for vertex in vertices:
                x, y, z = _vector_tuple(vertex.r)
                out.write(f"v {x} {y} {z}\n")
            for face in faces:
                out.write(f"f {face.v1 + 1} {face.v2 + 1} {face.v3 + 1}\n")

        return path

    def _write_user_data(self, filename, data, expected_size, storage):
        rows = _coerce_rows(data)
        if len(rows) != expected_size:
            raise ValueError(f"Expected {expected_size} rows, got {len(rows)}")

        path = _with_suffix(filename, ".txt")
        storage[Path(filename).stem] = rows

        with path.open("w", encoding="utf-8") as out:
            for index, row in enumerate(rows):
                values = " ".join(str(value) for value in row)
                out.write(f"{index} {values}\n")

        return path

    def user_data_vertex(self, filename, data):
        return self._write_user_data(filename, data, self.system.Numvertices, self._vertex_user_data)

    def user_data_face(self, filename, data):
        return self._write_user_data(filename, data, self.system.Numfaces, self._face_user_data)

    def user_data_edge(self, filename, data):
        return self._write_user_data(filename, data, self.system.Numedges, self._edge_user_data)
