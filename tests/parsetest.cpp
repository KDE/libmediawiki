/*
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

#ifndef TEST_PARSE_H
#define TEST_PARSE_H

#include <QObject>
#include <QtTest/QtTest>
#include <KJob>
#include <QDebug>

#include "mediawiki.h"
#include "parse.h"
#include "libmediawikitest/fakeserver.h"

using mediawiki::MediaWiki;
using mediawiki::Parse;

Q_DECLARE_METATYPE(FakeServer::Request)
Q_DECLARE_METATYPE(QVariant)
Q_DECLARE_METATYPE(Parse*)

QString QStringFromFile( const QString &fileName )
{
  QFile file( fileName );
  file.open( QFile::ReadOnly );
  QTextStream in(&file);
  QString scenario;
  // When loading from files we never have the authentication phase
  // force jumping directly to authenticated state.

  while ( !in.atEnd() ) {
    scenario.append( in.readLine() );
  }
  file.close();
  return scenario;
}

class ParseTest : public QObject
{
    Q_OBJECT

public slots:

    void parseHandle(KJob* job) {
        parseCount++;
    }

    void resultHandle(QString result) {
        parseResult = result;
    }

private slots:

    void initTestCase()
    {
        parseCount = 0;
        this->m_mediaWiki = new MediaWiki(QUrl("http://127.0.0.1:12566"));
    }    

    void result()
    {
        QString scenario = QStringFromFile("./parsetest_result.rc");
        QString result = QStringFromFile("./parsetest_resulttrue.rc");

        Parse * job = new Parse(*m_mediaWiki, NULL);
        parseCount = 0;
        FakeServer fakeserver;
        fakeserver.setScenario(scenario);
        fakeserver.startAndWait();
        connect(job, SIGNAL(result(QString )),this, SLOT(resultHandle(QString)));
        connect(job, SIGNAL(result(KJob* )),this, SLOT(parseHandle(KJob*)));
        job->exec();
        QCOMPARE(parseResult, result);
        QCOMPARE(this->parseCount, 1);


    }

    void parseSetters()
    {
        QFETCH(QString, scenario);
        QFETCH(QString, request);
        QFETCH(Parse*, job);

        parseCount = 0;
        FakeServer fakeserver;
        fakeserver.setScenario(scenario);
        fakeserver.startAndWait();
        connect(job, SIGNAL(result(KJob* )),this, SLOT(parseHandle(KJob*)));
        job->exec();
        FakeServer::Request serverrequest = fakeserver.getRequest()[0];
        QCOMPARE(serverrequest.type, QString("GET"));
        QCOMPARE(serverrequest.value, request);        
        QCOMPARE(this->parseCount, 1);
    }

    void parseSetters_data()
    {
        QTest::addColumn<QString>("scenario");
        QTest::addColumn<QString>("request");        
        QTest::addColumn<Parse*>("job");

        Parse * p1 = new Parse( *m_mediaWiki, NULL);
        p1->setText("listedecharacteres");

        QTest::newRow("Text")
                << QStringFromFile("./parsetest.rc")
                << "?format=xml&action=parse&text=listedecharacteres"
                << p1;

        Parse * p2 = new Parse( *m_mediaWiki, NULL);
        p2->setPageName("listedecharacteres");

        QTest::newRow("Page Name")
                << QStringFromFile("./parsetest.rc")
                << "?format=xml&action=parse&page=listedecharacteres"
                << p2;

        Parse * p3 = new Parse( *m_mediaWiki, NULL);
        p3->setTitle("listedecharacteres");

        QTest::newRow("Title")
                << QStringFromFile("./parsetest.rc")
                << "?format=xml&action=parse&title=listedecharacteres"
                << p3;

        Parse * p4 = new Parse( *m_mediaWiki, NULL);
        p4->setUseLang("fr");

        QTest::newRow("User Langue")
                << QStringFromFile("./parsetest.rc")
                << "?format=xml&action=parse&uselang=fr"
                << p4;
    }

    void error()
    {
        QFETCH(QString, scenario);
        QFETCH(int, error);
        parseCount = 0;
        FakeServer fakeserver;
        if(scenario != QString("error serveur"))
        {
            fakeserver.addScenario(scenario);
            fakeserver.startAndWait();
        }

        Parse * job = new Parse(*m_mediaWiki);
        job->setTitle( "title" );

        connect(job, SIGNAL(result(KJob*)), this, SLOT(parseHandle(KJob*)));

        job->exec();

        if(scenario != QString("error serveur"))
        {
            QList<FakeServer::Request> requests = fakeserver.getRequest();
            QCOMPARE(requests.size(), 1);
        }
        QCOMPARE(job->error(), error);
        QCOMPARE(parseCount, 1);

        if(scenario != QString("error serveur"))
        {
            QVERIFY(fakeserver.isAllScenarioDone());
        }
    }

    void error_data()
    {
        QTest::addColumn<QString>("scenario");
        QTest::addColumn<int>("error");

        QTest::newRow("XML")
                << "<?xml version=\"1.0\" encoding=\"utf-8\"?><api><parse><text>"
                << int(Parse::XmlError);

        QTest::newRow("Network")
                << "error serveur"
                << int(Parse::NetworkError);

        QTest::newRow("Params")
                << "<api><error code=\"params\" info=\"\"/></api>"
                << int(Parse::TooManyParams);

        QTest::newRow("Missing Title")
                << "<api><error code=\"missingtitle\" info=\"\"/></api>"
                << int(Parse::MissingPage);
    }

    void cleanupTestCase()
    {
        delete this->m_mediaWiki;        
    }

private:

    int parseCount;
    QString request;
    QString parseResult;
    MediaWiki* m_mediaWiki;
};


QTEST_MAIN(ParseTest);
#include "parsetest.moc"
#endif // TEST_PARSE_H

