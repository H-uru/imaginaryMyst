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

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(anim)
add_subdirectory(scene)
add_subdirectory(sound)
add_subdirectory(surface)
add_subdirectory(vfs)

set(imCommon_HEADERS
    imCommon.h
    imGeometry3.h
    imKeyList.h
    imRef.h
    imString.h
    imThread.h
)

set(imCommon_SOURCES
    imGeometry3.cpp
    imKeyList.cpp
    imLog.cpp
    imString.cpp
    Main.cpp
)

source_group("Header Files" FILES ${imCommon_HEADERS})
source_group("Source Files" FILES ${imCommon_SOURCES})

add_executable(imaginaryMyst WIN32 ${imCommon_HEADERS} ${imCommon_SOURCES})
target_link_libraries(imaginaryMyst imAnim imScene imSound imSurface imVfs)
target_link_libraries(imaginaryMyst squish)
target_link_libraries(imaginaryMyst ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})
target_link_libraries(imaginaryMyst ${ZLIB_LIBRARIES})

install(TARGETS imaginaryMyst RUNTIME DESTINATION bin)
