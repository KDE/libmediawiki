/** ===========================================================
 * @file
 *
 * This file is a part of KDE project
 * <a href="https://commits.kde.org/libmediawiki">libmediawiki</a>
 *
 * @date   2010-03-22
 * @brief  a MediaWiki C++ interface for KDE
 *
 * @author Copyright (C) 2010-2011 by Hormiere Guillaume
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

#ifndef SERVER_H
#define SERVER_H

#include <QList>
#include <QMutex>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>

class FakeServer : QThread
{
    Q_OBJECT

public:

    struct Request
    {
        Request(){}
        Request(const QString& t, const QString& a, const QString& v){type=t; agent=a; value=v;}

        QString type;
        QString agent;
        QString value;

        bool operator==(const FakeServer::Request &other) const {
            return this->type  == other.type  &&
                   this->agent == other.agent &&
                   this->value == other.value;
        }
    };

    FakeServer(QObject* const parent = nullptr);
    ~FakeServer() override;

    void startAndWait();
    void run() override;

    void setScenario( const QString& scenario, const QString& cookie = QStringLiteral("empty"));
    void addScenario( const QString& scenario, const QString& cookie = QStringLiteral("empty"));
    void addScenarioFromFile( const QString& fileName, const QString& cookie = QStringLiteral("empty"));

    bool isScenarioDone( int scenarioNumber ) const;
    bool isAllScenarioDone() const;
    QList<FakeServer::Request>& getRequest(){return m_request;} const
    FakeServer::Request takeLastRequest(){return m_request.takeLast();}
    FakeServer::Request takeFirstRequest(){return m_request.takeFirst();}
    void clearRequest(){return m_request.clear();}

private Q_SLOTS:

    void newConnection();
    void dataAvailable();
    void started();

private:

    QStringList                m_scenarios;
    QStringList                m_cookie;
    QList<FakeServer::Request> m_request;
    QTcpServer*                m_tcpServer;
    mutable QMutex             m_mutex;
    QTcpSocket*                m_clientSocket;
};
//! [0]

#endif // SERVER_H
