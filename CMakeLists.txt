# This file is part of imaginaryMyst.
#
# imaginaryMyst is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# imaginaryMyst is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with imaginaryMyst.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.0)
project(imaginaryMyst LANGUAGES CXX)

include(FeatureSummary)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(ENABLE_SSE2 "Build with SSE2 SIMD instructions" ON)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS "-Wno-unused-parameter -Werror ${CMAKE_CXX_FLAGS}")
    if(ENABLE_SSE2)
        set(CMAKE_CXX_FLAGS "-msse2 ${CMAKE_CXX_FLAGS}")
    endif()
endif()

find_package(OpenGL REQUIRED)
set_package_properties(OpenGL PROPERTIES
    DESCRIPTION "libGL and libGLU libraries"
    TYPE REQUIRED
)

find_package(SDL2 REQUIRED)
set_package_properties(SDL2 PROPERTIES
    URL "https://www.libsdl.org"
    DESCRIPTION "Simple DirectMedia Layer"
    TYPE REQUIRED
)

find_package(GLM REQUIRED)
set_package_properties(GLM PROPERTIES
    URL "https://glm.g-truc.net/"
    DESCRIPTION "OpenGL Mathematics library"
    TYPE REQUIRED
)

find_package(ZLIB REQUIRED)
set_package_properties(ZLIB PROPERTIES
    URL "http://www.zlib.net"
    DESCRIPTION "Fast (de)compression library"
    TYPE REQUIRED
)

find_package(string_theory 2.0 REQUIRED)
set_package_properties(string_theory PROPERTIES
    URL "http://github.com/zrax/string_theory"
    DESCRIPTION "Flexible C++11 string library and type-safe formatter"
    TYPE REQUIRED
)
include_directories(${STRING_THEORY_INCLUDE_DIRS})

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
include_directories(${GLEW_INCLUDE_DIR})
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIRS})
include_directories(${ZLIB_INCLUDE_DIR})

# 3rd party Squish library for DXT codecs
include_directories(libsquish)
add_subdirectory(libsquish)

# The imaginaryMyst application
add_subdirectory(src)
