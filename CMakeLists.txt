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
 install(
	      CODE"
	        set(PREFIX \$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX})
	        set(INPUT_FILE \${PREFIX}/include/half/half.hpp)
	        set(SYMLINK_LOCATION \${PREFIX}/include)
	        execute_process(COMMAND ln -sfr \${INPUT_FILE} \${SYMLINK_LOCATION})
	        message(STATUS \"Created symlink in \${SYMLINK_LOCATION} to \${INPUT_FILE}.\")
	        "
	    )
endif()

rocm_create_package(
    NAME half
    DESCRIPTION "HALF-PRECISION FLOATING POINT LIBRARY"
    MAINTAINER "Paul Fultz II <paul.fultz@amd.com>"
)
