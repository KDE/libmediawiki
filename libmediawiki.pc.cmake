prefix=${CMAKE_INSTALL_PREFIX}
exec_prefix=${BIN_INSTALL_DIR}
libdir=${LIB_INSTALL_DIR}
includedir=${INCLUDE_INSTALL_DIR}

Name: libmediawiki
Description: a MediaWiki Qt/C++ interface for KDE.
URL: https://projects.kde.org/projects/extragear/libs/libmediawiki
Requires:
Version: ${MEDIAWIKI_LIB_VERSION_STRING}
Libs: -L${LIB_INSTALL_DIR} -lmediawiki
Cflags: -I${INCLUDE_INSTALL_DIR}
