set(mediawiki_SRCS
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

add_library(KF5MediaWiki ${mediawiki_SRCS})
generate_export_header(KF5MediaWiki BASE_NAME MediaWiki)
add_library(KF5::MediaWiki ALIAS KF5MediaWiki)

target_include_directories(KF5MediaWiki INTERFACE "$<INSTALL_INTERFACE:${KF5_INCLUDE_INSTALL_DIR}/MediaWiki>")

target_link_libraries(KF5MediaWiki
    PRIVATE
        Qt5::Core
        Qt5::Network
        KF5::CoreAddons
)

set_target_properties(KF5MediaWiki
    PROPERTIES VERSION ${MEDIAWIKI_VERSION_STRING}
    SOVERSION ${MEDIAWIKI_SOVERSION}
    EXPORT_NAME "MediaWiki"
)

ecm_generate_headers(MediaWiki_HEADERS
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
        ${INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
            ${CMAKE_CURRENT_BINARY_DIR}/mediawiki_export.h
            ${MediaWiki_HEADERS}
	    DESTINATION ${KF5_INCLUDE_INSTALL_DIR}/MediaWiki
        COMPONENT Devel)

include(ECMGeneratePriFile)
ecm_generate_pri_file(
    BASE_NAME MediaWiki
    LIB_NAME KF5MediaWiki
    DEPS "core network"
    FILENAME_VAR PRI_FILENAME
)
install(FILES ${PRI_FILENAME}
        DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
