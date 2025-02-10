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

set(imSurface_HEADERS
    imAnimLayer.h
    imLayer.h
    imMaterial.h
    imMipmap.h
    imTexture.h
)

set(imSurface_SOURCES
    imAnimLayer.cpp
    imLayer.cpp
    imMaterial.cpp
    imMipmap.cpp
    imTexture.cpp
)

source_group("Header Files" FILES ${imSurface_HEADERS})
source_group("Source Files" FILES ${imSurface_SOURCES})

add_library(imSurface STATIC ${imSurface_HEADERS} ${imSurface_SOURCES})
target_include_directories(imSurface PRIVATE ${LIBSQUISH_INCLUDE_DIR})
target_link_libraries(imSurface imAnim)
target_link_libraries(imSurface imScene)    # WARNING: Circular dependency!
target_link_libraries(imSurface ${LIBSQUISH_LIBRARIES})
