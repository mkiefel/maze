cmake_minimum_required (VERSION 2.8)

project (maze)

include_directories (${PROJECT_SOURCE_DIR} )

find_package (PNG REQUIRED)
include_directories (${PNG_INCLUDE_DIRS})
link_directories (${PNG_LIBRARY_DIRS})

include (CheckIncludeFileCXX)

check_include_file_cxx("CImg.h" HAVE_CIMG)
if (NOT HAVE_CIMG)
  unset (HAVE_CIMG CACHE)
  message (FATAL_ERROR "CImg.h is not found")
endif ()

if (NOT CMAKE_CONFIGURATION_TYPES )
  if (NOT CMAKE_BUILD_TYPE)
     set (CMAKE_BUILD_TYPE "Release")
  endif ()
endif ()

include (CheckCXXCompilerFlag)

check_cxx_compiler_flag(--std=c++11 HAS_CXX11_FLAG)
check_cxx_compiler_flag(--std=c++0x HAS_CXX0X_FLAG)

if (HAS_CXX11_FLAG)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")
elseif (HAS_CXX0X_FLAG)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++0x")
endif ()

# Display the selected build type if no "solution generator" is used
if (NOT CMAKE_CONFIGURATION_TYPES)
   if (CMAKE_BUILD_TYPE STREQUAL "Release")
     message(STATUS "  -> Release Build")
     message(STATUS "  -> Using flags: ${CMAKE_CXX_FLAGS_RELEASE}")
   elseif (CMAKE_BUILD_TYPE STREQUAL "Debug")
     message(STATUS "  -> Debug Build")
     message(STATUS "  -> Using flags: ${CMAKE_CXX_FLAGS_DEBUG}")
   endif ()
endif ()

enable_testing ()

add_subdirectory (maze)
