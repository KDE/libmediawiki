# ===========================================================
#
# This file is a part of digiKam project
# <a href="http://www.digikam.org">http://www.digikam.org</a>
#
# @date   2011-03-24
# @brief  a MediaWiki C++ interface for KDE
#
# @author Copyright (C) 2011 by Gilles Caulier
#         <a href="mailto:caulier dot gilles at gmail dot com">caulier dot gilles at gmail dot com</a>
#
# This program is free software; you can redistribute it
# and/or modify it under the terms of the GNU General
# Public License as published by the Free Software Foundation;
# either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# ============================================================

PROJECT(libmediawiki)

MESSAGE(STATUS "----------------------------------------------------------------------------------")
MESSAGE(STATUS "Starting CMake configuration for: libmediawiki")

FIND_PACKAGE(KDE4 REQUIRED)

INCLUDE(KDE4Defaults)
INCLUDE(MacroLibrary)
INCLUDE(MacroOptionalAddSubdirectory)
INCLUDE(MacroOptionalFindPackage)
INCLUDE(FindPackageHandleStandardArgs)

ADD_DEFINITIONS (${QT_DEFINITIONS} ${QT_QTDBUS_DEFINITIONS} ${KDE4_DEFINITIONS})
INCLUDE_DIRECTORIES (${QDBUS_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})

SET(LIBMEDDIAWIKI_AREA_CODE_GENERAL 51007)
ADD_DEFINITIONS(-DKDE_DEFAULT_DEBUG_AREA=${LIBMEDIAWIKI_AREA_CODE_GENERAL})

# =======================================================
# Information to update before to release this library.

# Library version history:
# API      ABI
# 1.0.0 => 1.0.0  (released with KDE 4.7.0)

# Library API version
SET(MEDIAWIKI_LIB_MAJOR_VERSION "1")
SET(MEDIAWIKI_LIB_MINOR_VERSION "0")
SET(MEDIAWIKI_LIB_PATCH_VERSION "0")

# Suffix to add at end of version string. Usual values are:
# "-git"   : alpha code unstable from git. Do not use in production
# "-beta1" : beta1 release.
# "-beta2" : beta2 release.
# "-beta3" : beta3 release.
# "-rc"    : release candidate.
# ""       : final relase. Can be used in production.
SET(MEDIAWIKI_LIB_SUFFIX_VERSION "")

# Library ABI version used by linker.
# For details : http://www.gnu.org/software/libtool/manual/libtool.html#Updating-version-info
SET(MEDIAWIKI_LIB_SO_CUR_VERSION "1")
SET(MEDIAWIKI_LIB_SO_REV_VERSION "0")
SET(MEDIAWIKI_LIB_SO_AGE_VERSION "0")

# =======================================================
# Set env. variables accordinly.

SET(MEDIAWIKI_LIB_VERSION_STRING "${MEDIAWIKI_LIB_MAJOR_VERSION}.${MEDIAWIKI_LIB_MINOR_VERSION}.${MEDIAWIKI_LIB_PATCH_VERSION}${MEDIAWIKI_LIB_SUFFIX_VERSION}")
SET(MEDIAWIKI_LIB_VERSION_ID "0x0${MEDIAWIKI_LIB_MAJOR_VERSION}0${MEDIAWIKI_LIB_MINOR_VERSION}0${MEDIAWIKI_LIB_PATCH_VERSION}")

SET(MEDIAWIKI_LIB_SO_VERSION_STRING "${MEDIAWIKI_LIB_SO_CUR_VERSION}.${MEDIAWIKI_LIB_SO_REV_VERSION}.${MEDIAWIKI_LIB_SO_AGE_VERSION}")

# =======================================================

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

ADD_SUBDIRECTORY(tests)
ADD_SUBDIRECTORY(samplecode)
ADD_SUBDIRECTORY(libmediawiki)

IF(NOT WIN32)
  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/libmediawiki.pc.cmake ${CMAKE_CURRENT_BINARY_DIR}/libmediawiki.pc)
  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/libmediawiki.lsm.cmake ${CMAKE_CURRENT_BINARY_DIR}/libmediawiki.lsm)
  INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/libmediawiki.pc DESTINATION ${LIB_INSTALL_DIR}/pkgconfig )
ENDIF(NOT WIN32)

# Install find cmake script to the system for client applications.

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/FindMediawiki.cmake
        DESTINATION ${DATA_INSTALL_DIR}/cmake/modules)
