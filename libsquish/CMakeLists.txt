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

set(squish_SOURCES
    alpha.h
    alpha.cpp
    clusterfit.h
    clusterfit.cpp
    colourblock.h
    colourblock.cpp
    colourfit.h
    colourfit.cpp
    colourset.h
    colourset.cpp
    maths.h
    maths.cpp
    rangefit.h
    rangefit.cpp
    simd.h
    simd_float.h
    simd_sse.h
    singlecolourfit.h
    singlecolourfit.cpp
    squish.h
    squish.cpp
)

if(ENABLE_SSE2)
    # Use SSE2 optimized squish codecs
    set(-DSQUISH_USE_SSE=2)
endif()

add_library(squish STATIC ${squish_SOURCES})
