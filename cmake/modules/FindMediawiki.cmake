# - Try to find the Mediawiki library
#
# If you have put a local version of libmediawiki into your source tree,
# set MEDIAWIKI_LOCAL_DIR to the relative path to the local directory.
#
# Once done this will define
#
#  MEDIAWIKI_FOUND - system has libmediawiki
#  MEDIAWIKI_INCLUDE_DIR - the libmediawiki include directory
#  MEDIAWIKI_LIBRARIES - Link these to use libmediawiki
#  MEDIAWIKI_DEFINITIONS - Compiler switches required for using libmediawiki
#

# Copyright (c) 2011-2014, Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

if (MEDIAWIKI_INCLUDE_DIR AND MEDIAWIKI_LIBRARIES AND MEDIAWIKI_DEFINITIONS)

  message(STATUS "Found Mediawiki library in cache: ${MEDIAWIKI_LIBRARIES}")

  # in cache already
  set(MEDIAWIKI_FOUND TRUE)

else (MEDIAWIKI_INCLUDE_DIR AND MEDIAWIKI_LIBRARIES AND MEDIAWIKI_DEFINITIONS)

  message(STATUS "Check Mediawiki library in local sub-folder...")

  # Check if library is not in local sub-folder

  if (MEDIAWIKI_LOCAL_DIR)
    set (MEDIAWIKI_LOCAL_FOUND TRUE)
  else (MEDIAWIKI_LOCAL_DIR)
    find_file(MEDIAWIKI_LOCAL_FOUND libmediawiki/mediawiki.h ${CMAKE_SOURCE_DIR}/libmediawiki ${CMAKE_SOURCE_DIR}/libs/libmediawiki NO_DEFAULT_PATH)

    if (MEDIAWIKI_LOCAL_FOUND)
      # Was it found in libmediawiki/ or in libs/libmediawiki?
      find_file(MEDIAWIKI_LOCAL_FOUND_IN_LIBS libmediawiki/mediawiki.h ${CMAKE_SOURCE_DIR}/libs/libmediawiki NO_DEFAULT_PATH)
      if (MEDIAWIKI_LOCAL_FOUND_IN_LIBS)
        set(MEDIAWIKI_LOCAL_DIR libs/libmediawiki)
      else (MEDIAWIKI_LOCAL_FOUND_IN_LIBS)
        set(MEDIAWIKI_LOCAL_DIR libmediawiki)
      endif (MEDIAWIKI_LOCAL_FOUND_IN_LIBS)
    endif (MEDIAWIKI_LOCAL_FOUND)
  endif (MEDIAWIKI_LOCAL_DIR)

  if (MEDIAWIKI_LOCAL_FOUND)
    # we need two include directories: because the version.h file is put into the build directory
    # TODO MEDIAWIKI_INCLUDE_DIR sounds like it should contain only one directory...
    set(MEDIAWIKI_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/${MEDIAWIKI_LOCAL_DIR} ${CMAKE_BINARY_DIR}/${MEDIAWIKI_LOCAL_DIR})
    set(MEDIAWIKI_DEFINITIONS "-I${CMAKE_SOURCE_DIR}/${MEDIAWIKI_LOCAL_DIR}" "-I${CMAKE_BINARY_DIR}/${MEDIAWIKI_LOCAL_DIR}")
    set(MEDIAWIKI_LIBRARIES mediawiki)
    message(STATUS "Found Mediawiki library in local sub-folder: ${CMAKE_SOURCE_DIR}/${MEDIAWIKI_LOCAL_DIR}")
    set(MEDIAWIKI_FOUND TRUE)
    mark_as_advanced(MEDIAWIKI_INCLUDE_DIR MEDIAWIKI_LIBRARIES MEDIAWIKI_DEFINITIONS)

  else (MEDIAWIKI_LOCAL_FOUND)

    if (NOT WIN32)
      message(STATUS "Check Mediawiki library using pkg-config...")

      # use pkg-config to get the directories and then use these values
      # in the FIND_PATH() and FIND_LIBRARY() calls
      include(UsePkgConfig)

      PKGCONFIG(libmediawiki _MEDIAWIKIIncDir _MEDIAWIKILinkDir _MEDIAWIKILinkFlags _MEDIAWIKICflags)

      if (_MEDIAWIKILinkFlags)
        # query pkg-config asking for a libmediawiki >= 1.0.0
        exec_program(${PKGCONFIG_EXECUTABLE} ARGS --atleast-version=1.0.0 libmediawiki RETURN_VALUE _return_VALUE OUTPUT_VARIABLE _pkgconfigDevNull )
        if (_return_VALUE STREQUAL "0")
            message(STATUS "Found libmediawiki release >= 1.0.0")
            set(MEDIAWIKI_VERSION_GOOD_FOUND TRUE)
        else (_return_VALUE STREQUAL "0")
            message(STATUS "Found libmediawiki release < 1.0.0, too old")
            set(MEDIAWIKI_VERSION_GOOD_FOUND FALSE)
            set(MEDIAWIKI_FOUND FALSE)
        endif (_return_VALUE STREQUAL "0")
      else (_MEDIAWIKILinkFlags)
        set(MEDIAWIKI_VERSION_GOOD_FOUND FALSE)
        set(MEDIAWIKI_FOUND FALSE)
      endif (_MEDIAWIKILinkFlags)
    else (NOT WIN32)
      set(MEDIAWIKI_VERSION_GOOD_FOUND TRUE)
    endif (NOT WIN32)
    if (MEDIAWIKI_VERSION_GOOD_FOUND)
        set(MEDIAWIKI_DEFINITIONS ${_MEDIAWIKICflags})

        find_path(MEDIAWIKI_INCLUDE_DIR NAMES libmediawiki/version.h PATHS ${KDE4_INCLUDE_DIR} ${_MEDIAWIKIIncDir})
        find_library(MEDIAWIKI_LIBRARIES NAMES mediawiki PATHS ${KDE4_LIB_DIR} ${_MEDIAWIKILinkDir})

        if (MEDIAWIKI_INCLUDE_DIR AND MEDIAWIKI_LIBRARIES)
            set(MEDIAWIKI_FOUND TRUE)
        endif (MEDIAWIKI_INCLUDE_DIR AND MEDIAWIKI_LIBRARIES)
      endif (MEDIAWIKI_VERSION_GOOD_FOUND)
      if (MEDIAWIKI_FOUND)
          if (NOT Mediawiki_FIND_QUIETLY)
              message(STATUS "Found libmediawiki: ${MEDIAWIKI_LIBRARIES}")
          endif (NOT Mediawiki_FIND_QUIETLY)
      else (MEDIAWIKI_FOUND)
          if (Mediawiki_FIND_REQUIRED)
              if (NOT MEDIAWIKI_INCLUDE_DIR)
                  message(FATAL_ERROR "Could NOT find libmediawiki header files")
              endif (NOT MEDIAWIKI_INCLUDE_DIR)
              if (NOT MEDIAWIKI_LIBRARIES)
                  message(FATAL_ERROR "Could NOT find libmediawiki library")
              endif (NOT MEDIAWIKI_LIBRARIES)
          endif (Mediawiki_FIND_REQUIRED)
      endif (MEDIAWIKI_FOUND)

    mark_as_advanced(MEDIAWIKI_INCLUDE_DIR MEDIAWIKI_LIBRARIES MEDIAWIKI_DEFINITIONS)

  endif (MEDIAWIKI_LOCAL_FOUND)

endif (MEDIAWIKI_INCLUDE_DIR AND MEDIAWIKI_LIBRARIES AND MEDIAWIKI_DEFINITIONS)
