/*
 *   Copyright 2010 by Guillaume Hormiere <hormiere.guillaume@gmail.com>
 *   Copyright 2011 by Manuel Campomanes <campomanes.manuel@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef TEST_GENERAL_H
#define TEST_GENERAL_H

#include <QObject>
#include <QtTest/QtTest>
#include <KJob>

#include "libmediawikitest/fakeserver.h"

#include "mediawiki.h"
#include "querysiteinfogeneral.h"
#include "generalinfo.h"

using mediawiki::MediaWiki;
using mediawiki::QuerySiteInfoGeneral;
using mediawiki::Generalinfo;

class QuerySiteInfoGeneralTest : public QObject
{
    Q_OBJECT

public slots:

void resultHandle(const Generalinfo & generalinfo) {
    ++generalCount;
    generalResult = generalinfo;
}
private slots:

void initTestCase()
{
    generalCount = 0;
    this->request = "?format=xml&action=query&meta=siteinfo&siprop=general";
}
void QuerySiteInfoGeneralTestConnectTrue()
{

    MediaWiki mediaWiki(QUrl("http://127.0.0.1:12566"));
    FakeServer server;

    generalCount = 0;
    QString senario("<api><query><general mainpage=\"Main Page\" base=\"http://en.wikipedia.org/wiki/Main_Page\" sitename=\"Wikipedia\" generator=\"MediaWiki 1.16wmf4\" phpversion=\"5.2.4-2ubuntu5.12wm1\" phpsapi=\"apache2handler\" dbtype=\"mysql\" dbversion=\"5.1.46-facebook-r3489-log\" rev=\"75268\" case=\"first-letter\" rights=\"Creative Commons Attribution-Share Alike 3.0 Unported\" lang=\"en\" fallback8bitEncoding=\"windows-1252\" writeapi=\"\" timezone=\"UTC\" timeoffset=\"0\" articlepath=\"/wiki/$1\" scriptpath=\"/w\" script=\"/w/index.php\" variantarticlepath=\"\" server=\"http://en.wikipedia.org\" wikiid=\"enwiki\" time=\"2010-10-24T19:53:13Z\"/></query></api>");
    server.addScenario(senario);
    server.startAndWait();

    QuerySiteInfoGeneral * general = new QuerySiteInfoGeneral(mediaWiki);

    connect(general, SIGNAL(result(const Generalinfo &)), this, SLOT(resultHandle(const Generalinfo &)));
    general->exec();
    FakeServer::Request serverrequest = server.getRequest()[0];
    QCOMPARE(this->generalCount, 1);
    QCOMPARE(serverrequest.type, QString("GET"));
    QCOMPARE(serverrequest.value, this->request);
    QVERIFY(general->error() == QuerySiteInfoGeneral::NoError);

}
void QuerySiteInfoGeneralTestAttribute()
{

    MediaWiki mediaWiki(QUrl("http://127.0.0.1:12566"));
    FakeServer server;

    generalCount = 0;
    QString senario("<api><query><general mainpage=\"Main Page\" base=\"http://en.wikipedia.org/wiki/Main_Page\" sitename=\"Wikipedia\" generator=\"MediaWiki 1.16wmf4\" phpversion=\"5.2.4-2ubuntu5.12wm1\" phpsapi=\"apache2handler\" dbtype=\"mysql\" dbversion=\"5.1.46-facebook-r3489-log\" rev=\"75268\" case=\"first-letter\" rights=\"Creative Commons Attribution-Share Alike 3.0 Unported\" lang=\"en\" fallback8bitEncoding=\"windows-1252\" writeapi=\"\" timezone=\"UTC\" timeoffset=\"0\" articlepath=\"/wiki/$1\" scriptpath=\"/w\" script=\"/w/index.php\" variantarticlepath=\"\" server=\"http://en.wikipedia.org\" wikiid=\"enwiki\" time=\"2010-10-24T19:53:13Z\"/></query></api>");
    server.addScenario(senario);
    server.startAndWait();

    QuerySiteInfoGeneral * general = new QuerySiteInfoGeneral(mediaWiki);

    connect(general, SIGNAL(result(const Generalinfo &)), this, SLOT(resultHandle(const Generalinfo &)));
    general->exec();
    FakeServer::Request serverrequest = server.getRequest()[0];
    QCOMPARE(this->generalCount, 1);
    QCOMPARE(serverrequest.type, QString("GET"));
    QCOMPARE(serverrequest.value, this->request);
    QVERIFY(general->error() == QuerySiteInfoGeneral::NoError);
    Generalinfo resultExpected;
    resultExpected.setMainPage(QString("Main Page"));
    resultExpected.setUrl(QUrl("http://en.wikipedia.org/wiki/Main_Page"));
    resultExpected.setSiteName(QString("Wikipedia"));
    resultExpected.setGenerator(QString("MediaWiki 1.16wmf4"));
    resultExpected.setPhpVersion(QString("5.2.4-2ubuntu5.12wm1"));
    resultExpected.setPhpApi(QString("apache2handler"));
    resultExpected.setDataBaseType(QString("mysql"));
    resultExpected.setDataBaseVersion(QString("5.1.46-facebook-r3489-log"));
    resultExpected.setRev(QString("75268"));
    resultExpected.setCas(QString("first-letter"));
    resultExpected.setLicence(QString("Creative Commons Attribution-Share Alike 3.0 Unported"));
    resultExpected.setLanguage(QString("en"));
    resultExpected.setFallBack8bitEncoding(QString("windows-1252"));
    resultExpected.setWriteApi(QString(""));
    resultExpected.setTimeZone(QString("UTC"));
    resultExpected.setTimeOffset(QString("0"));
    resultExpected.setArticlePath(QString("/wiki/$1"));
    resultExpected.setScriptPath(QString("/w"));
    resultExpected.setScript(QString("/w/index.php"));
    resultExpected.setVariantArticlePath(QString(""));
    resultExpected.setServerUrl(QUrl("http://en.wikipedia.org"));
    resultExpected.setWikiId(QString("enwiki"));
    resultExpected.setTime(QDateTime(QDate(2010, 10, 24), QTime(19, 53, 13)));
    QCOMPARE(generalResult, resultExpected);
}
void QuerySiteInfoGeneralTestConnectFalseXML()
{

    MediaWiki mediaWiki(QUrl("http://127.0.0.1:12566"));
    FakeServer server;

    generalCount = 0;
    QString senario("<api><query<general mainpage=\"Main Page\" base=\"http://en.wikipedia.org/wiki/Main_Page\" sitename=\"Wikipedia\" generator=\"MediaWiki 1.16wmf4\" phpversion=\"5.2.4-2ubuntu5.12wm1\" phpsapi=\"apache2handler\" dbtype=\"mysql\" dbversion=\"5.1.46-facebook-r3489-log\" rev=\"75268\" case=\"first-letter\" rights=\"Creative Commons Attribution-Share Alike 3.0 Unported\" lang=\"en\" fallback8bitEncoding=\"windows-1252\" writeapi=\"\" timezone=\"UTC\" timeoffset=\"0\" articlepath=\"/wiki/$1\" scriptpath=\"/w\" script=\"/w/index.php\" variantarticlepath=\"\" server=\"http://en.wikipedia.org\" wikiid=\"enwiki\" time=\"2010-10-24T19:53:13Z\"/>");
    server.addScenario(senario);
    server.startAndWait();

    QuerySiteInfoGeneral * general = new QuerySiteInfoGeneral(mediaWiki);

    connect(general, SIGNAL(result(const Generalinfo &)), this, SLOT(resultHandle(const Generalinfo &)));
    general->exec();
    FakeServer::Request serverrequest = server.getRequest()[0];
    QCOMPARE(this->generalCount, 0);
    QCOMPARE(serverrequest.type, QString("GET"));
    QCOMPARE(serverrequest.value, this->request);
    QVERIFY(general->error() == QuerySiteInfoGeneral::XmlError);

}

void QuerySiteInfoGeneralTestErrortIncludeAllDenied()
{

    MediaWiki mediaWiki(QUrl("http://127.0.0.1:12566"));
    FakeServer server;

    generalCount = 0;
    QString senario("<api><error code=\"includeAllDenied\" info=\"\"/></api>");
    server.addScenario(senario);
    server.startAndWait();

    QuerySiteInfoGeneral * general = new QuerySiteInfoGeneral(mediaWiki);

    connect(general, SIGNAL(result(const Generalinfo &)), this, SLOT(resultHandle(const Generalinfo &)));
    general->exec();
    FakeServer::Request serverrequest = server.getRequest()[0];
    QCOMPARE(this->generalCount, 0);
    QCOMPARE(serverrequest.type, QString("GET"));
    QCOMPARE(serverrequest.value, this->request);
    QVERIFY(general->error() == QuerySiteInfoGeneral::IncludeAllDenied);

}

private:
    int generalCount;
    Generalinfo generalResult;
    QString request;
};
QTEST_MAIN(QuerySiteInfoGeneralTest);
#include "QuerySiteInfoGeneralTest.moc"

#endif // TEST_GENERAL_H

