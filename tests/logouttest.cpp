/*
 *   Copyright 2010 by Alexandre Mendes <alex.mendes1988@gmail.com>
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

#ifndef TEST_LOGOUT_H
#define TEST_LOGOUT_H

#include <QObject>
#include <QtTest/QtTest>
#include <KJob>

#include "mediawiki.h"
#include "logout.h"
#include "libmediawikitest/fakeserver.h"

using mediawiki::MediaWiki;
using mediawiki::Logout;

class LogoutTest : public QObject
{
    Q_OBJECT

public slots:

    void logoutHandle(KJob* job) {
        logoutCount++;
    }

private slots:

    void initTestCase()
    {
        logoutCount = 0;
        this->m_mediaWiki = new MediaWiki(QUrl("http://127.0.0.1:12566"));
        this->m_server = new FakeServer;
        this->request = "?format=xml&action=logout";
    }

    void logoutTestConnectTrue()
    {
        QString senario("<api />" );
        QString cookie( "cookieprefix=\"enwiki\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\" expires=\"Sat, 12-Feb-2011 21:39:30 GMT\"");
        m_server->setScenario(senario, cookie);
        m_server->startAndWait();

        logoutCount = 0;
        Logout logout(*m_mediaWiki);
        connect(&logout, SIGNAL(result(KJob* )),this, SLOT(logoutHandle(KJob*)));
        logout.exec();
        QCOMPARE(this->logoutCount, 1);
        QCOMPARE(logout.error(), (int)Logout::NoError);

        QList<FakeServer::Request> requests = m_server->getRequest();
        QCOMPARE(requests.size(), 1);

        FakeServer::Request request = requests[0];
        QCOMPARE(request.agent, m_mediaWiki->userAgent());
        QCOMPARE(request.type, QString("GET"));
        QCOMPARE(request.value, QString("?format=xml&action=logout"));
    }

    void cleanupTestCase()
    {
        delete this->m_mediaWiki;
        delete this->m_server;
    }

private:

    int logoutCount;
    QString request;
    MediaWiki* m_mediaWiki;
    FakeServer* m_server;
};


QTEST_MAIN(LogoutTest);
#include "logouttest.moc"
#endif // TEST_LOGOUT_H

