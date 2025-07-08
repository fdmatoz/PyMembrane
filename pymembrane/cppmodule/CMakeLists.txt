#####################################################################################
# MIT License                                                                       #
#                                                                                   #
# Copyright (c) 2023 Dr. Daniel Alejandro Matoz Fernandez                           #
#               fdamatoz@gmail.com                                                  #
# Permission is hereby granted, free of charge, to any person obtaining a copy      #
# of this software and associated documentation files (the "Software"), to deal     #
# in the Software without restriction, including without limitation the rights      #
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell         #
# copies of the Software, and to permit persons to whom the Software is             #
# furnished to do so, subject to the following conditions:                          #
#                                                                                   #
# The above copyright notice and this permission notice shall be included in all    #
# copies or substantial portions of the Software.                                   #
#                                                                                   #
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR        #
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,          #
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE       #
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER            #
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,     #
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     #
# SOFTWARE.                                                                         #
#####################################################################################

CMAKE_MINIMUM_REQUIRED(VERSION 3.12 FATAL_ERROR)
project (MEMBRANEEVOLVER C CXX)
set(CMAKE_BUILD_TYPE "Release")
# place all executables in the build directory 
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_VERBOSE_MAKEFILE ON)

########### ADD C++ SUPPORT  ##############
set(CMAKE_CXX_STANDARD 14)
find_program(CMAKE_C_COMPILER NAMES $ENV{CC} gcc PATHS ENV PATH NO_DEFAULT_PATH)
find_program(CMAKE_CXX_COMPILER NAMES $ENV{CXX} g++ PATHS ENV PATH NO_DEFAULT_PATH)
set(CMAKE_CXX_STANDARD 14)
########### SET C++ COMPILER FLAGS  ############
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall" CACHE STRING "Flags used by the compiler during debug builds." FORCE)
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -Wall -DNDEBUG" CACHE STRING "Flags used by the compiler during minimum size release builds." FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -funroll-loops -ffast-math -DNDEBUG -std=c++14 -fvisibility=hidden -fpermissive" CACHE STRING "Flags used by the compiler during release builds." FORCE)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -O -funroll-loops -ffast-math -DNDEBUG -Wall" CACHE STRING "Flags used by the compiler during release builds with debug info." FORCE)
############################################

#find_package(VTK)
#include(${VTK_USE_FILE})
#add_definitions(-DHAS_VTK)
#find_package(VTK)
if(VTK_FOUND)
  if (VTK_MAJOR_VERSION LESS 9)
    include(${VTK_USE_FILE})
  endif()
  add_definitions(-DHAS_VTK)
  set(VTK_LIBS ${VTK_LIBRARIES})
  set(VTK_Group_StandAlone OFF)
  set(VTK_Group_Rendering OFF)
