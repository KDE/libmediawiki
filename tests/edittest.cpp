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
 * @author Copyright (C) 2011 by Manuel Campomanes
 *         <a href="mailto:campomanes dot manuel at gmail dot com">campomanes dot manuel at gmail dot com</a>
 * @author Copyright (C) 2011 by Hormiere Guillaume
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

#ifndef TEST_EDIT_H
#define TEST_EDIT_H

#include <QObject>
#include <QtTest/QtTest>

#include <kjob.h>

#include "mediawiki.h"
#include "edit.h"
#include "libmediawikitest/fakeserver.h"

using mediawiki::MediaWiki;
using mediawiki::Edit;

Q_DECLARE_METATYPE(FakeServer::Request)
Q_DECLARE_METATYPE(QVariant)
Q_DECLARE_METATYPE(Edit*)

class EditTest : public QObject
{
    Q_OBJECT

signals:

    void captchaSignal(const QString & CaptchaAnswer);

public Q_SLOTS:

    void editHandle(KJob* )
    {
        editCount++;
    }

    void editHandle(const QVariant& captcha)
    {
        editCount++;
        this->CaptchaQuestion = captcha;
        emit captchaSignal(this->CaptchaAnswer);
    }

private Q_SLOTS:

    void initTestCase()
    {
        editCount            = 0;
        this->m_mediaWiki    = new MediaWiki(QUrl("http://127.0.0.1:12566"));
        this->m_infoScenario = "<api><query><pages><page pageid=\"27697087\" ns=\"0\" title=\"API\" touched=\"2010-11-25T13:59:03Z\" lastrevid=\"367741756\" counter=\"0\" length=\"70\" redirect=\"\" starttimestamp=\"2010-11-25T16:14:51Z\" edittoken=\"cecded1f35005d22904a35cc7b736e18%2B\" talkid=\"5477418\" fullurl=\"http://en.wikipedia.org/wiki/API\" editurl=\"http://en.wikipedia.org/w/index.php?title=API&action=edit\" ><protection /></page></pages></query></api>";
    }

