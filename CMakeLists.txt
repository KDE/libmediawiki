#
# Copyright (c) 2010-2016, Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

project(MediaWiki)

message(STATUS "----------------------------------------------------------------------------------")
message(STATUS "Starting CMake configuration for: ${PROJECT_NAME}")

set(CMAKE_MIN_VERSION   "2.8.12")
set(ECM_MIN_VERSION     "1.5.0")
set(KF5_VERSION         "5.5.0")
set(REQUIRED_QT_VERSION "5.2.0")

# Used to set library's SONAME, change first digit on API/ABI break
set(LIBMEDIAWIKI_LIBVERSION         "5.37.0")

cmake_minimum_required(VERSION ${CMAKE_MIN_VERSION})

############## ECM setup #################################################################################################################

find_package(ECM ${ECM_MIN_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(ECMGenerateHeaders)
include(ECMGeneratePriFile)
include(ECMSetupVersion)
include(ECMMarkNonGuiExecutable)
include(ECMOptionalAddSubdirectory)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings)
include(ECMPackageConfigHelpers)
include(GenerateExportHeader)
include(FeatureSummary)

ecm_setup_version(${LIBMEDIAWIKI_LIBVERSION}
    VARIABLE_PREFIX MEDIAWIKI
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/mediawiki_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5MediaWikiConfigVersion.cmake"
)

# Required Qt5 components to build this framework
find_package(Qt5 ${REQUIRED_QT_VERSION} NO_MODULE REQUIRED Core Network)
find_package(KF5CoreAddons ${KF5_VERSION} REQUIRED)

# Remove Wsuggest-override GCC flag which generate a lots of compile warnings
while(CMAKE_CXX_FLAGS MATCHES "-Wsuggest-override")
    string(REPLACE "-Wsuggest-override" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endwhile()

# Subdirectories
add_subdirectory(src)
add_subdirectory(autotests)
add_subdirectory(tests)

# Create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/KF5MediaWiki")

ecm_configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KF5MediaWikiConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5MediaWikiConfig.cmake"
    INSTALL_DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
)

install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/KF5MediaWikiConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/KF5MediaWikiConfigVersion.cmake"
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel)

install(EXPORT KF5MediaWikiTargets
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        FILE KF5MediaWikiTargets.cmake
        NAMESPACE KF5::)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/mediawiki_version.h"
        DESTINATION "${KF5_INCLUDE_INSTALL_DIR}"
        COMPONENT Devel)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
