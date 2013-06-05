/** ===========================================================
 * @file
 *
 * This file is a part of KDE project
 * <a href="https://projects.kde.org/projects/extragear/libs/libmediawiki">libmediawiki</a>
 *
 * @date   2011-03-22
 * @brief  a MediaWiki C++ interface for KDE
 *
 * @author Copyright (C) 2011-2012 by Gilles Caulier
 *         <a href="mailto:caulier dot gilles at gmail dot com">caulier dot gilles at gmail dot com</a>
 * @author Copyright (C) 2011 by Manuel Campomanes
 *         <a href="mailto:campomanes dot manuel at gmail dot com">campomanes dot manuel at gmail dot com</a>
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

#include "login.moc"

// Qt includes

#include <QtCore/QStringList>
#include <QtCore/QTimer>
#include <QtCore/QUrl>
#include <QtCore/QXmlStreamReader>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

// Local includes

#include "mediawiki.h"
#include "job_p.h"

namespace mediawiki
{

class LoginPrivate : public JobPrivate
{

public:

    LoginPrivate(MediaWiki& mediawiki, const QString& login, const QString& password)
        : JobPrivate(mediawiki),
          login(login),
          password(password)
    {
    }

    static int error(const QString& error)
    {
        QStringList list;
        list << "NoName"
             << "Illegal"
             << "NotExists"
             << "EmptyPass"
             << "WrongPass"
             << "WrongPluginPass"
             << "CreateBlocked"
             << "Throttled"
             << "Blocked"
             << "NeedToken";

        int ret = list.indexOf(error);

        if(ret == -1)
        {
            ret = 0;
        }

        return (ret + (int)Login::LoginMissing);
    }

public:

    QUrl    baseUrl;
    QString login;
    QString password;
    QString lgsessionid;
    QString lgtoken;
};

Login::Login(MediaWiki& mediawiki, const QString& login, const QString& password, QObject* const parent)
    : Job(*new LoginPrivate(mediawiki, login, password), parent)
{
}

Login::~Login()
{
}

void Login::start()
{
    QTimer::singleShot(0, this, SLOT(doWorkSendRequest()));
}

void Login::doWorkSendRequest()
{
    Q_D(Login);

    // Set the url
    QUrl url = d->mediawiki.url();
    url.addQueryItem("format", "xml");
    url.addQueryItem("action", "login");
    url.addQueryItem("lgname", d->login);
    url.addQueryItem("lgpassword", d->password);
    d->baseUrl = url;

    // Set the request
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", d->mediawiki.userAgent().toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    // Send the request
    d->reply = d->manager->post(request, url.toString().toUtf8());
    connect(d->reply, SIGNAL(finished()), 
            this, SLOT(doWorkProcessReply()));
}

void Login::doWorkProcessReply()
{
    Q_D(Login);
    disconnect(d->reply, SIGNAL(finished()),
               this, SLOT(doWorkProcessReply()));

    if (d->reply->error() != QNetworkReply::NoError)
    {
        this->setError(Job::NetworkError);
        d->reply->close();
        d->reply->deleteLater();
        emitResult();
        return;
    }

    QXmlStreamReader reader(d->reply);

    while (!reader.atEnd() && !reader.hasError())
    {
        QXmlStreamReader::TokenType token = reader.readNext();

        if (token == QXmlStreamReader::StartElement)
        {
            QXmlStreamAttributes attrs = reader.attributes();

            if (reader.name() == QString("login"))
            {
                if (attrs.value(QString("result")).toString() == "Success")
                {
                    this->setError(Job::NoError);
                    d->lgtoken     = attrs.value(QString("lgtoken")).toString();
                    d->lgsessionid = attrs.value(QString("sessionid")).toString();

                    if(d->manager->cookieJar()->cookiesForUrl(d->mediawiki.url()).isEmpty())
                    {
                        QList<QNetworkCookie> cookies;
                        QString prefix = attrs.value(QString("cookieprefix")).toString();

                        QString prefixSession = prefix;
                        prefixSession.append("_session");
                        QNetworkCookie cookie1(prefixSession.toUtf8(),attrs.value(QString("sessionid")).toString().toUtf8());
                        cookies.append(cookie1);

                        QString prefixUserName = prefix;
                        prefixUserName.append("UserName");
                        QNetworkCookie cookie2(prefixUserName.toUtf8(),attrs.value(QString("lgusername")).toString().toUtf8());
                        cookies.append(cookie2);

                        QString prefixUserID = prefix;
                        prefixUserID.append("UserID");
                        QNetworkCookie cookie3(prefixUserID.toUtf8(),attrs.value(QString("lguserid")).toString().toUtf8());
                        cookies.append(cookie3);

                        QString prefixToken = prefix;
                        prefixToken.append("Token");
                        QNetworkCookie cookie4(prefixToken.toUtf8(),attrs.value(QString("lgtoken")).toString().toUtf8());
                        cookies.append(cookie4);

                        d->manager->cookieJar()->setCookiesFromUrl(cookies, d->mediawiki.url());
                    }

                    d->reply->close();
                    d->reply->deleteLater();
                    emitResult();
                    return;
                }
                else if (attrs.value(QString("result")).toString() == "NeedToken")
                {
                    this->setError(Job::NoError);
                    d->lgtoken     = attrs.value(QString("token")).toString();
                    d->lgsessionid = attrs.value(QString("sessionid")).toString();

                    if(d->manager->cookieJar()->cookiesForUrl(d->mediawiki.url()).isEmpty())
                    {
                        QString prefix = attrs.value(QString("cookieprefix")).toString();
                        prefix.append("_session");
                        QNetworkCookie cookie(prefix.toUtf8(),QString(d->lgsessionid).toUtf8());
                        QList<QNetworkCookie> cookies;
                        cookies.append(cookie);
                        d->manager->cookieJar()->setCookiesFromUrl(cookies, d->mediawiki.url());
                    }
                }
                else
                {
                    this->setError(LoginPrivate::error(attrs.value(QString("result")).toString()));
                    d->reply->close();
                    d->reply->deleteLater();
                    emitResult();
                    return;
                }
            }
            else if (reader.name() == QLatin1String("error"))
            {
                this->setError(LoginPrivate::error(attrs.value(QString("code")).toString()));
                d->reply->close();
                d->reply->deleteLater();
                emitResult();
                return;
            }
        }
        else if (token == QXmlStreamReader::Invalid && reader.error() != QXmlStreamReader::PrematureEndOfDocumentError)
        {
            this->setError(XmlError);
            d->reply->close();
            d->reply->deleteLater();
            emitResult();
            return;
        }
    }
    d->reply->close();
    d->reply->deleteLater();

    QUrl url     = d->baseUrl;
    url.addQueryItem("lgtoken", d->lgtoken);
    QString data = url.toString();

    // Set the request
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", d->mediawiki.userAgent().toUtf8());
    request.setRawHeader("Cookie", d->manager->cookieJar()->cookiesForUrl(d->mediawiki.url()).at(0).toRawForm());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    // Send the request
    d->reply = d->manager->post(request, data.toUtf8());
    connectReply();

    connect(d->reply, SIGNAL(finished()), 
            this, SLOT(doWorkProcessReply()));
}

} // namespace mediawiki
