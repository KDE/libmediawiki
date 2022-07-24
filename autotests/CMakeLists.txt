find_package(Qt${QT_MAJOR_VERSION} ${QT_MIN_VERSION} CONFIG REQUIRED Test)

include(ECMMarkAsTest)

macro(mediawiki_tests)
    foreach(_testname ${ARGN})
        add_executable(${_testname} ${_testname}.cpp)
        target_link_libraries(${_testname} Qt::Test KF5MediaWiki)
        add_test(NAME mediawiki-${_testname} COMMAND ${_testname})
        ecm_mark_as_test(${_testname})
    endforeach(_testname)
endmacro()

#mediawiki_tests(
#    mediawikitest
#)
