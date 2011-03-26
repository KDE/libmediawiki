/*
 *   Copyright 2010 by Guillaume Hormiere <hormiere.guillaume@gmail.com>
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

#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QStringList>
#include <QList>
#include <QString>
#include <QTcpSocket>
#include <QThread>
#include <QMutex>

QT_BEGIN_NAMESPACE
class QTcpServer;
class QNetworkSession;
QT_END_NAMESPACE

//! [0]

class FakeServer : QThread
{
    Q_OBJECT

public:

    struct Request{
        Request(){}
        Request(QString t, QString a, QString v){type=t;agent=a;value=v;}
        QString type;
        QString agent;
        QString value;
    };

    FakeServer(QObject* parent = 0);
    ~FakeServer();

    void startAndWait();
    virtual void run();
    void setScenario( const QString &scenario, const QString &cookie = QString("empty"));
    void addScenario( const QString &scenario, const QString &cookie = QString("empty"));
    void addScenarioFromFile( const QString &fileName, const QString &cookie = QString("empty"));

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

    void writeServerPart();
    void readClientPart();

    QStringList m_scenarios;
    QStringList m_cookie;
    QList<FakeServer::Request> m_request;
    QTcpServer *m_tcpServer;
    mutable QMutex m_mutex;
    QTcpSocket* m_clientSocket;
};
//! [0]

#endif
