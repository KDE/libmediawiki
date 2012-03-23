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

#ifndef TEST_UPLOAD_H
#define TEST_UPLOAD_H

#include <QObject>
#include <QtTest/QtTest>

#include <kjob.h>

#include "mediawiki.h"
#include "upload.h"
#include "libmediawikitest/fakeserver.h"

using mediawiki::MediaWiki;
using mediawiki::Upload;

Q_DECLARE_METATYPE(FakeServer::Request)
Q_DECLARE_METATYPE(Upload*)

class UploadTest : public QObject
{
    Q_OBJECT

public Q_SLOTS:

    void uploadHandle(KJob* ) {
        uploadCount++;
    }

private Q_SLOTS:

    void initTestCase()
    {
        uploadCount = 0;
        this->m_mediaWiki = new MediaWiki(QUrl("http://127.0.0.1:12566"));
        this->m_infoScenario = "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"cecded1f35005d22904a35cc7b736e18%2B\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection /></page></pages></query></api>";
        this->m_file = new QFile("/home/alex/Documents/MediaWiki-Silk/src/mediawiki/libmediawiki/tests/Test.jpeg");
        this->m_file->open(QIODevice::ReadOnly);
    }

    void uploadSetters()
    {
        QFETCH(QString, request);
        QFETCH(QString, senario);
        QFETCH(Upload*, job);

        uploadCount = 0;
        FakeServer fakeserver;
        fakeserver.setScenario(m_infoScenario);
        fakeserver.addScenario(senario);
        fakeserver.startAndWait();
        connect(job, SIGNAL(result(KJob*)),this, SLOT(uploadHandle(KJob*)));
        job->exec();
        FakeServer::Request serverrequest = fakeserver.getRequest()[1];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, request);
        QCOMPARE(job->error(), (int)Upload::NoError);
        QCOMPARE(this->uploadCount, 1);
    }
    void uploadSetters_data()
    {
        QTest::addColumn<QString>("request");
        QTest::addColumn<QString>("senario");
        QTest::addColumn<Upload*>("job");

        Upload * e1 = new Upload( *m_mediaWiki, NULL);
        e1->setFile(this->m_file);
        e1->setFilename("Test.jpeg");
        e1->setComment("Test");
        e1->setText("{{Information|Description=Ajout du logo de l'IUP ISI, Toulouse.|Source=http://www.iupisi.ups-tlse.fr/|Date=1992-01-01|Author=iup|Permission={{PD-EEA}}|other_versions=}}");
        QTest::newRow("Text")
                << "?action=upload&format=xml"
                << "<api><upload result=\"Success\" pageid=\"12\" title=\"Talk:Main Page\" oldrevid=\"465\" newrevid=\"471\" /></api>"
                << e1;
    }

    void error()
    {
        QFETCH(QString, scenario);
        QFETCH(int, error);

        uploadCount = 0;
        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        FakeServer fakeserver;
        if(scenario != QString("error server"))
        {
            fakeserver.setScenario(m_infoScenario);
            fakeserver.addScenario(scenario);
            fakeserver.startAndWait();
        }

        Upload * job = new Upload(mediawiki, NULL);
        job->setFile(this->m_file);
        job->setFilename("Test.jpeg");
        job->setComment("Test");
        job->setText("{{Information|Description=Ajout du logo de l'IUP ISI, Toulouse.|Source=http://www.iupisi.ups-tlse.fr/|Date=1992-01-01|Author=iup|Permission={{PD-EEA}}|other_versions=}}");
        connect(job,  SIGNAL(result(KJob*)),this, SLOT(uploadHandle(KJob*)));

        job->exec();

        if(scenario != QString("error server"))
        {
            QList<FakeServer::Request> requests = fakeserver.getRequest();
            QCOMPARE(requests.size(), 2);
        }
        QCOMPARE(job->error(), error);
        QCOMPARE(uploadCount, 1);

        if(scenario != QString("error server"))
        {
            QVERIFY(fakeserver.isAllScenarioDone());
        }
    }
    void error_data()
    {
        QTest::addColumn<QString>("scenario");
        QTest::addColumn<int>("error");

        QTest::newRow("UploadDisabled")
                << "<api><upload result=\"Failure\"><error code=\"upload-disabled\" info=\"\" /></upload> </api>"
                << int(Upload::UploadDisabled);

        QTest::newRow("InvalidSessionKey")
                << "<api><upload result=\"Failure\"><error code=\"invalid-session-key\" info=\"\" /></upload> </api>"
                << int(Upload::InvalidSessionKey);

        QTest::newRow("BadAccess")
                << "<api><upload result=\"Failure\"><error code=\"bad-access-groups\" info=\"\" /></upload> </api>"
                << int(Upload::BadAccess);

        QTest::newRow("ParamMissing")
                << "<api><upload result=\"Failure\"><error code=\"missing-param\" info=\"\" /></upload> </api>"
                << int(Upload::ParamMissing);

        QTest::newRow("MustBeLoggedIn")
                << "<api><upload result=\"Failure\"><error code=\"mustbeloggedin\" info=\"\" /></upload> </api>"
                << int(Upload::MustBeLoggedIn);

        QTest::newRow("FetchFileError")
                << "<api><upload result=\"Failure\"><error code=\"fetchfile-error\" info=\"\" /></upload> </api>"
                << int(Upload::FetchFileError);

        QTest::newRow("NoModule")
                << "<api><upload result=\"Failure\"><error code=\"no-module\" info=\"\" /></upload> </api>"
                << int(Upload::NoModule);

        QTest::newRow("EmptyFile")
                << "<api><upload result=\"Failure\"><error code=\"emptyfile\" info=\"\" /></upload> </api>"
                << int(Upload::EmptyFile);

        QTest::newRow("ExtensionMissing")
                << "<api><upload result=\"Failure\"><error code=\"filetype-missing\" info=\"\" /></upload> </api>"
                << int(Upload::ExtensionMissing);

        QTest::newRow("TooShortFilename")
                << "<api><upload result=\"Failure\"><error code=\"filenametooshort\" info=\"\" /></upload> </api>"
                << int(Upload::TooShortFilename);

        QTest::newRow("OverWriting")
                << "<api><upload result=\"Failure\"><error code=\"overwrite\" info=\"\" /></upload> </api>"
                << int(Upload::OverWriting);

        QTest::newRow("StashFailed")
                << "<api><upload result=\"Failure\"><error code=\"stashfailed\" info=\"\" /></upload> </api>"
                << int(Upload::StashFailed);

        QTest::newRow("InternalError")
                << "<api><upload result=\"Failure\"><error code=\"internal-error\" info=\"\" /></upload> </api>"
                << int(Upload::InternalError);
    }

    void cleanupTestCase()
    {
        delete this->m_mediaWiki;
        delete this->m_file;
    }

private:

    int        uploadCount;
    QString    request;
    QString    m_infoScenario;
    QIODevice* m_file;
    MediaWiki* m_mediaWiki;
};

QTEST_MAIN(UploadTest)

#include "uploadtest.moc"

#endif // TEST_UPLOAD_H
