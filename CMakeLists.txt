cmake_minimum_required(VERSION 3.13.2)
project(libvl VERSION 1.0 DESCRIPTION "libvl - A versatile init library")

#--------------------------------------------------------------
# Options
#
cmake_policy(SET CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
cmake_policy(SET CMP0077 NEW)
set(CMAKE_MACOSX_RPATH ON)

# use, i.e. don't skip the full RPATH for the build tree
set(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

# the RPATH to be used when installing
set(CMAKE_INSTALL_RPATH "") # "

# don't add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)

# Build with debug info.
set(CMAKE_BUILD_TYPE RelWithDebInfo)

add_definitions(-DDEBUG -DACCTDEBUG)

#--------------------------------------------------------------
# Subdirectories
#
add_subdirectory(src)
add_subdirectory(test)