else(VTK_FOUND)
  message("VTK not found on the system. It will be downloaded in compiled locally. It may take a while.")
  include(ExternalProject)
  set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external/VTK)
  set(VTK_version 8.2)
  if (NOT APPLE)
    set(ADDITIONAL_LINKER_FLAGS "-Wl,--disable-new-dtags")
    ExternalProject_Add(VTKLocal
      GIT_REPOSITORY https://github.com/Kitware/VTK.git
      GIT_TAG v${VTK_version}.0
      GIT_SHALLOW TRUE
      CMAKE_ARGS  -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION} 
                  -DCMAKE_INSTALL_LIBDIR=${EXTERNAL_INSTALL_LOCATION}/lib
                  -DCMAKE_INSTALL_RPATH=${CMAKE_INSTALL_PREFIX}/external/VTK/lib
                  -DCMAKE_SHARED_LINKER_FLAGS=${ADDITIONAL_LINKER_FLAGS}
                  -DCMAKE_BUILD_WITH_INSTALL_RPATH=ON
                  -DCMAKE_BUILD_TYPE=Debug
                  -DBUILD_EXAMPLES=OFF 
                  -DBUILD_SHARED_LIBS=ON 
                  -DBUILD_TESTING=OFF 
                  -DVTK_Group_Rendering=OFF 
                  -DVTK_Group_StandAlone=OFF 
                  -DVTK_WRAP_PYTHON=OFF 
                  -DModule_vtkIOXML=ON 
                  -DModule_vtkIOXMLParser=ON 
                  -DVTK_RENDERING_BACKEND=None
    )
  else()
    ExternalProject_Add(VTKLocal
    GIT_REPOSITORY https://github.com/Kitware/VTK.git
    GIT_TAG v${VTK_version}.0
    GIT_SHALLOW TRUE
    CMAKE_ARGS  -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION} 
                -DCMAKE_INSTALL_LIBDIR=${EXTERNAL_INSTALL_LOCATION}/lib
                -DCMAKE_INSTALL_RPATH=${CMAKE_INSTALL_PREFIX}/external/VTK/lib
                -DCMAKE_BUILD_TYPE=Debug
                -DBUILD_EXAMPLES=OFF 
                -DBUILD_SHARED_LIBS=ON 
                -DBUILD_TESTING=OFF 
                -DVTK_Group_Rendering=OFF 
                -DVTK_Group_StandAlone=OFF 
                -DVTK_WRAP_PYTHON=OFF 
                -DModule_vtkIOXML=ON 
                -DModule_vtkIOXMLParser=ON 
                -DVTK_RENDERING_BACKEND=None
      )
  endif()
  set(VTKLocal TRUE)
  set(VTK_LIBS vtkCommonCore-${VTK_version} 
               vtkCommonDataModel-${VTK_version}
               vtkCommonExecutionModel-${VTK_version} 
               vtkCommonMath-${VTK_version} 
               vtkCommonSystem-${VTK_version}
               vtkCommonTransforms-${VTK_version}
               vtkIOCore-${VTK_version}
               vtkIOXML-${VTK_version}
               vtkIOXMLParser-${VTK_version}
               vtkdoubleconversion-${VTK_version}
               vtkexpat-${VTK_version}
               vtklz4-${VTK_version}
               vtklzma-${VTK_version}
               vtksys-${VTK_version}
               vtkzlib-${VTK_version}
               )
  include_directories(${EXTERNAL_INSTALL_LOCATION}/include/vtk-${VTK_version})
  link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)
  # use, i.e. don't skip the full RPATH for the build tree
  set(CMAKE_SKIP_BUILD_RPATH FALSE)
  
  if (NOT APPLE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    set(CMAKE_INSTALL_RPATH "$ORIGIN/lib")
  else()
    set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
    set(CMAKE_INSTALL_RPATH "${EXTERNAL_INSTALL_LOCATION}/lib")
  endif()

  #MESSAGE("origin:" ${ORIGIN})
  # add the automatically determined parts of the RPATH
  # which point to directories outside the build tree to the install RPATH
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

  #set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)

  # the RPATH to be used when installing, but only if it's not a system directory
  if (APPLE)
    list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${EXTERNAL_INSTALL_LOCATION}/lib" isSystemDir)
    if("${isSystemDir}" STREQUAL "-1")
        set(CMAKE_INSTALL_RPATH "${EXTERNAL_INSTALL_LOCATION}/lib")
    endif("${isSystemDir}" STREQUAL "-1")
  endif()
  add_definitions(-DHAS_VTK)
endif(VTK_FOUND)


########### ADD PYBIND SUPPORT  ###############
add_subdirectory(external/pybind11)
set(PYBIND11_PYTHON_VERSION 3.7 CACHE STRING "")
set(PYBIND11_CPP_STANDARD -std=c++14)
############################################

############################################
SET(INCLUDE_DIRS "${MEMBRANEEVOLVER_SOURCE_DIR}/src")
include_directories(${INCLUDE_DIRS})
file(GLOB INCLUDE_DIRS_SRCS ${INCLUDE_DIRS}/*.cpp)
file(GLOB INCLUDE_DIRS_HDRS ${INCLUDE_DIRS}/*.hpp)
## utils ##
file(GLOB utils_HDRS ${INCLUDE_DIRS}/utils/*.hpp)
file(GLOB utils_SRCS ${INCLUDE_DIRS}/utils/*.cpp)
list(APPEND INCLUDE_DIRS_HDRS ${utils_HDRS})
list(APPEND INCLUDE_DIRS_SRCS ${utils_SRCS})
## rng ##
file(GLOB types_HDRS ${INCLUDE_DIRS}/rng/*.hpp)
file(GLOB types_SRCS ${INCLUDE_DIRS}/rng/*.cpp)
list(APPEND INCLUDE_DIRS_HDRS ${types_HDRS})
list(APPEND INCLUDE_DIRS_SRCS ${types_SRCS})
## types ##
file(GLOB types_HDRS ${INCLUDE_DIRS}/types/*.hpp)
file(GLOB types_SRCS ${INCLUDE_DIRS}/types/*.cpp)
list(APPEND INCLUDE_DIRS_HDRS ${types_HDRS})
list(APPEND INCLUDE_DIRS_SRCS ${types_SRCS})
## box ##
file(GLOB box_GPUHDRS ${INCLUDE_DIRS}/box/*.hpp)
file(GLOB box_GPUSRCS ${INCLUDE_DIRS}/box/*.cpp)
list(APPEND INCLUDE_DIRS_GPUHDRS ${box_GPUHDRS})
list(APPEND INCLUDE_DIRS_GPUSRCS ${box_GPUSRCS})
## system ##
file(GLOB system_HDRS ${INCLUDE_DIRS}/system/*.hpp)
file(GLOB system_SRCS ${INCLUDE_DIRS}/system/*.cpp)
list(APPEND INCLUDE_DIRS_HDRS ${system_HDRS})
list(APPEND INCLUDE_DIRS_SRCS ${system_SRCS})
## dump ##
file(GLOB dumper_HDRS ${INCLUDE_DIRS}/dumper/*.hpp)
file(GLOB dumper_SRCS ${INCLUDE_DIRS}/dumper/*.cpp)
list(APPEND INCLUDE_DIRS_HDRS ${dumper_HDRS})
list(APPEND INCLUDE_DIRS_SRCS ${dumper_SRCS})
## compute ##
file(GLOB compute_HDRS ${INCLUDE_DIRS}/compute/*.hpp)
file(GLOB compute_SRCS ${INCLUDE_DIRS}/compute/*.cpp)
list(APPEND INCLUDE_DIRS_HDRS ${compute_HDRS})
list(APPEND INCLUDE_DIRS_SRCS ${compute_SRCS})
## mesh ##
file(GLOB mesh_HDRS ${INCLUDE_DIRS}/mesh/*.hpp)
file(GLOB mesh_SRCS ${INCLUDE_DIRS}/mesh/*.cpp)
list(APPEND INCLUDE_DIRS_HDRS ${mesh_HDRS})
list(APPEND INCLUDE_DIRS_SRCS ${mesh_SRCS})
## potentials ##
file(GLOB potentials_HDRS ${INCLUDE_DIRS}/potentials/*.hpp)
file(GLOB potentials_SRCS ${INCLUDE_DIRS}/potentials/*.cpp)
list(APPEND INCLUDE_DIRS_HDRS ${potentials_HDRS})
list(APPEND INCLUDE_DIRS_SRCS ${potentials_SRCS})
## constraint ##
file(GLOB constraints_HDRS ${INCLUDE_DIRS}/constraints/*.hpp)
file(GLOB constraints_SRCS ${INCLUDE_DIRS}/constraints/*.cpp)
list(APPEND INCLUDE_DIRS_HDRS ${constraints_HDRS})
list(APPEND INCLUDE_DIRS_SRCS ${constraints_SRCS})
## integrators ##
file(GLOB integrators_HDRS ${INCLUDE_DIRS}/integrators/*.hpp)
file(GLOB integrators_SRCS ${INCLUDE_DIRS}/integrators/*.cpp)
list(APPEND INCLUDE_DIRS_HDRS ${integrators_HDRS})
list(APPEND INCLUDE_DIRS_SRCS ${integrators_SRCS})
## minimizer ##
file(GLOB minimizer_HDRS ${INCLUDE_DIRS}/minimizer/*.hpp)
file(GLOB minimizer_SRCS ${INCLUDE_DIRS}/minimizer/*.cpp)
list(APPEND INCLUDE_DIRS_HDRS ${minimizer_HDRS})
list(APPEND INCLUDE_DIRS_SRCS ${minimizer_SRCS})
## evolver ##
file(GLOB evolver_HDRS ${INCLUDE_DIRS}/evolver/*.hpp)
file(GLOB evolver_SRCS ${INCLUDE_DIRS}/evolver/*.cpp)
list(APPEND INCLUDE_DIRS_HDRS ${evolver_HDRS})
list(APPEND INCLUDE_DIRS_SRCS ${evolver_SRCS})
############################################

set_directory_properties( PROPERTIES COMPILE_DEFINITIONS "" )

set(MEMB_LIBS ${VTK_LIBS})

#pybind11_add_module(cppmodule ${INCLUDE_DIRS_SRCS})
#target_link_libraries(cppmodule PRIVATE pybind11::module ${VTK_LIBS} ${VTK_LIBRARIES} ${THREAD_LIB}) 
# use, i.e. don't skip the full RPATH for the build tree
set(CMAKE_SKIP_BUILD_RPATH FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)

if (NOT APPLE)
  set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
	set(CMAKE_INSTALL_RPATH $ORIGIN)
else()
  set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/external/VTK/lib")
endif()


# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


# the RPATH to be used when installing, but only if it's not a system directory
if (APPLE)
  list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/external/VTK/lib" isSystemDir)
  if("${isSystemDir}" STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/external/VTK/lib")
  endif("${isSystemDir}" STREQUAL "-1")
endif()

add_library(cppmodule SHARED ${INCLUDE_DIRS_SRCS})
target_link_libraries(cppmodule PRIVATE pybind11::module ${MEMB_LIBS}) 


#set_target_properties(cppmodule PROPERTIES PREFIX "${PYTHON_MODULE_PREFIX}" SUFFIX "${PYTHON_MODULE_EXTENSION}")
#set_target_properties(cppmodule PROPERTIES PREFIX "${PYTHON_MODULE_PREFIX}" SUFFIX "${PYTHON_MODULE_EXTENSION}" OUTPUT_NAME "cppmodule")                                               
#set_target_properties(cppmodule PROPERTIES PREFIX "${PYTHON_MODULE_PREFIX}" OUTPUT_NAME "cppmodule")    
if (NOT APPLE)                                           
  set_target_properties(cppmodule PROPERTIES PREFIX ""  OUTPUT_NAME "cppmodule" SUFFIX ".so" CXX_STANDARD 14 INSTALL_RPATH "$ORIGIN/external/VTK/lib")
else()
  set_target_properties(cppmodule PROPERTIES PREFIX ""  OUTPUT_NAME "cppmodule" SUFFIX ".so" CXX_STANDARD 14)
endif()

install(TARGETS cppmodule DESTINATION ${CMAKE_INSTALL_PREFIX})

if (VTKLocal)
  add_dependencies(cppmodule VTKLocal)
  install(DIRECTORY ${CMAKE_BINARY_DIR}/external/VTK DESTINATION external)
endif()

#MESSAGE("origin:" ${ORIGIN})
