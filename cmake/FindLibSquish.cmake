#################################################
#
# Finds the open source DXT Library, Squish
#
# https://sourceforge.net/projects/libsquish/
#
#
# Supplies:
#  LIBSQUISH_FOUND
#  LIBSQUISH_INCLUDE_DIR
#  LIBSQUISH_LIBRARIES
#

# Additional modules
include(FindPackageHandleStandardArgs)
include(SelectLibraryConfigurations)

set(LIBSQUISH_PATH CACHE PATH "Path to libSquish installation")

if(LIBSQUISH_PATH)
    file(TO_CMAKE_PATH ${LIBSQUISH_PATH}/include LIBSQUISH_INC_SEARCH )
    file(TO_CMAKE_PATH ${LIBSQUISH_PATH}/lib LIBSQUISH_LIB_SEARCH)
endif(LIBSQUISH_PATH)

find_path(LIBSQUISH_INCLUDE_DIR squish.h PATHS ${LIBSQUISH_INC_SEARCH} DOC "Path containing libSquish header")
find_library(LIBSQUISH_LIBRARY_RELEASE NAMES squish libsquish
    PATHS ${LIBSQUISH_LIB_SEARCH}
    PATH_SUFFIXES lib
    DOC "Path to libSquish release library")
find_library(LIBSQUISH_LIBRARY_DEBUG NAMES squishd libsquishd
    PATHS ${LIBSQUISH_LIB_SEARCH}
    PATH_SUFFIXES lib
    DOC "Path to libSquish debug library")

select_library_configurations(LIBSQUISH)
find_package_handle_standard_args(LibSquish REQUIRED_VARS LIBSQUISH_LIBRARY LIBSQUISH_INCLUDE_DIR)
