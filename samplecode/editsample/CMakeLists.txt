#
# Copyright (c) 2010-2014, Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

project(editSample)

ki18n_wrap_ui(ui_src mainwindow.ui)

set(editSample_SRCS
    main.cpp
    mainwindow.cpp
   )

add_executable(editSample
               ${editSample_SRCS}
               ${ui_src}
               )

target_link_libraries(editSample
                      LINK_PRIVATE
                      KF5::KIOCore
                      mediawiki
                     )
