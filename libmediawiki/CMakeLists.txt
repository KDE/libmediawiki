#
# Copyright (c) 2010-2014, Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/version.h.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/version.h)

set(mediawiki_LIB_SRCS image.cpp
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

target_link_libraries(mediawiki
                      ${KDE4_KIO_LIBS}
                      ${QT_QTCORE_LIBRARY}
                     )

set_target_properties(mediawiki PROPERTIES VERSION ${MEDIAWIKI_LIB_SO_VERSION_STRING}
                                SOVERSION ${MEDIAWIKI_LIB_SO_CUR_VERSION})

install(TARGETS mediawiki ${INSTALL_TARGETS_DEFAULT_ARGS})

# Check if compilation is done through digiKam SC or not. If yes, do not install header files.
if(NOT DIGIKAMSC_USE_PRIVATE_SHAREDLIBS)

    install(FILES image.h
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

endif()
