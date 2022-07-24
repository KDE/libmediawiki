#
# Copyright (c) 2010-2015, Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

find_package(Qt${QT_MAJOR_VERSION} ${QT_MIN_VERSION} CONFIG REQUIRED Test)

include(ECMMarkAsTest)

macro(mediawiki_executable_tests)
    foreach(_testname ${ARGN})
        add_executable(${_testname} ${_testname}.cpp libmediawikitest/fakeserver.cpp)
        target_link_libraries(${_testname} Qt::Core Qt::Test Qt::Network KF5::MediaWiki KF5::CoreAddons)
        ecm_mark_as_test(${_testname})
        add_test(NAME libmediawiki-${_testname} COMMAND ${_testname})
    endforeach(_testname)
endmacro()

mediawiki_executable_tests(
    edittest
    logintest
    logouttest
    mediawikitest
    parsetest
    queryimageinfotest
    queryimagestest
    queryinfotest
    queryrevisiontest
    querysiteinfogeneraltest
    querysiteinfousergroupstest
    uploadtest
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
    uploadtest_image.jpg
)

add_custom_target(TestResources ALL DEPENDS ${test_resources_dest})
