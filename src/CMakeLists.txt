add_library(KF5MediaWiki)
add_library(KF5::MediaWiki ALIAS KF5MediaWiki)
set_target_properties(KF5MediaWiki
    PROPERTIES VERSION ${MEDIAWIKI_VERSION}
    SOVERSION ${MEDIAWIKI_SOVERSION}
    EXPORT_NAME "MediaWiki"
)

target_sources(KF5MediaWiki PRIVATE
    image.cpp
    imageinfo.cpp
    mediawiki.cpp
    login.cpp
    logout.cpp
    queryimageinfo.cpp
    queryimages.cpp
    querysiteinfogeneral.cpp
    generalinfo.cpp
    querysiteinfousergroups.cpp
    queryrevision.cpp
    queryinfo.cpp
    edit.cpp
    job.cpp
    usergroup.cpp
    revision.cpp
    page.cpp
    parse.cpp
    protection.cpp
    upload.cpp
)

generate_export_header(KF5MediaWiki BASE_NAME MediaWiki)

target_include_directories(KF5MediaWiki INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/MediaWiki>")

target_link_libraries(KF5MediaWiki
    PUBLIC
        Qt${QT_MAJOR_VERSION}::Core
        Qt${QT_MAJOR_VERSION}::Network
        KF5::CoreAddons
)

ecm_generate_headers(MediaWiki_CamelCase_HEADERS

                     HEADER_NAMES
                     Edit
                     Generalinfo
                     Image
                     Imageinfo
                     Job
                     Login
                     Logout
                     MediaWiki
                     Page
                     Parse
                     Protection
                     QueryImageinfo
                     QueryImages
                     QueryInfo
                     QueryRevision
                     QuerySiteInfoGeneral
                     QuerySiteinfoUsergroups
                     Revision
                     Upload
                     UserGroup

                     REQUIRED_HEADERS MediaWiki_HEADERS
)

# Install files
install(TARGETS KF5MediaWiki
        EXPORT KF5MediaWikiTargets
        ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
            ${CMAKE_CURRENT_BINARY_DIR}/mediawiki_export.h
            ${MediaWiki_HEADERS}
            ${MediaWiki_CamelCase_HEADERS}
        DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/MediaWiki
        COMPONENT Devel)

include(ECMGeneratePriFile)

ecm_generate_pri_file(
    BASE_NAME MediaWiki
    LIB_NAME KF5MediaWiki
    DEPS "core network KCoreAddons"
    FILENAME_VAR PRI_FILENAME
)

install(FILES ${PRI_FILENAME}
        DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
