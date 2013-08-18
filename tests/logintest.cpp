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

#ifndef TEST_LOGIN_H
#define TEST_LOGIN_H

#include <QObject>
#include <QtTest/QtTest>

#include <kjob.h>

#include "mediawiki.h"
#include "login.h"
#include "libmediawikitest/fakeserver.h"

using mediawiki::MediaWiki;
using mediawiki::Login;

class LoginTest : public QObject
{
    Q_OBJECT

public slots:

    void loginHandle(KJob* job) {
        Q_UNUSED(job)
        loginCount++;
    }

private Q_SLOTS:

    void initTestCase()
    {
        loginCount = 0;
        this->m_server = new FakeServer;
    }

    void loginTestConnectTrue()
    {
        loginCount = 0;
        QString senario("<api><login result=\"NeedToken\" token=\"b5780b6e2f27e20b450921d9461010b4\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\" /> </api>" );
        QString cookie( "cookieprefix=\"enwiki\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\"");
        m_server->setScenario(senario, cookie);
        senario = "<api><login result=\"Success\" lguserid=\"12345\" lgusername=\"alexTest\" lgtoken=\"b5780b6e2f27e20b450921d9461010b4\" cookieprefix=\"enwiki\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\" /> </api>";
        m_server->addScenario(senario, cookie);
        m_server->startAndWait();

        MediaWiki site(QUrl("http://127.0.0.1:12566"));
        Login login(site, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob*)),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, QString("?format=xml&action=login&lgname=alexTest&lgpassword=test"));
        QCOMPARE(login.error(), (int)Login::NoError);
    }

    void loginTestAttribute()
    {
        loginCount = 0;
        QString senario("<api><login result=\"NeedToken\" token=\"b5780b6e2f27e20b450921d9461010b4\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\" /> </api>" );
        QString cookie( "cookieprefix=\"enwiki\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\"");
        m_server->setScenario(senario, cookie);
        senario = "<api><login result=\"Success\" lguserid=\"12345\" lgusername=\"alexTest\" lgtoken=\"b5780b6e2f27e20b450921d9461010b4\" cookieprefix=\"enwiki\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\" /></api>";
        m_server->addScenario(senario, cookie);
        m_server->startAndWait();

        MediaWiki site(QUrl("http://127.0.0.1:12566"));
        Login login(site, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob*)),this, SLOT(loginHandle(KJob*)));
        login.exec();

        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, QString("?format=xml&action=login&lgname=alexTest&lgpassword=test"));
        QCOMPARE(login.error(), (int)Login::NoError);        
    }

    void loginTestConnectTrueWithoutCookie()
    {
        loginCount = 0;
        QString senario("<api><login result=\"NeedToken\" token=\"b5780b6e2f27e20b450921d9461010b4\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\" /> </api>" );
        QString cookie( "cookieprefix=\"enwiki\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\"");
        m_server->setScenario(senario);
        senario = "<api><login result=\"Success\" lguserid=\"12345\" lgusername=\"alexTest\" lgtoken=\"b5780b6e2f27e20b450921d9461010b4\" cookieprefix=\"enwiki\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\" /></api>";
        m_server->addScenario(senario, cookie);
        m_server->startAndWait();

        MediaWiki site(QUrl("http://127.0.0.1:12566"));
        Login login(site, "alexTest", "test");
        connect(&login, SIGNAL(result(KJob*)),this, SLOT(loginHandle(KJob*)));
        login.exec();
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(login.error(), (int)Login::NoError);
    }

    void loginTestFalseXMLLogin()
    {
        loginCount = 0;
        QString senario("<api><login result=\"NeedToken\" token=\"b5780b6e2f27e20b450921d9461010b4\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\" </api>" );
        QString cookie( "cookieprefix=\"enwiki\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\"");
        m_server->setScenario(senario, cookie);
        senario = "<api><login result=\"Success\" lguserid=\"12345\" lgusername=\"alexTest\" lgtoken=\"b5780b6e2f27e20b450921d9461010b4\" cookieprefix=\"enwiki\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\" /></api>";
        m_server->addScenario(senario);
        m_server->startAndWait();

        MediaWiki site(QUrl("http://127.0.0.1:12566"));
        Login login(site, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob*)),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, QString("?format=xml&action=login&lgname=alexTest&lgpassword=test"));
        QCOMPARE(login.error(), (int)Login::XmlError);

    }

    void loginTestNoNameLogin()
    {
        loginCount = 0;
        QString senario("<api><login result=\"NoName\" info=\"\" /> </api>" );
        m_server->setScenario(senario);
        m_server->startAndWait();

        MediaWiki site(QUrl("http://127.0.0.1:12566"));
        Login login(site, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob*)),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, QString("?format=xml&action=login&lgname=alexTest&lgpassword=test"));

        QCOMPARE(login.error(), (int)Login::LoginMissing);
    }

    void loginTestIllegalLogin()
    {
        loginCount = 0;
        QString senario("<api><login result=\"Illegal\" info=\"\" /> </api>" );
        m_server->setScenario(senario);
        m_server->startAndWait();

        MediaWiki site(QUrl("http://127.0.0.1:12566"));
        Login login(site, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob*)),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, QString("?format=xml&action=login&lgname=alexTest&lgpassword=test"));

        QCOMPARE(login.error(), (int)Login::IllegalUsername);
    }

    void loginTestNotExistsLogin()
    {
        loginCount = 0;
        QString senario("<api><login result=\"NotExists\" info=\"\" /> </api>" );
        m_server->setScenario(senario);
        m_server->startAndWait();

        MediaWiki site(QUrl("http://127.0.0.1:12566"));
        Login login(site, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob*)),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, QString("?format=xml&action=login&lgname=alexTest&lgpassword=test"));

        QCOMPARE(login.error(), (int)Login::UsernameNotExists);
    }

    void loginTestEmptyPassLogin()
    {
        loginCount = 0;
        QString senario("<api><login result=\"EmptyPass\" info=\"\" /> </api>" );
        m_server->setScenario(senario);
        m_server->startAndWait();

        MediaWiki site(QUrl("http://127.0.0.1:12566"));
        Login login(site, "alexTest", "");

        connect(&login, SIGNAL(result(KJob*)),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, QString("?format=xml&action=login&lgname=alexTest&lgpassword="));

        QCOMPARE(login.error(), (int)Login::PasswordMissing);
    }

    void loginTestWrongPassLogin()
    {
        loginCount = 0;
        QString senario("<api><login result=\"WrongPass\" info=\"\" /> </api>" );
        m_server->setScenario(senario);
        m_server->startAndWait();

        MediaWiki site(QUrl("http://127.0.0.1:12566"));
        Login login(site, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob*)),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, QString("?format=xml&action=login&lgname=alexTest&lgpassword=test"));

        QCOMPARE(login.error(), (int)Login::WrongPassword);
    }

    void loginTestWrongPluginPassLogin()
    {
        loginCount = 0;
        QString senario("<api><login result=\"WrongPluginPass\" info=\"\" /> </api>" );
        m_server->setScenario(senario);
        m_server->startAndWait();

        MediaWiki site(QUrl("http://127.0.0.1:12566"));
        Login login(site, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob*)),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, QString("?format=xml&action=login&lgname=alexTest&lgpassword=test"));

        QCOMPARE(login.error(), (int)Login::WrongPluginPassword);
    }

    void loginTestCreateBlockedLogin()
    {
        loginCount = 0;
        QString senario("<api><login result=\"CreateBlocked\" info=\"\" /> </api>" );
        m_server->setScenario(senario);
        m_server->startAndWait();

        MediaWiki site(QUrl("http://127.0.0.1:12566"));
        Login login(site, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob*)),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, QString("?format=xml&action=login&lgname=alexTest&lgpassword=test"));

        QCOMPARE(login.error(), (int)Login::IPAddressBlocked);
    }

    void loginTestThrottledLogin()
    {
        loginCount = 0;
        QString senario("<api><login result=\"Throttled\" info=\"\" /> </api>" );
        m_server->setScenario(senario);
        m_server->startAndWait();

        MediaWiki site(QUrl("http://127.0.0.1:12566"));
        Login login(site, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob*)),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, QString("?format=xml&action=login&lgname=alexTest&lgpassword=test"));

        QCOMPARE(login.error(), (int)Login::TooManyConnections);
    }

    void loginTestBlockedLogin()
    {
        loginCount = 0;
        QString senario("<api><login result=\"Blocked\" info=\"\" /> </api>" );
        m_server->setScenario(senario);
        m_server->startAndWait();

        MediaWiki site(QUrl("http://127.0.0.1:12566"));
        Login login(site, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob*)),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, QString("?format=xml&action=login&lgname=alexTest&lgpassword=test"));

        QCOMPARE(login.error(), (int)Login::UserBlocked);
    }

    void loginTestFalseXMLToken()
    {
        loginCount = 0;
        QString senario("<api><login result=\"NeedToken\" token=\"b5780b6e2f27e20b450921d9461010b4\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\" /> </api>" );
        QString cookie( "cookieprefix=\"enwiki\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\"");
        m_server->setScenario(senario, cookie);
        senario = "<api><login result=\"Success\" lguserid=\"12345\" lgusername=\"alexTest\" lgtoken=\"b5780b6e2f27e20b450921d9461010b4\" cookieprefix=\"enwiki\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\"</api>";
        m_server->addScenario(senario);
        m_server->startAndWait();

        MediaWiki site(QUrl("http://127.0.0.1:12566"));
        Login login(site, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob*)),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, QString("?format=xml&action=login&lgname=alexTest&lgpassword=test"));
        QCOMPARE(login.error(), (int)Login::XmlError);

    }

    void loginTestNoNameToken()
    {
        loginCount = 0;
        QString senario("<api><login result=\"NeedToken\" token=\"b5780b6e2f27e20b450921d9461010b4\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\" /> </api>" );
        QString cookie( "cookieprefix=\"enwiki\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\"");
        m_server->setScenario(senario, cookie);
        senario = "<api><login result=\"NoName\" info=\"\" /> </api>" ;
        m_server->addScenario(senario);
        m_server->startAndWait();

        MediaWiki site(QUrl("http://127.0.0.1:12566"));
        Login login(site, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob*)),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, QString("?format=xml&action=login&lgname=alexTest&lgpassword=test"));

        QCOMPARE(login.error(), (int)Login::LoginMissing);
    }

    void loginTestIllegalToken()
    {
        loginCount = 0;
        QString senario("<api><login result=\"NeedToken\" token=\"b5780b6e2f27e20b450921d9461010b4\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\" /> </api>" );
        QString cookie( "cookieprefix=\"enwiki\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\"");
        m_server->setScenario(senario, cookie);
        senario = "<api><login result=\"Illegal\" info=\"\" /> </api>" ;
        m_server->addScenario(senario);
        m_server->startAndWait();

        MediaWiki site(QUrl("http://127.0.0.1:12566"));
        Login login(site, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob*)),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, QString("?format=xml&action=login&lgname=alexTest&lgpassword=test"));

        QCOMPARE(login.error(), (int)Login::IllegalUsername);
    }

    void loginTestNotExistsToken()
    {
        loginCount = 0;
        QString senario("<api><login result=\"NeedToken\" token=\"b5780b6e2f27e20b450921d9461010b4\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\" /> </api>" );
        QString cookie( "cookieprefix=\"enwiki\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\"");
        m_server->setScenario(senario, cookie);
        senario = "<api><login result=\"NotExists\" info=\"\" /> </api>" ;
        m_server->addScenario(senario);
        m_server->startAndWait();

        MediaWiki site(QUrl("http://127.0.0.1:12566"));
        Login login(site, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob*)),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, QString("?format=xml&action=login&lgname=alexTest&lgpassword=test"));

        QCOMPARE(login.error(), (int)Login::UsernameNotExists);
    }

    void loginTestEmptyPassToken()
    {
        loginCount = 0;
        QString senario("<api><login result=\"NeedToken\" token=\"b5780b6e2f27e20b450921d9461010b4\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\" /> </api>" );
        QString cookie( "cookieprefix=\"enwiki\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\"");
        m_server->setScenario(senario, cookie);
        senario = "<api><login result=\"EmptyPass\" info=\"\" /> </api>" ;
        m_server->addScenario(senario);
        m_server->startAndWait();

        MediaWiki site(QUrl("http://127.0.0.1:12566"));
        Login login(site, "alexTest", "");

        connect(&login, SIGNAL(result(KJob*)),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, QString("?format=xml&action=login&lgname=alexTest&lgpassword="));

        QCOMPARE(login.error(), (int)Login::PasswordMissing);
    }

    void loginTestWrongPassToken()
    {
        loginCount = 0;
        QString senario("<api><login result=\"NeedToken\" token=\"b5780b6e2f27e20b450921d9461010b4\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\" /> </api>" );
        QString cookie( "cookieprefix=\"enwiki\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\"");
        m_server->setScenario(senario, cookie);
        senario = "<api><login result=\"WrongPass\" info=\"\" /> </api>" ;
        m_server->addScenario(senario);
        m_server->startAndWait();

        MediaWiki site(QUrl("http://127.0.0.1:12566"));
        Login login(site, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob*)),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, QString("?format=xml&action=login&lgname=alexTest&lgpassword=test"));

        QCOMPARE(login.error(), (int)Login::WrongPassword);
    }

    void loginTestWrongPluginPassToken()
    {
        loginCount = 0;
        QString senario("<api><login result=\"NeedToken\" token=\"b5780b6e2f27e20b450921d9461010b4\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\" /> </api>" );
        QString cookie( "cookieprefix=\"enwiki\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\"");
        m_server->setScenario(senario, cookie);
        senario = "<api><login result=\"WrongPluginPass\" info=\"\" /> </api>" ;
        m_server->addScenario(senario);
        m_server->startAndWait();

        MediaWiki site(QUrl("http://127.0.0.1:12566"));
        Login login(site, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob*)),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, QString("?format=xml&action=login&lgname=alexTest&lgpassword=test"));

        QCOMPARE(login.error(), (int)Login::WrongPluginPassword);
    }

    void loginTestCreateBlockedToken()
    {
        loginCount = 0;
        QString senario("<api><login result=\"NeedToken\" token=\"b5780b6e2f27e20b450921d9461010b4\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\" /> </api>" );
        QString cookie( "cookieprefix=\"enwiki\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\"");
        m_server->setScenario(senario, cookie);
        senario = "<api><login result=\"CreateBlocked\" info=\"\" /> </api>" ;
        m_server->addScenario(senario);
        m_server->startAndWait();

        MediaWiki site(QUrl("http://127.0.0.1:12566"));
        Login login(site, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob*)),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, QString("?format=xml&action=login&lgname=alexTest&lgpassword=test"));

        QCOMPARE(login.error(), (int)Login::IPAddressBlocked);
    }

    void loginTestThrottledToken()
    {
        loginCount = 0;
        QString senario("<api><login result=\"NeedToken\" token=\"b5780b6e2f27e20b450921d9461010b4\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\" /> </api>" );
        QString cookie( "cookieprefix=\"enwiki\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\"");
        m_server->setScenario(senario, cookie);
        senario = "<api><login result=\"Throttled\" info=\"\" /> </api>" ;
        m_server->addScenario(senario);
        m_server->startAndWait();

        MediaWiki site(QUrl("http://127.0.0.1:12566"));
        Login login(site, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob*)),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, QString("?format=xml&action=login&lgname=alexTest&lgpassword=test"));

        QCOMPARE(login.error(), (int)Login::TooManyConnections);
    }

    void loginTestBlockedToken()
    {
        loginCount = 0;
        QString senario("<api><login result=\"NeedToken\" token=\"b5780b6e2f27e20b450921d9461010b4\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\" /> </api>" );
        QString cookie( "cookieprefix=\"enwiki\" sessionid=\"17ab96bd8ffbe8ca58a78657a918558e\"");
        m_server->setScenario(senario, cookie);
        senario = "<api><login result=\"Blocked\" info=\"\" /> </api>" ;
        m_server->addScenario(senario);
        m_server->startAndWait();

        MediaWiki site(QUrl("http://127.0.0.1:12566"));
        Login login(site, "alexTest", "test");

        connect(&login, SIGNAL(result(KJob*)),this, SLOT(loginHandle(KJob*)));
        login.exec();
        FakeServer::Request serverrequest = m_server->getRequest()[0];
        QCOMPARE(this->loginCount, 1);
        QCOMPARE(serverrequest.type, QString("POST"));
        QCOMPARE(serverrequest.value, QString("?format=xml&action=login&lgname=alexTest&lgpassword=test"));

        QCOMPARE(login.error(), (int)Login::UserBlocked);
    }

    void cleanupTestCase()
    {
        delete this->m_server;
    }

private:

    int         loginCount;
    FakeServer* m_server;
};

QTEST_MAIN(LoginTest)

#include "logintest.moc"

#endif // TEST_LOGIN_H
