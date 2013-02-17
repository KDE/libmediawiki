/** ===========================================================
 * @file
 *
 * This file is a part of KDE project
 * <a href="https://projects.kde.org/projects/extragear/libs/libmediawiki">libmediawiki</a>
 *
 * @date   2011-03-22
 * @brief  a MediaWiki C++ interface for KDE
 *
 * @author Copyright (C) 2010 by Alexandre Mendes
 *         <a href="mailto:alex dot mendes1988 at gmail dot com">alex dot mendes1988 at gmail dot com</a>
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

#ifndef TEST_QUERYINFO_H
#define TEST_QUERYINFO_H

#include <QObject>
#include <QtTest/QtTest>

#include <kjob.h>
#include <kdebug.h>

#include "mediawiki.h"
#include "queryinfo.h"
#include "page.h"
#include "protection.h"
#include "libmediawikitest/fakeserver.h"

using mediawiki::MediaWiki;
using mediawiki::QueryInfo;
using mediawiki::Page;
using mediawiki::Protection;

Q_DECLARE_METATYPE(Page)
Q_DECLARE_METATYPE(Protection)
Q_DECLARE_METATYPE(QueryInfo*)
Q_DECLARE_METATYPE(QVector <Protection>)

void debugPages(Page p)
{
    kDebug() << p.pageId();
    kDebug() << p.pageTitle();
    kDebug() << p.pageNs();
    kDebug() << p.pageLastRevId();
    kDebug() << p.pageCounter();
    kDebug() << p.pageLength();
    kDebug() << p.pageEditToken();
    kDebug() << p.pageTalkid();
    kDebug() << p.pageFullurl();
    kDebug() << p.pageEditurl();
    kDebug() << p.pageReadable();
    kDebug() << p.pagePreload();
    kDebug() << p.pageTouched();
    kDebug() << p.pageStarttimestamp();
}

void debugProtection(QVector <Protection> p)
{
    foreach(const Protection& pr, p)
    {
        kDebug() << pr.type();
        kDebug() << pr.level();
        kDebug() << pr.expiry();
        kDebug() << pr.source();
    }
}

class QueryInfoTest : public QObject
{
    Q_OBJECT

public Q_SLOTS:

    void queryInfoHandlePages(const Page& page)
    {
        ++queryInfoCount;
        queryInfoResultsPage = page;
    }

    void queryInfoHandleProtection(const QVector<Protection>& protection)
    {
        ++queryInfoCount;
        queryInfoResultsProtections = protection;
    }

private Q_SLOTS:

    void initTestCase()
    {
        queryInfoCount    = 0;
        this->m_mediaWiki = new MediaWiki(QUrl("http://127.0.0.1:12566"));
    }

    void constructQuery()
    {
        QFETCH(QString, request);
        QFETCH(QueryInfo*, job);

        queryInfoCount = 0;
        FakeServer fakeserver;
        fakeserver.startAndWait();

        job->exec();

        QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 1);

        FakeServer::Request requestServeur = requests[0];
        QCOMPARE(requestServeur.agent, m_mediaWiki->userAgent());
        QCOMPARE(requestServeur.type, QString("GET"));
        QCOMPARE(requestServeur.value, request);
    }

    void constructQuery_data()
    {
        QTest::addColumn<QString>("request");
        QTest::addColumn<QueryInfo*>("job");

        QueryInfo* const j1 = new QueryInfo(*m_mediaWiki);
        j1->setPageName("API");

        QTest::newRow("Name pages")
                << QString("?format=xml&action=query&prop=info&inprop=protection|talkid|watched|subjectid|url|readable|preload&titles=API")
                << j1;

        QueryInfo* const j2 = new QueryInfo(*m_mediaWiki);
        j2->setToken( "cecded1f35005d22904a35cc7b736e18+\\" );

        QTest::newRow("Token")
                << QString("?format=xml&action=query&prop=info&inprop=protection|talkid|watched|subjectid|url|readable|preload&intoken=cecded1f35005d22904a35cc7b736e18+\\")
                << j2;

        QueryInfo* const j3 = new QueryInfo(*m_mediaWiki);
        j3->setPageId(25255);

        QTest::newRow("Page Id")
                << QString("?format=xml&action=query&prop=info&inprop=protection|talkid|watched|subjectid|url|readable|preload&pageids=25255")
                << j3;

        QueryInfo *j4 = new QueryInfo(*m_mediaWiki);
        j4->setRevisionId(44545);

        QTest::newRow("Revision Id")
                << QString("?format=xml&action=query&prop=info&inprop=protection|talkid|watched|subjectid|url|readable|preload&revids=44545")
                << j4;
    }

    void parseData()
    {
        QFETCH(QString,scenario);
        QFETCH(Page ,page);
        QFETCH(QVector<Protection> ,protections);

        QueryInfo job(*m_mediaWiki);
        queryInfoCount = 0;
        FakeServer fakeserver;
        fakeserver.addScenario(scenario);
        fakeserver.startAndWait();

        connect(&job, SIGNAL(page(Page)),
                this,SLOT(queryInfoHandlePages(Page)));

        connect(&job, SIGNAL(protection(QVector<Protection>)),
                this,SLOT(queryInfoHandleProtection(QVector<Protection>)));

        job.exec();

        QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), 1);

        QCOMPARE(queryInfoCount, 2);
        QCOMPARE(queryInfoResultsPage, page);
        QCOMPARE(queryInfoResultsProtections, protections);
        QVERIFY(fakeserver.isAllScenarioDone());
    }

    void parseData_data()
    {
        QTest::addColumn<QString>("scenario");
        QTest::addColumn< Page >("page");
        QTest::addColumn< QVector<Protection> >("protections");

        Protection pr1;
        pr1.setType("edit");
        pr1.setLevel("sysop");
        pr1.setExpiry("infinity");
        pr1.setSource("");

        Protection pr2;
        pr2.setType("move");
        pr2.setLevel("sysop");
        pr2.setExpiry("infinity");
        pr2.setSource("");

        Page page;
        page.setPageId(27697087);
        page.setTitle("API");
        page.setNs(0);
        page.setTouched( QDateTime::fromString("2010-11-25T13:59:03Z", "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'") );
        page.setLastRevId(367741756);
        page.setCounter(0);
        page.setLength(70);
        page.setStarttimestamp(QDateTime::fromString("2010-11-25T16:14:51Z", "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'"));
        page.setEditToken("+\\");
        page.setTalkid(5477418);
        page.setFullurl(QUrl("http://en.wikipedia.org/wiki/API"));
        page.setEditurl(QUrl("http://en.wikipedia.org/w/index.php?title=API&action=edit"));
        page.setReadable("");
        page.setPreload("");

        QTest::newRow("No protection")
                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection /></page></pages></query></api>"
                << page
                << QVector<Protection>();

        QTest::newRow("One pages and one protection")
                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
                << page
                << (QVector<Protection>() << pr1);

        QTest::newRow("One pages and two protection")
                << "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"+\\\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection><pr type=\"edit\" level=\"sysop\" expiry=\"infinity\"/><pr type=\"move\" level=\"sysop\" expiry=\"infinity\"/></protection></page></pages></query></api>"
                << page
                << (QVector<Protection>() << pr1 << pr2);
    }

    void cleanupTestCase()
    {
        delete this->m_mediaWiki;
    }

private:

    int                  queryInfoCount;
    Page                 queryInfoResultsPage;
    QVector <Protection> queryInfoResultsProtections;
    MediaWiki*           m_mediaWiki;
};

QTEST_MAIN(QueryInfoTest)

#include "queryinfotest.moc"

#endif // TEST_QUERYINFO_H
