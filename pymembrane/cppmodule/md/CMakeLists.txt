cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(pymembrane_md LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

add_subdirectory(external/pybind11)

file(GLOB_RECURSE PYMEMBRANE_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
list(FILTER PYMEMBRANE_SOURCES EXCLUDE REGEX ".*/dumper/.*\\.cpp$")
list(FILTER PYMEMBRANE_SOURCES EXCLUDE REGEX ".*/meshoperations\\.cpp$")

pybind11_add_module(_core ${PYMEMBRANE_SOURCES})
target_compile_features(_core PRIVATE cxx_std_14)
target_include_directories(_core PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
if(DEFINED NUMPY_INCLUDE_DIR)
  target_include_directories(_core PRIVATE "${NUMPY_INCLUDE_DIR}")
endif()

if(CMAKE_LIBRARY_OUTPUT_DIRECTORY)
  set_target_properties(_core PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
  )
endif()

install(TARGETS _core DESTINATION "${CMAKE_INSTALL_PREFIX}")
