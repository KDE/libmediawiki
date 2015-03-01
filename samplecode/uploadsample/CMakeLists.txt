#
# Copyright (c) 2010-2014, Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

project(uploadSample)

ki18n_wrap_ui(ui_src mainwindow.ui)

set(uploadSample_SRCS
    main.cpp
    mainwindow.cpp
   )

add_executable(uploadSample
               ${uploadSample_SRCS}
               ${ui_src}
               )

target_link_libraries(uploadSample
                      LINK_PRIVATE
                      KF5::KIOCore
                      mediawiki
                      )

