IF( KDE4_BUILD_TESTS )

    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../libmediawiki)

    MACRO( MEDIAWIKI_UNIT_TESTS )

    FOREACH( _testname ${ARGN} )

        ADD_DEFINITIONS( -DTEST_DATA="\"${CMAKE_CURRENT_SOURCE_DIR}\"" )
        KDE4_ADD_UNIT_TEST( ${_testname} TESTNAME mediawiki-${_testname}
                                         NOGUI ${_testname}.cpp libmediawikitest/fakeserver.cpp )
        TARGET_LINK_LIBRARIES( ${_testname} ${KDE4_KIO_LIBS} ${QT_QTTEST_LIBRARY} mediawiki -fprofile-arcs)
        SET_PROPERTY(TEST mediawiki-${_testname} PROPERTY LABELS library)

    ENDFOREACH( _testname )

    ENDMACRO( MEDIAWIKI_UNIT_TESTS )

    MEDIAWIKI_UNIT_TESTS(logintest
                         logouttest
                         queryimageinfotest
                         queryimagestest
                         querysiteinfousergroupstest
                         querysiteinfogeneraltest
                         mediawikitest
                         queryrevisiontest
                         queryinfotest
                         edittest
                         uploadtest
                         parsetest
                        )

    SET(ressources_RC
        queryrevisiontest_content.rc
        queryrevisiontest_onetitle.rc
        queryrevisiontest.rc
        queryrevisiontest_cuted.rc
        queryrevisiontest_timestamponly.rc
        queryrevisiontest_useronly.rc
        queryrevisiontest_parsetree.rc
        queryrevisiontest_rollback.rc
        parsetest.rc
       )

    INSTALL(FILES ${ressources_RC} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

ENDIF( KDE4_BUILD_TESTS )
