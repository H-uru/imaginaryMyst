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

set(imAnim_HEADERS
    imAnimBehavior.h
    imController.h
    imKeyFrame.h
    imLeafController.h
)

set(imAnim_SOURCES
    imAnimBehavior.cpp
    imController.cpp
    imKeyFrame.cpp
    imLeafController.cpp
)

source_group("Header Files" FILES ${imAnim_HEADERS})
source_group("Source Files" FILES ${imAnim_SOURCES})

add_library(imAnim STATIC ${imAnim_HEADERS} ${imAnim_SOURCES})
target_link_libraries(imAnim imVfs)