    void editSetters()
    {
        QFETCH(QString, request);
        QFETCH(QString, senario);
        QFETCH(Edit*, job);

        editCount = 0;
        FakeServer fakeserver;
        fakeserver.setScenario(m_infoScenario);
        fakeserver.addScenario(senario);
        fakeserver.startAndWait();

        connect(job, SIGNAL(result(KJob*)),
                this, SLOT(editHandle(KJob*)));

        job->exec();
        FakeServer::Request serverrequest = fakeserver.getRequest()[1];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, request);
        QCOMPARE(job->error(), (int)Edit::NoError);
        QCOMPARE(this->editCount, 1);
    }
    void editSetters_data()
    {
        QTest::addColumn<QString>("request");
        QTest::addColumn<QString>("senario");
        QTest::addColumn<Edit*>("job");

        Edit* const e1 = new Edit( *m_mediaWiki, NULL);
        e1->setSection("new");
        e1->setSummary("Hello World" );
        e1->setPageName( "Talk:Main Page" );
        e1->setToken( "cecded1f35005d22904a35cc7b736e18+\\" );
        e1->setBaseTimestamp( QDateTime::fromString("2008-03-20T17:26:39Z","yyyy-MM-ddThh:mm:ssZ") );
        e1->setStartTimestamp( QDateTime::fromString("2008-03-27T21:15:39Z","yyyy-MM-ddThh:mm:ssZ") );
        e1->setText( "Hello everyone!" );
        QTest::newRow("Text")
                << "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B"
                << "<api><edit result=\"Success\" pageid=\"12\" title=\"Talk:Main Page\" oldrevid=\"465\" newrevid=\"471\" /></api>"
                << e1;

        Edit* const e2 = new Edit( *m_mediaWiki, NULL);
        e2->setSection("new");
        e2->setSummary("Hello World");
        e2->setPageName( "Talk:Main Page" );
        e2->setToken( "cecded1f35005d22904a35cc7b736e18+\\" );
        e2->setBaseTimestamp( QDateTime::fromString("2008-03-20T17:26:39Z","yyyy-MM-ddThh:mm:ssZ") );
        e2->setStartTimestamp( QDateTime::fromString("2008-03-27T21:15:39Z","yyyy-MM-ddThh:mm:ssZ") );
        e2->setAppendText( "Hello everyone!" );
        QTest::newRow("Append")
                << "?format=xml&action=edit&appendtext=Hello%20everyone!&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B"
                << "<api><edit result=\"Success\" pageid=\"12\" title=\"Talk:Main Page\" oldrevid=\"465\" newrevid=\"471\" /></api>"
                << e2;

        Edit* const e3 = new Edit( *m_mediaWiki, NULL);
        e3->setSection("new");
        e3->setSummary("Hello World");
        e3->setPageName( "Talk:Main Page" );
        e3->setToken( "cecded1f35005d22904a35cc7b736e18+\\" );
        e3->setBaseTimestamp( QDateTime::fromString("2008-03-20T17:26:39Z","yyyy-MM-ddThh:mm:ssZ") );
        e3->setStartTimestamp( QDateTime::fromString("2008-03-27T21:15:39Z","yyyy-MM-ddThh:mm:ssZ") );
        e3->setPrependText( "Hello everyone!" );
        QTest::newRow("Prepend")
                <<  "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&prependtext=Hello%20everyone!&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B"
                << "<api><edit result=\"Success\" pageid=\"12\" title=\"Talk:Main Page\" oldrevid=\"465\" newrevid=\"471\" /></api>"
                << e3;

        Edit* const e4 = new Edit( *m_mediaWiki, NULL);
        e4->setSummary("Hello World");
        e4->setPageName( "Talk:Main Page" );
        e4->setToken( "cecded1f35005d22904a35cc7b736e18+\\" );
        e4->setBaseTimestamp( QDateTime::fromString("2008-03-20T17:26:39Z","yyyy-MM-ddThh:mm:ssZ") );
        e4->setStartTimestamp( QDateTime::fromString("2008-03-27T21:15:39Z","yyyy-MM-ddThh:mm:ssZ") );
        e4->setUndo(13585);
        QTest::newRow("Undo")
                <<  "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&title=Talk:Main%20Page&undo=13585&token=cecded1f35005d22904a35cc7b736e18%252B"
                << "<api><edit result=\"Success\" pageid=\"12\" title=\"Talk:Main Page\" oldrevid=\"465\" newrevid=\"471\" /></api>"
                << e4;

        Edit* const e5 = new Edit( *m_mediaWiki, NULL);
        e5->setSummary("Hello World");
        e5->setPageName( "Talk:Main Page" );
        e5->setToken( "cecded1f35005d22904a35cc7b736e18+\\" );
        e5->setBaseTimestamp( QDateTime::fromString("2008-03-20T17:26:39Z","yyyy-MM-ddThh:mm:ssZ") );
        e5->setStartTimestamp( QDateTime::fromString("2008-03-27T21:15:39Z","yyyy-MM-ddThh:mm:ssZ") );
        e5->setUndoAfter(13585);
        QTest::newRow("Undo After")
                <<  "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&title=Talk:Main%20Page&undoafter=13585&token=cecded1f35005d22904a35cc7b736e18%252B"
                << "<api><edit result=\"Success\" pageid=\"12\" title=\"Talk:Main Page\" oldrevid=\"465\" newrevid=\"471\" /></api>"
                << e5;

        Edit* const e6 = new Edit( *m_mediaWiki, NULL);
        e6->setSummary("Hello World");
        e6->setPageName( "Talk:Main Page" );
        e6->setToken( "cecded1f35005d22904a35cc7b736e18+\\" );
        e6->setBaseTimestamp( QDateTime::fromString("2008-03-20T17:26:39Z","yyyy-MM-ddThh:mm:ssZ") );
        e6->setText( "Hello everyone!" );
        e6->setRecreate(true);
        e6->setSection("new");
        e6->setStartTimestamp( QDateTime::fromString("2008-03-27T21:15:39Z","yyyy-MM-ddThh:mm:ssZ") );
        QTest::newRow("Recreate")
                <<  "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&recreate=on&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B"
                << "<api><edit result=\"Success\" pageid=\"12\" title=\"Talk:Main Page\" oldrevid=\"465\" newrevid=\"471\" /></api>"
                << e6;

        Edit* const e7 = new Edit( *m_mediaWiki, NULL);
        e7->setSummary("Hello World");
        e7->setPageName( "Talk:Main Page" );
        e7->setToken( "cecded1f35005d22904a35cc7b736e18+\\" );
        e7->setBaseTimestamp( QDateTime::fromString("2008-03-20T17:26:39Z","yyyy-MM-ddThh:mm:ssZ") );
        e7->setText( "Hello everyone!" );
        e7->setCreateonly(true);
        e7->setSection("new");
        e7->setStartTimestamp( QDateTime::fromString("2008-03-27T21:15:39Z","yyyy-MM-ddThh:mm:ssZ") );
        QTest::newRow("CreateOnly")
                <<  "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&createonly=on&md5=4d184ec6e8fe61abccb8ff62c4583cd0&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B"
                << "<api><edit result=\"Success\" pageid=\"12\" title=\"Talk:Main Page\" oldrevid=\"465\" newrevid=\"471\" /></api>"
                << e7;

        Edit* const e8 = new Edit( *m_mediaWiki, NULL);
        e8->setSummary("Hello World");
        e8->setPageName( "Talk:Main Page" );
        e8->setToken( "cecded1f35005d22904a35cc7b736e18+\\" );
        e8->setBaseTimestamp( QDateTime::fromString("2008-03-20T17:26:39Z","yyyy-MM-ddThh:mm:ssZ") );
        e8->setText( "Hello everyone!" );
        e8->setNocreate(true);
        e8->setSection("new");
        e8->setStartTimestamp( QDateTime::fromString("2008-03-27T21:15:39Z","yyyy-MM-ddThh:mm:ssZ") );
        QTest::newRow("No Create")
                <<  "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&nocreate=on&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B"
                << "<api><edit result=\"Success\" pageid=\"12\" title=\"Talk:Main Page\" oldrevid=\"465\" newrevid=\"471\" /></api>"
                << e8;

        Edit* const e9 = new Edit( *m_mediaWiki, NULL);
        e9->setSummary("Hello World");
        e9->setPageName( "Talk:Main Page" );
        e9->setToken( "cecded1f35005d22904a35cc7b736e18+\\" );
        e9->setBaseTimestamp( QDateTime::fromString("2008-03-20T17:26:39Z","yyyy-MM-ddThh:mm:ssZ") );
        e9->setText( "Hello everyone!" );
        e9->setMinor(true);
        e9->setSection("new");
        e9->setStartTimestamp( QDateTime::fromString("2008-03-27T21:15:39Z","yyyy-MM-ddThh:mm:ssZ") );
        QTest::newRow("Minor")
                <<  "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&minor=on&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B"
                << "<api><edit result=\"Success\" pageid=\"12\" title=\"Talk:Main Page\" oldrevid=\"465\" newrevid=\"471\" /></api>"
                << e9;

        Edit* const e10 = new Edit( *m_mediaWiki, NULL);
        e10->setSummary("Hello World");
        e10->setPageName( "Talk:Main Page" );
        e10->setToken( "cecded1f35005d22904a35cc7b736e18+\\" );
        e10->setBaseTimestamp( QDateTime::fromString("2008-03-20T17:26:39Z","yyyy-MM-ddThh:mm:ssZ") );
        e10->setText( "Hello everyone!" );
        e10->setWatchList(Edit::watch);
        QTest::newRow("WatchList watch")
                <<  "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&watchlist=watch&token=cecded1f35005d22904a35cc7b736e18%252B"
                << "<api><edit result=\"Success\" pageid=\"12\" title=\"Talk:Main Page\" oldrevid=\"465\" newrevid=\"471\" /></api>"
                << e10;

        Edit* const e11 = new Edit( *m_mediaWiki, NULL);
        e11->setSummary("Hello World");
        e11->setPageName( "Talk:Main Page" );
        e11->setToken( "cecded1f35005d22904a35cc7b736e18+\\" );
        e11->setBaseTimestamp( QDateTime::fromString("2008-03-20T17:26:39Z","yyyy-MM-ddThh:mm:ssZ") );
        e11->setText( "Hello everyone!" );
        e11->setWatchList(Edit::unwatch);
        QTest::newRow("WatchList unwatch")
                <<  "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&watchlist=unwatch&token=cecded1f35005d22904a35cc7b736e18%252B"
                << "<api><edit result=\"Success\" pageid=\"12\" title=\"Talk:Main Page\" oldrevid=\"465\" newrevid=\"471\" /></api>"
                << e11;

        Edit* const e12 = new Edit( *m_mediaWiki, NULL);
        e12->setSummary("Hello World");
        e12->setPageName( "Talk:Main Page" );
        e12->setToken( "cecded1f35005d22904a35cc7b736e18+\\" );
        e12->setBaseTimestamp( QDateTime::fromString("2008-03-20T17:26:39Z","yyyy-MM-ddThh:mm:ssZ") );
        e12->setText( "Hello everyone!" );
        e12->setWatchList(Edit::preferences);
        QTest::newRow("WatchList preferences")
                <<  "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&watchlist=preferences&token=cecded1f35005d22904a35cc7b736e18%252B"
                << "<api><edit result=\"Success\" pageid=\"12\" title=\"Talk:Main Page\" oldrevid=\"465\" newrevid=\"471\" /></api>"
                << e12;

        Edit* const e13 = new Edit( *m_mediaWiki, NULL);
        e13->setSummary("Hello World");
        e13->setPageName( "Talk:Main Page" );
        e13->setToken( "cecded1f35005d22904a35cc7b736e18+\\" );
        e13->setBaseTimestamp( QDateTime::fromString("2008-03-20T17:26:39Z","yyyy-MM-ddThh:mm:ssZ") );
        e13->setText( "Hello everyone!" );
        e13->setWatchList(Edit::nochange);
        QTest::newRow("WatchList nochange")
                <<  "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&watchlist=nochange&token=cecded1f35005d22904a35cc7b736e18%252B"
                << "<api><edit result=\"Success\" pageid=\"12\" title=\"Talk:Main Page\" oldrevid=\"465\" newrevid=\"471\" /></api>"
                << e13;
    }

    void error()
    {
        QFETCH(QString, scenario);
        QFETCH(int, error);


        editCount = 0;
        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        FakeServer fakeserver;

        if(scenario != QString("error serveur"))
        {
            fakeserver.setScenario(m_infoScenario);
            fakeserver.addScenario(scenario);
            fakeserver.startAndWait();
        }

        Edit* const job = new Edit(mediawiki, NULL);
        job->setSection("new");
        job->setSummary("Hello World" );
        job->setPageName( "Talk:Main Page" );
        job->setToken( "cecded1f35005d22904a35cc7b736e18+\\" );
        job->setBaseTimestamp( QDateTime::fromString("2008-03-20T17:26:39Z","yyyy-MM-ddThh:mm:ssZ") );
        job->setStartTimestamp( QDateTime::fromString("2008-03-27T21:15:39Z","yyyy-MM-ddThh:mm:ssZ") );
        job->setText( "Hello everyone!" );

        connect(job,  SIGNAL(result(KJob*)),
                this, SLOT(editHandle(KJob*)));

        job->exec();

        if(scenario != QString("error serveur"))
        {
            QList<FakeServer::Request> requests = fakeserver.getRequest();
            QCOMPARE(requests.size(), 2);
        }

        QCOMPARE(job->error(), error);
        QCOMPARE(editCount, 1);

        if(scenario != QString("error serveur"))
        {
            QVERIFY(fakeserver.isAllScenarioDone());
        }
    }
    void error_data()
    {
        QTest::addColumn<QString>("scenario");
        QTest::addColumn<int>("error");

        QTest::newRow("No text")
                << "<api><error code=\"notext\" info=\"\" /> </api>"
                << int(Edit::TextMissing);
        QTest::newRow("Invalide section")
                << "<api><error code=\"invalidsection\" info=\"\" /> </api>"
                << int(Edit::InvalidSection);
        QTest::newRow("Invalide section")
                << "<api><error code=\"invalidsection\" info=\"\" /> </api>"
                << int(Edit::InvalidSection);
        QTest::newRow("Protected Title")
                << "<api><error code=\"protectedtitle\" info=\"\" /> </api>"
                << int(Edit::TitleProtected);
        QTest::newRow("Cant Create")
                << "<api><error code=\"cantcreate\" info=\"\" /> </api>"
                << int(Edit::CreatePagePermissionMissing);
        QTest::newRow("Cant Create Anonymous")
                << "<api><error code=\"cantcreate-anon\" info=\"\" /> </api>"
                << int(Edit::AnonymousCreatePagePermissionMissing);
        QTest::newRow("Cant Create Anonymous")
                << "<api><error code=\"cantcreate-anon\" info=\"\" /> </api>"
                << int(Edit::AnonymousCreatePagePermissionMissing);
        QTest::newRow("Article Duplication")
                << "<api><error code=\"articleexists\" info=\"\" /> </api>"
                << int(Edit::ArticleDuplication);
        QTest::newRow("Anonymous Create Image Permission Missing")
                << "<api><error code=\"noimageredirect-anon\" info=\"\" /> </api>"
                << int(Edit::AnonymousCreateImagePermissionMissing);
        QTest::newRow("Create Image Permission Missing")
                << "<api><error code=\"noimageredirect\" info=\"\" /> </api>"
                << int(Edit::CreateImagePermissionMissing);
        QTest::newRow("Spam Detected")
                << "<api><error code=\"spamdetected\" info=\"\" /> </api>"
                << int(Edit::SpamDetected);
        QTest::newRow("Filtered")
                << "<api><error code=\"filtered\" info=\"\" /> </api>"
                << int(Edit::Filtered);
        QTest::newRow("Article Size Exceed")
                << "<api><error code=\"contenttoobig\" info=\"\" /> </api>"
                << int(Edit::ArticleSizeExceed);
        QTest::newRow("Anonymous no edit")
                << "<api><error code=\"noedit-anon\" info=\"\" /> </api>"
                << int(Edit::AnonymousEditPagePermissionMissing);
        QTest::newRow("No edit")
                << "<api><error code=\"noedit\" info=\"\" /> </api>"
                << int(Edit::EditPagePermissionMissing);
        QTest::newRow("Page delete")
                << "<api><error code=\"pagedeleted\" info=\"\" /> </api>"
                << int(Edit::PageDeleted);
        QTest::newRow("Empty page")
                << "<api><error code=\"emptypage\" info=\"\" /> </api>"
                << int(Edit::EmptyPage);
        QTest::newRow("Empty section")
                << "<api><error code=\"emptynewsection\" info=\"\" /> </api>"
                << int(Edit::EmptySection);
        QTest::newRow("Edit conflict")
                << "<api><error code=\"editconflict\" info=\"\" /> </api>"
                << int(Edit::EditConflict);
        QTest::newRow("Revision wrong page")
                << "<api><error code=\"revwrongpage\" info=\"\" /> </api>"
                << int(Edit::RevWrongPage);
        QTest::newRow("Undo Failed")
                << "<api><error code=\"undofailure\" info=\"\" /> </api>"
                << int(Edit::UndoFailed);
    }

    void editTestTextCaptchaTrue()
    {
        editCount = 0;
        FakeServer fakeserver;

        this->request = "?format=xml&action=edit&basetimestamp=2008-03-20T17:26:39Z&md5=4d184ec6e8fe61abccb8ff62c4583cd0&section=new&starttimestamp=2008-03-27T21:15:39Z&summary=Hello%20World&text=Hello%20everyone!&title=Talk:Main%20Page&token=cecded1f35005d22904a35cc7b736e18%252B";
        QString senario("<api><edit result=\"Failure\"><captcha type=\"math\" mime=\"text/tex\" id=\"509895952\" question=\"36 + 4 = \" /></edit></api>");

        fakeserver.setScenario(m_infoScenario);
        fakeserver.addScenario(senario);
        senario = "<api><edit result=\"Success\" pageid=\"12\" title=\"Talk:Main Page\" oldrevid=\"465\" newrevid=\"471\" /></api>";
        fakeserver.addScenario(senario);
        fakeserver.startAndWait();

        Edit edit( *m_mediaWiki, NULL);
        edit.setSection("new");
        edit.setSummary("Hello World" );
        edit.setPageName( "Talk:Main Page" );
        edit.setToken( "cecded1f35005d22904a35cc7b736e18+\\" );
        edit.setBaseTimestamp( QDateTime::fromString("2008-03-20T17:26:39Z","yyyy-MM-ddThh:mm:ssZ") );
        edit.setStartTimestamp( QDateTime::fromString("2008-03-27T21:15:39Z","yyyy-MM-ddThh:mm:ssZ") );
        edit.setText( "Hello everyone!" );

        connect(&edit, SIGNAL(resultCaptcha(QVariant)),
                this, SLOT(editHandle(QVariant)));

        connect(this, SIGNAL(captchaSignal(QString)),
                &edit, SLOT(finishedCaptcha(QString)));

        edit.exec();

        FakeServer::Request serverrequest = fakeserver.getRequest()[1];
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, this->request);
        QCOMPARE(edit.error(), (int)Edit::NoError);
        QCOMPARE(this->editCount, 1);
    }

    void cleanupTestCase()
    {
        delete this->m_mediaWiki;
    }

private:

    int        editCount;
    QVariant   CaptchaQuestion;
    QString    CaptchaAnswer;
    QString    request;
    QString    m_infoScenario;
    MediaWiki* m_mediaWiki;
};

QTEST_MAIN(EditTest)

#include "edittest.moc"

#endif // TEST_EDIT_H
