prefix=${CMAKE_INSTALL_PREFIX}
exec_prefix=${BIN_INSTALL_DIR}
libdir=${LIB_INSTALL_DIR}
includedir=${INCLUDE_INSTALL_DIR}

Name: libmediawiki
Description: a MediaWiki C++ interface for KDE.
URL: http://www.digikam.org
Requires:
Version: ${MEDIAWIKI_LIB_VERSION_STRING}
Libs: -L${LIB_INSTALL_DIR} -lmediawiki
Cflags: -I${INCLUDE_INSTALL_DIR}
