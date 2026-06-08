from __future__ import annotations

import os
import shutil
import subprocess
import sys
from pathlib import Path

from setuptools import Extension, find_packages, setup
from setuptools.command.build_ext import build_ext


ROOT = Path(__file__).resolve().parent
EXTENSION_NAME = "pymembrane.cppmodule.md._core"
EXTENSION_SOURCE_DIR = ROOT / "pymembrane" / "cppmodule" / "md"


class CMakeExtension(Extension):
    def __init__(self, name: str, sourcedir: Path) -> None:
        super().__init__(name, sources=[])
        self.sourcedir = str(sourcedir)


class CMakeBuild(build_ext):
    def build_extension(self, ext: CMakeExtension) -> None:
        cmake = shutil.which("cmake")
        if cmake is None:
            raise RuntimeError("CMake is required to build pymembrane.")

        ext_fullpath = Path(self.get_ext_fullpath(ext.name)).resolve()
        extdir = ext_fullpath.parent
        build_temp = Path(self.build_temp, ext.name.replace(".", "_")).resolve()
        cfg = "Debug" if self.debug else "Release"

        extdir.mkdir(parents=True, exist_ok=True)
        build_temp.mkdir(parents=True, exist_ok=True)

        cmake_args = [
            f"-DCMAKE_BUILD_TYPE={cfg}",
            f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={extdir}",
            f"-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY={extdir}",
            f"-DCMAKE_RUNTIME_OUTPUT_DIRECTORY={extdir}",
        ]

        extra_cmake_args = os.environ.get("CMAKE_ARGS")
        if extra_cmake_args:
            cmake_args.extend(extra_cmake_args.split())

        build_args = ["--config", cfg]
        if "CMAKE_BUILD_PARALLEL_LEVEL" not in os.environ:
            build_args.extend(["--parallel", str(os.cpu_count() or 2)])

        subprocess.check_call([cmake, "-S", ext.sourcedir, "-B", str(build_temp), *cmake_args])
        subprocess.check_call([cmake, "--build", str(build_temp), *build_args])

        built_extension = extdir / ext_fullpath.name
        if not built_extension.exists():
            candidates = sorted(build_temp.rglob(ext_fullpath.name))
            if not candidates:
                raise RuntimeError(
                    "CMake build completed but the compiled extension was not found."
                )
            built_extension = candidates[0]

        if built_extension.resolve() != ext_fullpath:
            shutil.copy2(built_extension, ext_fullpath)


with (ROOT / "README.md").open("r", encoding="utf-8") as fh:
    long_description = fh.read()


setup(
    name="pymembrane",
    version="1.01",
    description="CPU C++ membrane simulation tools with Python bindings",
    long_description=long_description,
    long_description_content_type="text/markdown",
    author="Daniel Matoz Fernandez",
    license="MIT",
    python_requires=">=3.8",
    packages=find_packages(include=["pymembrane", "pymembrane.*"]),
    include_package_data=True,
    install_requires=["numpy"],
    ext_modules=[CMakeExtension(EXTENSION_NAME, EXTENSION_SOURCE_DIR)],
    cmdclass={"build_ext": CMakeBuild},
    zip_safe=False,
)
