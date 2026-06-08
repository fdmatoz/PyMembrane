from __future__ import annotations

from dataclasses import dataclass, field

from pymembrane.dump.dumper import _write_legacy_vtk_mesh, _write_obj_mesh


@dataclass
class Vec:
    x: float
    y: float
    z: float


@dataclass
class Vertex:
    r: Vec
    id: int
    type: int = 0
    energy: float = 0.0
    forceC: Vec = field(default_factory=lambda: Vec(0.0, 0.0, 0.0))
    v: Vec = field(default_factory=lambda: Vec(0.0, 0.0, 0.0))
    normal: Vec = field(default_factory=lambda: Vec(0.0, 0.0, 1.0))
    n: Vec = field(default_factory=lambda: Vec(0.0, 0.0, 1.0))


@dataclass
class Face:
    v1: int
    v2: int
    v3: int
    id: int = 0
    type: int = 0
    energy: float = 0.0


def test_lightweight_dumpers_write_vtk_and_obj(tmp_path):
    vertices = [
        Vertex(Vec(0.0, 0.0, 0.0), id=0, type=1),
        Vertex(Vec(1.0, 0.0, 0.0), id=1, type=1),
        Vertex(Vec(0.0, 1.0, 0.0), id=2, type=2),
    ]
    faces = [Face(0, 1, 2, id=7, type=3)]

    vtk_path = _write_legacy_vtk_mesh(tmp_path / "test_mesh", vertices, faces)
    obj_path = _write_obj_mesh(tmp_path / "test_mesh", vertices, faces)

    assert vtk_path.exists()
    assert obj_path.exists()

    vtk_text = vtk_path.read_text(encoding="utf-8")
    assert "# vtk DataFile Version" in vtk_text
    assert "DATASET POLYDATA" in vtk_text
    assert "POINTS 3" in vtk_text
    assert "POLYGONS 1 4" in vtk_text

    obj_text = obj_path.read_text(encoding="utf-8")
    assert "v 0.0 0.0 0.0" in obj_text
    assert "v 1.0 0.0 0.0" in obj_text
    assert "f 1 2 3" in obj_text
