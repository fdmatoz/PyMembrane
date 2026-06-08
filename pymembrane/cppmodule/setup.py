from __future__ import annotations

import os
import shutil
import subprocess
import sys
import sysconfig
from pathlib import Path

from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext
from setuptools.command.develop import develop


class CMakeExtension(Extension):
    def __init__(self, name: str, sourcedir: str = "md") -> None:
        super().__init__(name, sources=[])
        self.sourcedir = str(Path(sourcedir).resolve())


class CMakeBuild(build_ext):
    def build_extension(self, ext: CMakeExtension) -> None:
        cmake = shutil.which("cmake")
        if cmake is None:
            raise RuntimeError("CMake is required to build pymembrane.")

        ext_fullpath = Path(self.get_ext_fullpath(ext.name)).resolve()
        extdir = ext_fullpath.parent
        build_temp = Path(self.build_temp, ext.name).resolve()
        cfg = "Debug" if self.debug else "Release"

        extdir.mkdir(parents=True, exist_ok=True)
        build_temp.mkdir(parents=True, exist_ok=True)

        cmake_args = [
            f"-DCMAKE_BUILD_TYPE={cfg}",
            f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={extdir}",
            f"-DCMAKE_INSTALL_PREFIX={extdir}",
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


class DevelopWithPackageParent(develop):
    def run(self) -> None:
        super().run()
        project_root = Path(__file__).resolve().parent
        project_parent = project_root.parent
        install_dir = Path(self.install_dir)

        egg_link = install_dir / f"{self.distribution.get_name()}.egg-link"
        if egg_link.exists():
            egg_link.write_text(f"{project_parent}\n{project_root.name}\n", encoding="utf-8")

        easy_install_pth = install_dir / "easy-install.pth"
        if easy_install_pth.exists():
            text = easy_install_pth.read_text(encoding="utf-8")
            text = text.replace(str(project_root), str(project_parent))
            easy_install_pth.write_text(text, encoding="utf-8")


setup(
    name="pymembrane",
    version="1.0a0",
    description="CPU C++ membrane simulation tools with Python bindings",
    long_description=(Path(__file__).with_name("README.md").read_text(encoding="utf-8")),
    long_description_content_type="text/markdown",
    author="Daniel Matoz Fernandez",
    license="MIT",
    python_requires=">=3.8",
    packages=["pymembrane", "pymembrane.md", "pymembrane.dump"],
    package_dir={"pymembrane": "."},
    include_package_data=True,
    install_requires=["numpy"],
    ext_modules=[CMakeExtension("pymembrane.md._core")],
    cmdclass={"build_ext": CMakeBuild, "develop": DevelopWithPackageParent},
    zip_safe=False,
)
