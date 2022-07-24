#
# Copyright (c) 2010-2016, Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

cmake_minimum_required(VERSION 3.16.0)

# Used to set library's SONAME, change first digit on API/ABI break
set(LIBMEDIAWIKI_LIBVERSION         "5.38.0")

project(MediaWiki VERSION ${LIBMEDIAWIKI_LIBVERSION})

message(STATUS "----------------------------------------------------------------------------------")
message(STATUS "Starting CMake configuration for: ${PROJECT_NAME}")

set(KF_MIN_VERSION "5.85.0")
set(QT_MIN_VERSION "5.15.0")

############## ECM setup #################################################################################################################

find_package(ECM ${KF_MIN_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(ECMGenerateHeaders)
include(ECMGeneratePriFile)
include(ECMSetupVersion)
include(ECMMarkNonGuiExecutable)
include(ECMOptionalAddSubdirectory)
include(CMakePackageConfigHelpers)
include(GenerateExportHeader)
include(FeatureSummary)

# Required Qt components to build this framework
find_package(Qt${QT_MAJOR_VERSION} ${QT_MIN_VERSION} NO_MODULE REQUIRED Core Network)
find_package(KF5CoreAddons ${KF_MIN_VERSION} REQUIRED)

if (QT_MAJOR_VERSION EQUAL "6")
    find_package(Qt6Core5Compat ${QT_MIN_VERSION} REQUIRED)
endif()

ecm_setup_version(${LIBMEDIAWIKI_LIBVERSION}
    VARIABLE_PREFIX MEDIAWIKI
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/mediawiki_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5MediaWikiConfigVersion.cmake"
)

# added with KDE_COMPILERSETTINGS_LEVEL 5.85.0
remove_definitions(
    -DQT_NO_KEYWORDS
    -DQT_NO_FOREACH
)

# Subdirectories
add_subdirectory(src)
add_subdirectory(autotests)
add_subdirectory(tests)

# Create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5MediaWiki")

configure_package_config_file(
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
        DESTINATION "${KDE_INSTALL_INCLUDEDIR_KF}"
        COMPONENT Devel)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
