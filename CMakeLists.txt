#
# Copyright (c) 2010-2015, Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

cmake_minimum_required(VERSION 2.8.9)
project(libmediawiki)

message(STATUS "----------------------------------------------------------------------------------")
message(STATUS "Starting CMake configuration for: libmediawiki")

find_package(KDE4 REQUIRED)

include(KDE4Defaults)
include(MacroLibrary)
include(MacroOptionalAddSubdirectory)
include(MacroOptionalFindPackage)
include(FindPackageHandleStandardArgs)

add_definitions (${QT_DEFINITIONS} ${QT_QTDBUS_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories (${QDBUS_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})

set(LIBMEDDIAWIKI_AREA_CODE_GENERAL 51007)
add_definitions(-DKDE_DEFAULT_DEBUG_AREA=${LIBMEDDIAWIKI_AREA_CODE_GENERAL})

# =======================================================
# Information to update before to release this library.

# Library version history:
# API      ABI
# 1.0.0 => 1.0.0  (released with KDE 4.7.0)

# Library API version
set(MEDIAWIKI_LIB_MAJOR_VERSION "1")
set(MEDIAWIKI_LIB_MINOR_VERSION "0")
set(MEDIAWIKI_LIB_PATCH_VERSION "0")

# Suffix to add at end of version string. Usual values are:
# "-git"   : alpha code unstable from git. Do not use in production
# "-beta1" : beta1 release.
# "-beta2" : beta2 release.
# "-beta3" : beta3 release.
# "-rc"    : release candidate.
# ""       : final relase. Can be used in production.
set(MEDIAWIKI_LIB_SUFFIX_VERSION "")

# Library ABI version used by linker.
# For details : http://www.gnu.org/software/libtool/manual/libtool.html#Updating-version-info
set(MEDIAWIKI_LIB_SO_CUR_VERSION "1")
set(MEDIAWIKI_LIB_SO_REV_VERSION "0")
set(MEDIAWIKI_LIB_SO_AGE_VERSION "0")

# =======================================================
# Set env. variables accordinly.

set(MEDIAWIKI_LIB_VERSION_STRING    "${MEDIAWIKI_LIB_MAJOR_VERSION}.${MEDIAWIKI_LIB_MINOR_VERSION}.${MEDIAWIKI_LIB_PATCH_VERSION}${MEDIAWIKI_LIB_SUFFIX_VERSION}")
set(MEDIAWIKI_LIB_VERSION_ID        "0x0${MEDIAWIKI_LIB_MAJOR_VERSION}0${MEDIAWIKI_LIB_MINOR_VERSION}0${MEDIAWIKI_LIB_PATCH_VERSION}")
set(MEDIAWIKI_LIB_SO_VERSION_STRING "${MEDIAWIKI_LIB_SO_CUR_VERSION}.${MEDIAWIKI_LIB_SO_REV_VERSION}.${MEDIAWIKI_LIB_SO_AGE_VERSION}")

# =======================================================

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

add_subdirectory(libmediawiki)
add_subdirectory(samplecode)

if(KDE4_BUILD_TESTS)
    MACRO_OPTIONAL_ADD_SUBDIRECTORY(tests)
endif()
