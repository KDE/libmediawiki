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

#include <QtGui>
#include <QtNetwork>
#include <iostream>
#include <QDebug>

#include "fakeserver.moc"

FakeServer::FakeServer(QObject* parent)
:  QThread( parent )
{
    moveToThread(this);
}

FakeServer::~FakeServer()
{
  quit();
  wait();
}

void FakeServer::startAndWait()
{
  start();
  // this will block until the event queue starts
  QMetaObject::invokeMethod( this, "started", Qt::BlockingQueuedConnection );
}

void FakeServer::newConnection()
{
    QMutexLocker locker(&m_mutex);
    m_clientSocket = m_tcpServer->nextPendingConnection();
    connect(m_clientSocket, SIGNAL(readyRead()), this, SLOT(dataAvailable()));
}

void FakeServer::dataAvailable()
{
    QMutexLocker locker(&m_mutex);
    readClientPart();
    writeServerPart();
}

void FakeServer::run()
{
    m_tcpServer = new QTcpServer();
    if ( !m_tcpServer->listen( QHostAddress( QHostAddress::LocalHost ), 12566 ) ) {

    }

    connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));

    exec();

    delete m_clientSocket;

    delete m_tcpServer;
}

void FakeServer::started()
{
  // do nothing: this is a dummy slot used by startAndWait()
}

void FakeServer::setScenario( const QString &scenario, const QString &cookie)
{
    QMutexLocker locker(&m_mutex);

    m_scenarios.clear();
    m_scenarios << scenario;
    m_cookie.clear();
    m_cookie << cookie;
}

void FakeServer::addScenario( const QString &scenario, const QString &cookie )
{
    QMutexLocker locker(&m_mutex);

    m_scenarios << scenario;
    m_cookie << cookie;
}

void FakeServer::addScenarioFromFile( const QString &fileName, const QString &cookie )
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

  addScenario( scenario , cookie);
}

bool FakeServer::isScenarioDone( int scenarioNumber ) const
{
  QMutexLocker locker(&m_mutex);

  if ( scenarioNumber < m_scenarios.size() ) {
    return m_scenarios[scenarioNumber].isEmpty();
  } else {
    return true; // Non existent hence empty, right?
  }
}

bool FakeServer::isAllScenarioDone() const
{
  QMutexLocker locker( &m_mutex );

  foreach ( const QString &scenario, m_scenarios ) {
    if ( !scenario.isEmpty() ) {
      return false;
    }
  }
  return true;
}

void FakeServer::writeServerPart()
{

    QString retour = m_scenarios.isEmpty() ? QString("vide") : m_scenarios.takeFirst();
    QString cookie = m_cookie.isEmpty() ? QString("empty") : m_cookie.takeFirst();
    QString scenario = "HTTP/1.0 200 Ok\r\nContent-Type: text/html; charset=\"utf-8\"\r\nSet-Cookie:"+cookie+"\r\n\r\n" + retour;
    m_clientSocket->write( scenario.toLocal8Bit() );
    m_clientSocket->close();

}

void FakeServer::readClientPart()
{
    if (m_clientSocket->canReadLine())
    {
        QStringList token = QString(m_clientSocket->readAll()).split(QRegExp("[ \r\n][ \r\n]*"));
        FakeServer::Request request;
        if(token.empty())return;
        request.type = token[0];
        request.agent = token[4];
        request.value = token[1];
        m_request << request;
    }
}
