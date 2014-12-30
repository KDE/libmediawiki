#
# Copyright (c) 2010-2014, Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

if(KDE4_BUILD_TESTS)

    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../libmediawiki)

    macro(MEDIAWIKI_UNIT_TESTS)

        foreach(_testname ${ARGN})

            add_definitions(-DTEST_DATA="\"${CMAKE_CURRENT_SOURCE_DIR}\"")

            KDE4_ADD_UNIT_TEST(${_testname} TESTNAME mediawiki-${_testname}
                                            NOGUI ${_testname}.cpp libmediawikitest/fakeserver.cpp
                              )

            target_link_libraries(${_testname}
                                  LINK_PRIVATE
                                  ${KDE4_KIO_LIBS}
                                  ${QT_QTTEST_LIBRARY}
                                  mediawiki
                                 )

            set_property(TEST mediawiki-${_testname} PROPERTY LABELS library)

        endforeach()

    endmacro()

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

    macro(MEDIAWIKI_TEST_RESOURCES)

        foreach(resource ${ARGN})

            add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${resource}"
                               COMMAND ${CMAKE_COMMAND} -E copy
                               "${CMAKE_CURRENT_SOURCE_DIR}/${resource}"
                               "${CMAKE_CURRENT_BINARY_DIR}/${resource}"
                               DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${resource}")
            list(APPEND test_resources_dest "${resource}" )

        endforeach()

    endmacro()

    MEDIAWIKI_TEST_RESOURCES(
        parsetest.rc
        queryrevisiontest.rc
        queryrevisiontest_content.rc
        queryrevisiontest_cuted.rc
        queryrevisiontest_onetitle.rc
        queryrevisiontest_parsetree.rc
        queryrevisiontest_rollback.rc
        queryrevisiontest_timestamponly.rc
        queryrevisiontest_useronly.rc
    )

    add_custom_target(TestResources ALL DEPENDS ${test_resources_dest})

endif()
