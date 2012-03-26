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

IF(KDE4_BUILD_TESTS)

    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../libmediawiki)

    MACRO(MEDIAWIKI_UNIT_TESTS)

        FOREACH(_testname ${ARGN})

            ADD_DEFINITIONS(-DTEST_DATA="\"${CMAKE_CURRENT_SOURCE_DIR}\"")

            KDE4_ADD_UNIT_TEST(${_testname} TESTNAME mediawiki-${_testname}
                                            NOGUI ${_testname}.cpp libmediawikitest/fakeserver.cpp
                              )

            TARGET_LINK_LIBRARIES(${_testname}
                                  ${KDE4_KIO_LIBS}
                                  ${QT_QTTEST_LIBRARY}
                                  mediawiki
                                 )

            SET_PROPERTY(TEST mediawiki-${_testname} PROPERTY LABELS library)

        ENDFOREACH(_testname)

    ENDMACRO(MEDIAWIKI_UNIT_TESTS)

    MEDIAWIKI_UNIT_TESTS(logintest
                         logouttest
                         queryimageinfotest
                         queryimagestest
                         querysiteinfousergroupstest
                         querysiteinfogeneraltest
                         mediawikitest
                         queryrevisiontest
                         queryinfotest
                         edittest
                         uploadtest
                         parsetest
                        )

    SET(ressources_RC
        queryrevisiontest_content.rc
        queryrevisiontest_onetitle.rc
        queryrevisiontest.rc
        queryrevisiontest_cuted.rc
        queryrevisiontest_timestamponly.rc
        queryrevisiontest_useronly.rc
        queryrevisiontest_parsetree.rc
        queryrevisiontest_rollback.rc
        parsetest.rc
       )

ENDIF(KDE4_BUILD_TESTS)
