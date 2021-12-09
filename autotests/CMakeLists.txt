find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED Test)

include(ECMMarkAsTest)

macro(mediawiki_tests)
    foreach(_testname ${ARGN})
        add_executable(${_testname} ${_testname}.cpp)
        target_link_libraries(${_testname} Qt5::Test KF5MediaWiki)
        add_test(mediawiki-${_testname} ${_testname})
        ecm_mark_as_test(${_testname})
    endforeach(_testname)
endmacro()

#mediawiki_tests(
#    mediawikitest
#)
