# ===========================================================
#
# This file is a part of KDE project
# <a href="https://projects.kde.org/projects/extragear/libs/libmediawiki">libmediawiki</a>
#
# @date   2011-03-24
# @brief  a MediaWiki C++ interface for KDE
#
# @author Copyright (C) 2011-2012 by Gilles Caulier
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

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/version.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/version.h)
IF( KDE4_BUILD_TESTS )
	set( CMAKE_CXX_FLAGS "-fprofile-arcs -ftest-coverage")
ENDIF( KDE4_BUILD_TESTS )
SET(mediawiki_LIB_SRCS image.cpp
                       imageinfo.cpp
                       mediawiki.cpp
                       login.cpp
                       logout.cpp
                       queryimageinfo.cpp
                       queryimages.cpp
                       querysiteinfogeneral.cpp
                       generalinfo.cpp
                       querysiteinfousergroups.cpp
                       queryrevision.cpp
                       queryinfo.cpp
                       edit.cpp
                       job.cpp
                       usergroup.cpp
                       revision.cpp
                       page.cpp
                       parse.cpp
                       protection.cpp
                       upload.cpp
   )

KDE4_ADD_LIBRARY(mediawiki SHARED ${mediawiki_LIB_SRCS})

TARGET_LINK_LIBRARIES(mediawiki
                      ${KDE4_KIO_LIBS}
                      ${QT_QTCORE_LIBRARY}
                     )

SET_TARGET_PROPERTIES(mediawiki PROPERTIES VERSION ${MEDIAWIKI_LIB_SO_VERSION_STRING}
                                SOVERSION ${MEDIAWIKI_LIB_SO_CUR_VERSION})

INSTALL(TARGETS mediawiki ${INSTALL_TARGETS_DEFAULT_ARGS})

INSTALL(FILES image.h
              imageinfo.h
              mediawiki.h
              login.h
              logout.h
              queryimageinfo.h
              queryimages.h
              querysiteinfogeneral.h
              generalinfo.h
              querysiteinfousergroups.h
              queryrevision.h
              queryinfo.h
              edit.h
              job.h
              usergroup.h
              revision.h
              page.h
              parse.h
              protection.h
              upload.h
              mediawiki_export.h
              ${CMAKE_CURRENT_BINARY_DIR}/version.h
        DESTINATION ${INCLUDE_INSTALL_DIR}/libmediawiki COMPONENT Devel)
