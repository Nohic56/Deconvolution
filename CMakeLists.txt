cmake_minimum_required(VERSION 2.8.12)
project(Deconvolution)

# The version number.
set (Deconvolution_VERSION_MAJOR 1)
set (Deconvolution_VERSION_MINOR 0)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/Deconvolution_Config.h.in"
  "${PROJECT_BINARY_DIR}/Deconvolution_Config.h"
  )
# add the binary tree to the search path for include files
# so that we will find IdXML_TopDownConfig.h
include_directories("${PROJECT_BINARY_DIR}")

# add the executable
add_executable(Deconvolution Main_Deconvolution.cpp Deconvolution.cpp)

