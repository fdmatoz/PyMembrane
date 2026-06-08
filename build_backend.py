from __future__ import annotations

import base64
import csv
import hashlib
import os
import shutil
import subprocess
import sys
import sysconfig
import tempfile
from pathlib import Path
from zipfile import ZIP_DEFLATED, ZipFile

ROOT = Path(__file__).resolve().parent
NAME = "pymembrane"
VERSION = "1.0"
SUMMARY = "CPU C++ membrane simulation tools with Python bindings"
PYTHON_REQUIRES = ">=3.8"
EXTENSION_NAME = "pymembrane.cppmodule.md._core"
EXTENSION_SOURCE_DIR = ROOT / "pymembrane" / "cppmodule" / "md"


def _wheel_tag() -> str:
    interpreter = f"cp{sys.version_info.major}{sys.version_info.minor}"
    abi = interpreter
    platform = sysconfig.get_platform().replace("-", "_").replace(".", "_")
    return f"{interpreter}-{abi}-{platform}"


def _distribution_name() -> str:
    return NAME.replace("-", "_")


def _dist_info_dir() -> str:
    return f"{_distribution_name()}-{VERSION}.dist-info"


def _metadata_text() -> str:
    return (
        "Metadata-Version: 2.1\n"
        f"Name: {NAME}\n"
        f"Version: {VERSION}\n"
        f"Summary: {SUMMARY}\n"
        f"Requires-Python: {PYTHON_REQUIRES}\n"
        "Requires-Dist: numpy\n"
    )


def _wheel_text(root_is_purelib: bool) -> str:
    return (
        "Wheel-Version: 1.0\n"
        "Generator: build_backend\n"
        f"Root-Is-Purelib: {'true' if root_is_purelib else 'false'}\n"
        f"Tag: {_wheel_tag()}\n"
    )


def _hash_bytes(data: bytes) -> str:
    digest = hashlib.sha256(data).digest()
    return "sha256=" + base64.urlsafe_b64encode(digest).rstrip(b"=").decode("ascii")


def _record_row(path: str, data: bytes) -> list[str]:
    return [path, _hash_bytes(data), str(len(data))]


def _copy_file(source: Path, destination: Path) -> None:
    destination.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(source, destination)


def _copy_data_tree(source_root: Path, destination_root: Path) -> None:
    if not source_root.exists():
        return
    for source in source_root.rglob("*"):
        if not source.is_file():
            continue
        if source.suffix in {".py", ".pyc", ".pyo", ".rst", ".md"}:
            continue
        destination = destination_root / source.relative_to(source_root)
        _copy_file(source, destination)


def _build_extension(output_dir: Path) -> Path:
    cmake = shutil.which("cmake")
    if cmake is None:
        raise RuntimeError("CMake is required to build pymembrane.")

    build_temp = Path(tempfile.mkdtemp(prefix="pymembrane-cmake-"))
    cfg = "Debug" if os.environ.get("DEBUG") else "Release"
    output_dir.mkdir(parents=True, exist_ok=True)

    cmake_args = [
        f"-DCMAKE_BUILD_TYPE={cfg}",
        f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={output_dir}",
        f"-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY={output_dir}",
        f"-DCMAKE_RUNTIME_OUTPUT_DIRECTORY={output_dir}",
        f"-DPYTHON_EXECUTABLE={sys.executable}",
        f"-DPython_EXECUTABLE={sys.executable}",
    ]

    python_include = sysconfig.get_path("include")
    if python_include:
        cmake_args.append(f"-DPYTHON_INCLUDE_DIR={python_include}")

    python_library_name = sysconfig.get_config_var("LDLIBRARY") or sysconfig.get_config_var("LIBRARY")
    python_library_dir = sysconfig.get_config_var("LIBDIR")
    if python_library_name and python_library_dir:
        python_library = Path(python_library_dir, python_library_name)
        if python_library.exists():
            cmake_args.append(f"-DPYTHON_LIBRARY={python_library}")

    extra_cmake_args = os.environ.get("CMAKE_ARGS")
    if extra_cmake_args:
        cmake_args.extend(extra_cmake_args.split())

    build_args = ["--config", cfg]
    if "CMAKE_BUILD_PARALLEL_LEVEL" not in os.environ:
        build_args.extend(["--parallel", str(os.cpu_count() or 2)])

    subprocess.check_call([cmake, "-S", str(EXTENSION_SOURCE_DIR), "-B", str(build_temp), *cmake_args])
    subprocess.check_call([cmake, "--build", str(build_temp), *build_args])

    ext_suffix = sysconfig.get_config_var("EXT_SUFFIX") or ".so"
    expected = output_dir / f"_core{ext_suffix}"
    if expected.exists():
        return expected

    candidates = sorted(build_temp.rglob(f"_core{ext_suffix}"))
    if not candidates:
        raise RuntimeError("CMake build completed but the compiled extension was not found.")
    shutil.copy2(candidates[0], expected)
    return expected


