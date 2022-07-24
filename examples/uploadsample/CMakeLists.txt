#
# Copyright (c) 2010-2014, Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

cmake_minimum_required(VERSION 3.16.0)

project(UploadSample)

find_package(Qt${QT_MAJOR_VERSION} 5.15.0 NO_MODULE REQUIRED Core Network Widgets)
find_package(KF5MediaWiki REQUIRED)
find_package(KF5CoreAddons REQUIRED NO_MODULE)

set(CMAKE_AUTOMOC ON)

add_executable(
    UploadSample
    main.cpp
    mainwindow.cpp
)

target_link_libraries(UploadSample
    PRIVATE
        Qt::Core
        Qt::Network
        Qt::Widgets
        KF5::MediaWiki
        KF5::CoreAddons
)
set_target_properties(UploadSample PROPERTIES COMPILE_FLAGS "-std=c++11")
