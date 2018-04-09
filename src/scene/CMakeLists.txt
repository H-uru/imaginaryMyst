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

set(imScene_HEADERS
    imEnvironment.h
    imSceneDatabase.h
    imSceneIndex.h
)

set(imScene_SOURCES
    imEnvironment.cpp
    imSceneDatabase.cpp
    imSceneIndex.cpp
)

source_group("Header Files" FILES ${imScene_HEADERS})
source_group("Source Files" FILES ${imScene_SOURCES})

add_library(imScene STATIC ${imScene_HEADERS} ${imScene_SOURCES})
target_link_libraries(imScene imSurface)    # WARNING: Circular dependency!
target_link_libraries(imScene imSound)
target_link_libraries(imScene imVfs)
