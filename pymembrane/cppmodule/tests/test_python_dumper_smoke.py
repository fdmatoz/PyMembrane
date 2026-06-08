from __future__ import annotations

import sys
from pathlib import Path

import pymembrane
from pymembrane import *  # noqa: F401,F403


def test_python_dumper_smoke(tmp_path):
    assert "vtk" not in sys.modules
    assert pymembrane.__file__

    repo_root = Path(__file__).resolve().parents[3]
    vertex_file = repo_root / "docs" / "examples" / "02_periodic" / "vertices.dat"
    face_file = repo_root / "docs" / "examples" / "02_periodic" / "faces.dat"

    system = System(Box(60.0, 60.0, 60.0, True, True, True))
    system.read_mesh_from_files({"vertices": str(vertex_file), "faces": str(face_file)})
    assert system.dumper is not None

    vtk_path = system.dumper.vtk(str(tmp_path / "smoke"), periodic=True)
    edge_vtk_path = system.dumper.edge_vtk(str(tmp_path / "smoke_edges"))
    json_path = system.dumper.json(str(tmp_path / "smoke"))
    ply_path = system.dumper.ply(str(tmp_path / "smoke"))
    txt_path = system.dumper.txt(str(tmp_path / "smoke"))
    obj_path = system.dumper.obj(str(tmp_path / "smoke"))
    vertex_user_path = system.dumper.user_data_vertex(str(tmp_path / "vertex_data"), [0.0] * system.Numvertices)
    face_user_path = system.dumper.user_data_face(str(tmp_path / "face_data"), [0.0] * system.Numfaces)
    edge_user_path = system.dumper.user_data_edge(str(tmp_path / "edge_data"), [0.0] * system.Numedges)

    assert vtk_path.exists()
    assert edge_vtk_path.exists()
    assert json_path.exists()
    assert ply_path.exists()
    assert txt_path.exists()
    assert obj_path.exists()
    assert vertex_user_path.exists()
    assert face_user_path.exists()
    assert edge_user_path.exists()

    vtk_text = vtk_path.read_text(encoding="utf-8")
    assert "ASCII" in vtk_text
    assert "DATASET POLYDATA" in vtk_text
    assert "POINTS " in vtk_text
    assert "POLYGONS " in vtk_text

    edge_vtk_text = edge_vtk_path.read_text(encoding="utf-8")
    assert "LINES " in edge_vtk_text

    obj_text = obj_path.read_text(encoding="utf-8")
    assert "\nv " in "\n" + obj_text
    assert "\nf " in "\n" + obj_text

    ply_text = ply_path.read_text(encoding="utf-8")
    assert ply_text.startswith("ply\n")