def _stage_package(root: Path, editable: bool) -> Path:
    package_root = root / NAME
    if editable:
        for source in [
            ROOT / "pymembrane" / "__init__.py",
            ROOT / "pymembrane" / "benchmarks" / "__init__.py",
            ROOT / "pymembrane" / "benchmarks" / "sphere.py",
            ROOT / "pymembrane" / "cppmodule" / "__init__.py",
            ROOT / "pymembrane" / "cppmodule" / "dump" / "__init__.py",
            ROOT / "pymembrane" / "cppmodule" / "dump" / "dumper.py",
            ROOT / "pymembrane" / "cppmodule" / "md" / "__init__.py",
            ROOT / "pymembrane" / "examples" / "__init__.py",
            ROOT / "pymembrane" / "examples" / "_resources.py",
            ROOT / "pymembrane" / "examples" / "periodic.py",
            ROOT / "pymembrane" / "examples" / "buckling.py",
            ROOT / "pymembrane" / "examples" / "minimizer.py",
            ROOT / "pymembrane" / "examples" / "disclination.py",
            ROOT / "pymembrane" / "examples" / "disclination_mc.py",
            ROOT / "pymembrane" / "examples" / "disclination_verlet.py",
            ROOT / "pymembrane" / "examples" / "hybrid_mc_bd.py",
            ROOT / "pymembrane" / "examples" / "data" / "__init__.py",
        ]:
            _copy_file(source, root / source.relative_to(ROOT))

        _copy_data_tree(ROOT / "docs" / "examples", root / "pymembrane" / "examples" / "data")
        built_extension = _build_extension(ROOT / "pymembrane" / "cppmodule" / "md")
        _copy_file(built_extension, root / "pymembrane" / "cppmodule" / "md" / built_extension.name)
        (root / f"{_distribution_name()}-{VERSION}.pth").write_text(str(ROOT) + "\n", encoding="utf-8")
        return root

    for source in [
        ROOT / "pymembrane" / "__init__.py",
        ROOT / "pymembrane" / "benchmarks" / "__init__.py",
        ROOT / "pymembrane" / "benchmarks" / "sphere.py",
        ROOT / "pymembrane" / "cppmodule" / "__init__.py",
        ROOT / "pymembrane" / "cppmodule" / "dump" / "__init__.py",
        ROOT / "pymembrane" / "cppmodule" / "dump" / "dumper.py",
        ROOT / "pymembrane" / "cppmodule" / "md" / "__init__.py",
        ROOT / "pymembrane" / "examples" / "__init__.py",
        ROOT / "pymembrane" / "examples" / "_resources.py",
        ROOT / "pymembrane" / "examples" / "periodic.py",
        ROOT / "pymembrane" / "examples" / "buckling.py",
        ROOT / "pymembrane" / "examples" / "minimizer.py",
        ROOT / "pymembrane" / "examples" / "disclination.py",
        ROOT / "pymembrane" / "examples" / "disclination_mc.py",
        ROOT / "pymembrane" / "examples" / "disclination_verlet.py",
        ROOT / "pymembrane" / "examples" / "hybrid_mc_bd.py",
        ROOT / "pymembrane" / "examples" / "data" / "__init__.py",
    ]:
        _copy_file(source, root / source.relative_to(ROOT))

    _copy_data_tree(ROOT / "docs" / "examples", root / "pymembrane" / "examples" / "data")

    ext_dir = root / "pymembrane" / "cppmodule" / "md"
    _build_extension(ext_dir)
    return package_root


def _write_metadata(root: Path, root_is_purelib: bool) -> str:
    dist_info = root / _dist_info_dir()
    dist_info.mkdir(parents=True, exist_ok=True)
    (dist_info / "METADATA").write_text(_metadata_text(), encoding="utf-8")
    (dist_info / "WHEEL").write_text(_wheel_text(root_is_purelib), encoding="utf-8")
    return dist_info.name


def _build_wheel_impl(wheel_directory: str, editable: bool) -> str:
    wheel_directory = str(wheel_directory)
    staging_root = Path(tempfile.mkdtemp(prefix="pymembrane-wheel-"))
    try:
        _stage_package(staging_root, editable=editable)
        dist_info = _write_metadata(staging_root, root_is_purelib=editable)

        records: list[list[str]] = []
        for file_path in sorted(staging_root.rglob("*")):
            if not file_path.is_file():
                continue
            if file_path.name == "RECORD":
                continue
            arcname = file_path.relative_to(staging_root).as_posix()
            records.append(_record_row(arcname, file_path.read_bytes()))

        record_path = staging_root / dist_info / "RECORD"
        with record_path.open("w", newline="", encoding="utf-8") as fp:
            writer = csv.writer(fp)
            writer.writerows(records)
            writer.writerow([f"{dist_info}/RECORD", "", ""])

        wheel_name = f"{_distribution_name()}-{VERSION}-{_wheel_tag()}.whl"
        wheel_path = Path(wheel_directory) / wheel_name
        with ZipFile(wheel_path, "w", compression=ZIP_DEFLATED) as zf:
            for file_path in sorted(staging_root.rglob("*")):
                if not file_path.is_file():
                    continue
                arcname = file_path.relative_to(staging_root).as_posix()
                zf.write(file_path, arcname)

        return wheel_name
    finally:
        shutil.rmtree(staging_root, ignore_errors=True)


def build_wheel(wheel_directory, config_settings=None, metadata_directory=None):
    return _build_wheel_impl(wheel_directory, editable=False)


def build_editable(wheel_directory, config_settings=None, metadata_directory=None):
    return _build_wheel_impl(wheel_directory, editable=True)


def _metadata_impl(metadata_directory: str, editable: bool) -> str:
    metadata_root = Path(metadata_directory) / _dist_info_dir()
    metadata_root.mkdir(parents=True, exist_ok=True)
    (metadata_root / "METADATA").write_text(_metadata_text(), encoding="utf-8")
    (metadata_root / "WHEEL").write_text(_wheel_text(root_is_purelib=editable), encoding="utf-8")
    return metadata_root.name


def prepare_metadata_for_build_wheel(metadata_directory, config_settings=None):
    return _metadata_impl(metadata_directory, editable=False)


def prepare_metadata_for_build_editable(metadata_directory, config_settings=None):
    return _metadata_impl(metadata_directory, editable=True)


def get_requires_for_build_wheel(config_settings=None):
    return []


def get_requires_for_build_editable(config_settings=None):
    return []


def _supported_features():
    return ["build_editable"]
