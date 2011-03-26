PROJECT( uploadSample )
KDE4_ADD_UI_FILES (ui_src mainwindow.ui)

SET(uploadSample_SRCS
    main.cpp
    mainwindow.cpp
   )

KDE4_ADD_EXECUTABLE( uploadSample ${uploadSample_SRCS} ${ui_src} )
TARGET_LINK_LIBRARIES( uploadSample ${KDE4_KIO_LIBS} mediawiki)
