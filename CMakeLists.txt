cmake_minimum_required(VERSION 3.5)

# This has to be initialized before the project() command appears
# Set the default of CMAKE_BUILD_TYPE to be release, unless user specifies with -D.  MSVC_IDE does not use CMAKE_BUILD_TYPE
if( NOT MSVC_IDE AND NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." )
endif()

# Default installation path
if(WIN32)
    set(CMAKE_INSTALL_PREFIX "/opt/rocm/x86_64-w64-mingw32" CACHE PATH "")
else()
    set(CMAKE_INSTALL_PREFIX "/opt/rocm" CACHE PATH "")
endif()

project(half)
find_package(ROCM REQUIRED)

include(ROCMSetupVersion)
include(ROCMCreatePackage)

# FOR HANDLING ENABLE/DISABLE OPTIONAL BACKWARD COMPATIBILITY for FILE/FOLDER REORG
option(BUILD_FILE_REORG_BACKWARD_COMPATIBILITY "Build with file/folder reorg with backward compatibility enabled" ON)

rocm_setup_version(VERSION 1.12.0 NO_GIT_TAG_VERSION)

install(FILES include/half.hpp DESTINATION include/half)

if(BUILD_FILE_REORG_BACKWARD_COMPATIBILITY)
#Generate header template file
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/wrapper/header.hpp.in "/* \nCopyright (c) 2022 Advanced Micro Devices, Inc. All rights reserved.\n*/
    \n\n#ifndef @include_guard@\n#define @include_guard@
    \n\n#pragma message(\"This file is deprecated. Use file from include path /opt/rocm-ver/include and prefix with half\")
    \n@include_statements@ \n\n#endif")

#use header template file and generate wrapper header file
  set(include_guard "HALF_WRAPPER_INC_HALF_HPP")
  set(include_statements "#include \"half/half.hpp\"\n")

  configure_file( ${CMAKE_CURRENT_BINARY_DIR}/wrapper/header.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/wrapper/half.hpp)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/wrapper/half.hpp DESTINATION include)

endif()

rocm_create_package(
    NAME half
    DESCRIPTION "HALF-PRECISION FLOATING POINT LIBRARY"
    MAINTAINER "Paul Fultz II <paul.fultz@amd.com>"
)
