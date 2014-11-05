#
# Copyright (c) 2010-2014, Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

set(CTEST_SOURCE_DIRECTORY "$ENV{MWSD}")
set(CTEST_BINARY_DIRECTORY "$ENV{MWBD}")

include(${CTEST_SOURCE_DIRECTORY}libmediawiki/CTestConfig.cmake)

set(WITH_MEMCHECK TRUE)
set(WITH_COVERAGE TRUE)

#######################################################################

find_program(CTEST_GIT_COMMAND      NAMES git)
find_program(CTEST_COVERAGE_COMMAND NAMES gcov)

#if(NOT EXISTS "${CTEST_SOURCE_DIRECTORY}")
#   set(CTEST_CHECKOUT_COMMAND "${CTEST_GIT_COMMAND} pull ${CTEST_SOURCE_DIRECTORY}")
#endif()

#set(CTEST_UPDATE_COMMAND "${CTEST_GIT_COMMAND}")

set(CTEST_CONFIGURE_COMMAND "${CMAKE_COMMAND} -DWITH_TESTING:BOOL=ON ${CTEST_BUILD_OPTIONS}")
set(CTEST_CONFIGURE_COMMAND "${CTEST_CONFIGURE_COMMAND} \"-G${CTEST_CMAKE_GENERATOR}\"")
set(CTEST_CONFIGURE_COMMAND "${CTEST_CONFIGURE_COMMAND} \"${CTEST_SOURCE_DIRECTORY}\"")

ctest_start("Nightly")
#ctest_update()
ctest_configure()
ctest_build()
ctest_test()

if(WITH_COVERAGE AND CTEST_COVERAGE_COMMAND)
    ctest_coverage()
endif()

#ctest_submit()
