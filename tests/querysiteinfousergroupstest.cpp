/** ===========================================================
 * @file
 *
 * This file is a part of KDE project
 * <a href="https://projects.kde.org/projects/extragear/libs/libmediawiki">libmediawiki</a>
 *
 * @date   2011-03-22
 * @brief  a MediaWiki C++ interface for KDE
 *
 * @author Copyright (C) 2010 by Ludovic Delfau
 *         <a href="mailto:ludovicdelfau at gmail dot com">ludovicdelfau at gmail dot com</a>
 * @author Copyright (C) 2010 by Hormiere Guillaume
 *         <a href="mailto:hormiere dot guillaume at gmail dot com">hormiere dot guillaume at gmail dot com</a>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtTest/QTest>

#include <KDE/KJob>

#include "libmediawikitest/fakeserver.h"

#include "mediawiki.h"
#include "querysiteinfousergroups.h"
#include "usergroup.h"

using mediawiki::MediaWiki;
using mediawiki::QuerySiteinfoUsergroups;
using mediawiki::UserGroup;

Q_DECLARE_METATYPE(QList<UserGroup>)

class QuerySiteinfoUsergroupsTest : public QObject
{
    Q_OBJECT

public Q_SLOTS:

    void usergroupsHandle(const QList<UserGroup> & usergroups) {
        ++usergroupsCount;
        usergroupsResults = usergroups;
    }

private Q_SLOTS:

    void init() {
        usergroupsCount = 0;
    }

    void testResult() {
        QFETCH(QString, scenario);
        QFETCH(bool, includeNumber);
        QFETCH(int, error);
        QFETCH(QList<UserGroup>, results);

        FakeServer fakeserver;
        fakeserver.setScenario(scenario);
        fakeserver.startAndWait();

        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        QuerySiteinfoUsergroups * job = new QuerySiteinfoUsergroups(mediawiki);

        job->setIncludeNumber(includeNumber);

        connect(job, SIGNAL(usergroups(QList<UserGroup>)), this, SLOT(usergroupsHandle(QList<UserGroup>)));

        job->exec();

        QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 1);

        FakeServer::Request request = requests[0];
        QCOMPARE(request.agent, mediawiki.userAgent());
        QCOMPARE(request.type, QString("GET"));
        if (includeNumber) {
            QCOMPARE(request.value, QString("?format=xml&action=query&meta=siteinfo&siprop=usergroups&sinumberingroup="));
        } else {
            QCOMPARE(request.value, QString("?format=xml&action=query&meta=siteinfo&siprop=usergroups"));
        }

        QCOMPARE(job->error(), error);

        QCOMPARE(usergroupsCount, 1);

        QCOMPARE(usergroupsResults, results);

        QVERIFY(fakeserver.isAllScenarioDone());
    }

    void testResult_data() {           
        QTest::addColumn<QString>("scenario");
        QTest::addColumn<bool>("includeNumber");
        QTest::addColumn<int>("error");
        QTest::addColumn< QList<UserGroup> >("results");

        UserGroup ug1,ug2,ug3;
        ug1.setName("name_1");
        ug1.setRights(QList<QString>());
        ug2.setName("name_2");
        ug2.setRights(QList<QString>());
        ug3.setName("name_3");

        QTest::newRow("No group")
                << "<?xml version=\"1.0\"?><api><query><usergroups></usergroups></query></api>"
                << false
                << int(KJob::NoError)
                << QList<UserGroup>();

        QTest::newRow("One group with no right")
                << "<?xml version=\"1.0\"?><api><query><usergroups><group name=\"name_1\"/></usergroups></query></api>"
                << false
                << int(KJob::NoError)
                << (QList<UserGroup>() << ug1);

        ug1.setRights(ug1.rights() << "permission_1_1");
        QTest::newRow("One group with one right")
                << "<?xml version=\"1.0\"?><api><query><usergroups><group name=\"name_1\"><rights><permission>permission_1_1</permission></rights></group></usergroups></query></api>"
                << false
                << int(KJob::NoError)
                << (QList<UserGroup>() << ug1);

        ug1.setRights(ug1.rights() << "permission_1_2");
        QTest::newRow("One group with two rights")
                << "<?xml version=\"1.0\"?><api><query><usergroups><group name=\"name_1\"><rights><permission>permission_1_1</permission><permission>permission_1_2</permission></rights></group></usergroups></query></api>"
                << false
                << int(KJob::NoError)
                << (QList<UserGroup>() << ug1);

        ug1.setRights(QList<QString>());
        QTest::newRow("Two groups with group one no right and group two no right")
                << "<?xml version=\"1.0\"?><api><query><usergroups><group name=\"name_1\"></group><group name=\"name_2\" /></usergroups></query></api>"
                << false
                << int(KJob::NoError)
                << (QList<UserGroup>() << ug1 << ug2);


        ug2.setRights(ug2.rights() << "permission_2_1");
        QTest::newRow("Two groups with group one no right and group two one right")
                << "<?xml version=\"1.0\"?><api><query><usergroups><group name=\"name_1\"></group><group name=\"name_2\"><rights><permission>permission_2_1</permission></rights></group></usergroups></query></api>"
                << false
                << int(KJob::NoError)
                << (QList<UserGroup>() << ug1 << ug2);


        ug2.setRights(ug2.rights() << "permission_2_2");
        QTest::newRow("Two groups with group one no right and group two two rights")
                << "<?xml version=\"1.0\"?><api><query><usergroups><group name=\"name_1\"></group><group name=\"name_2\"><rights><permission>permission_2_1</permission><permission>permission_2_2</permission></rights></group></usergroups></query></api>"
                << false
                << int(KJob::NoError)
                << (QList<UserGroup>() << ug1 << ug2);


        ug1.setRights(ug1.rights() << "permission_1_1");
        ug2.setRights(QList<QString>());
        QTest::newRow("Two groups with group one one right and group two no right")
                << "<?xml version=\"1.0\"?><api><query><usergroups><group name=\"name_1\"><rights><permission>permission_1_1</permission></rights></group><group name=\"name_2\"><rights></rights></group></usergroups></query></api>"
                << false
                << int(KJob::NoError)
                << (QList<UserGroup>() << ug1 << ug2);


        ug2.setRights(ug2.rights() << "permission_2_1");
        QTest::newRow("Two groups with group one one right and group two one right")
                << "<?xml version=\"1.0\"?><api><query><usergroups><group name=\"name_1\"><rights><permission>permission_1_1</permission></rights></group><group name=\"name_2\"><rights><permission>permission_2_1</permission></rights></group></usergroups></query></api>"
                << false
                << int(KJob::NoError)
                << (QList<UserGroup>() << ug1 << ug2);


        ug2.setRights(ug2.rights() << "permission_2_2");
        QTest::newRow("Two groups with group one one right and group two two rights")
                << "<?xml version=\"1.0\"?><api><query><usergroups><group name=\"name_1\"><rights><permission>permission_1_1</permission></rights></group><group name=\"name_2\"><rights><permission>permission_2_1</permission><permission>permission_2_2</permission></rights></group></usergroups></query></api>"
                << false
                << int(KJob::NoError)
                << (QList<UserGroup>() << ug1 << ug2);


        ug1.setRights(ug1.rights() << "permission_1_2");
        ug2.setRights(QList<QString>());
        QTest::newRow("Two groups with group one two rights and group two no right")
                << "<?xml version=\"1.0\"?><api><query><usergroups><group name=\"name_1\"><rights><permission>permission_1_1</permission><permission>permission_1_2</permission></rights></group><group name=\"name_2\"><rights /></group></usergroups></query></api>"
                << false
                << int(KJob::NoError)
                << (QList<UserGroup>() << ug1 << ug2);


        ug2.setRights(ug2.rights() << "permission_2_1");
        QTest::newRow("Two groups with group one two rights and group two one right")
                << "<?xml version=\"1.0\"?><api><query><usergroups><group name=\"name_1\"><rights><permission>permission_1_1</permission><permission>permission_1_2</permission></rights></group><group name=\"name_2\"><rights><permission>permission_2_1</permission></rights></group></usergroups></query></api>"
                << false
                << int(KJob::NoError)
                << (QList<UserGroup>() << ug1 << ug2);


        ug2.setRights(ug2.rights() << "permission_2_2");
        QTest::newRow("Two groups with group one two rights and group two two rights")
                << "<?xml version=\"1.0\"?><api><query><usergroups><group name=\"name_1\"><rights><permission>permission_1_1</permission><permission>permission_1_2</permission></rights></group><group name=\"name_2\"><rights><permission>permission_2_1</permission><permission>permission_2_2</permission></rights></group></usergroups></query></api>"
                << false
                << int(KJob::NoError)
                << (QList<UserGroup>() << ug1 << ug2);


        QTest::newRow("No group with include number")
                << "<?xml version=\"1.0\"?><api><query><usergroups></usergroups></query></api>"
                << true
                << int(KJob::NoError)
                << QList<UserGroup>();

        ug1.setRights(QList<QString>());
        ug1.setNumber(0);
        QTest::newRow("One group with include number")
                << "<?xml version=\"1.0\"?><api><query><usergroups><group name=\"name_1\" number=\"0\"/></usergroups></query></api>"
                << true
                << int(KJob::NoError)
                << (QList<UserGroup>() << ug1);

        ug1.setNumber(41);
        ug2.setRights(QList<QString>());
        ug2.setNumber(12543);
        QTest::newRow("Two groups with include number")
                << "<?xml version=\"1.0\"?><api><query><usergroups><group name=\"name_1\" number=\"41\"></group><group name=\"name_2\" number=\"12543\" /></usergroups></query></api>"
                << true
                << int(KJob::NoError)
                << (QList<UserGroup>() << ug1 << ug2);

        ug1.setRights(QList<QString>()<< "permission_1_1");
        ug1.setNumber(1781);
        ug2.setRights(QList<QString>() << "permission_2_1" << "permission_2_2" << "permission_2_3");
        ug2.setNumber(10989982);
        ug3.setRights(QList<QString>() << "permission_3_1" << "permission_3_2");
        ug3.setNumber(6);
        QTest::newRow("Three groups with rights and include number")
                << "<?xml version=\"1.0\"?><api><query><usergroups><group name=\"name_1\" number=\"1781\"><rights><permission>permission_1_1</permission></rights></group><group name=\"name_2\" number=\"10989982\"><rights><permission>permission_2_1</permission><permission>permission_2_2</permission><permission>permission_2_3</permission></rights></group><group name=\"name_3\" number=\"6\"><rights><permission>permission_3_1</permission><permission>permission_3_2</permission></rights></group></usergroups></query></api>"
                << true
                << int(KJob::NoError)
                << (QList<UserGroup>() << ug1 << ug2 << ug3);
    }

private:

    int              usergroupsCount;
    QList<UserGroup> usergroupsResults;
};

QTEST_MAIN(QuerySiteinfoUsergroupsTest)

#include "querysiteinfousergroupstest.moc"
