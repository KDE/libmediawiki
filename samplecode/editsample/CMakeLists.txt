PROJECT( editSample )

KDE4_ADD_UI_FILES (ui_src mainwindow.ui)

SET(editSample_SRCS
    main.cpp
    mainwindow.cpp
   )

KDE4_ADD_EXECUTABLE( editSample ${editSample_SRCS} ${ui_src} )
TARGET_LINK_LIBRARIES( editSample ${KDE4_KIO_LIBS} mediawiki)
