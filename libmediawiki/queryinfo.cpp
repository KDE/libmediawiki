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

#include "queryinfo.moc"

// Qt includes

#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <QtCore/QUrl>
#include <QtCore/QXmlStreamReader>
#include <QtCore/QRegExp>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

// Local includes

#include "mediawiki.h"
#include "job_p.h"

namespace mediawiki
{

class QueryInfoPrivate : public JobPrivate
{

public:

    QueryInfoPrivate(MediaWiki& mediawiki)
        : JobPrivate(mediawiki)
    {}

    QVector<Protection>    protections;
    QMap<QString, QString> requestParameter;
    Page                   page;
};

QueryInfo::QueryInfo(MediaWiki& mediawiki, QObject* const parent)
    : Job(*new QueryInfoPrivate(mediawiki), parent)
{
}

QueryInfo::~QueryInfo()
{
}

void QueryInfo::setPageName(const QString& title)
{
    Q_D(QueryInfo);
    d->requestParameter["titles"] = title;
}

void QueryInfo::setToken(const QString& token)
{
    Q_D(QueryInfo);
    d->requestParameter["intoken"] = token;
}

void QueryInfo::setPageId(unsigned int id)
{
    Q_D(QueryInfo);
    d->requestParameter["pageids"] = QString::number(id);
}

void QueryInfo::setRevisionId(unsigned int id)
{
    Q_D(QueryInfo);
    d->requestParameter["revids"] = QString::number(id);
}

void QueryInfo::start()
{
    QTimer::singleShot(0, this, SLOT(doWorkSendRequest()));
}

void QueryInfo::doWorkSendRequest()
{
    Q_D(QueryInfo);

    // Set the url
    QUrl url = d->mediawiki.url();
    url.addQueryItem("format", "xml");
    url.addQueryItem("action", "query");
    url.addQueryItem("prop",   "info");
    url.addEncodedQueryItem("inprop", QString("protection|talkid|watched|subjectid|url|readable|preload").toUtf8());

    QMapIterator<QString, QString> i(d->requestParameter);
    while (i.hasNext())
    {
        i.next();
        url.addEncodedQueryItem(QByteArray(i.key().toAscii()),              // TODO : check UTF-8 support
                                QByteArray(i.value().toAscii()));           // TODO : check UTF-8 support
    }

    // Set the request
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", d->mediawiki.userAgent().toUtf8());
    QByteArray cookie = "";
    QList<QNetworkCookie> mediawikiCookies = d->manager->cookieJar()->cookiesForUrl(d->mediawiki.url());

    for(int i = 0 ; i < mediawikiCookies.size(); ++i)
    {
        cookie += mediawikiCookies.at(i).toRawForm(QNetworkCookie::NameAndValueOnly);
        cookie += ';';
    }
    request.setRawHeader( "Cookie", cookie );

    // Send the request
    d->reply = d->manager->get(request);
    connectReply();

    connect(d->reply, SIGNAL(finished()),
            this, SLOT(doWorkProcessReply()));
}

void QueryInfo::doWorkProcessReply()
{
    Q_D(QueryInfo);

    disconnect(d->reply, SIGNAL(finished()), 
               this, SLOT(doWorkProcessReply()));

    if (d->reply->error() == QNetworkReply::NoError)
    {
        // Replace & in &amp;
        QString content(d->reply->readAll());
        QRegExp regex("&(?!\\w+;)");
        content.replace(regex, "&amp;");
        QXmlStreamReader reader(content);
        QVector<Protection> protect;

        while (!reader.atEnd() && !reader.hasError())
        {
            QXmlStreamReader::TokenType token = reader.readNext();
            QXmlStreamAttributes attrs = reader.attributes();

            if (token == QXmlStreamReader::StartElement)
            {
                if (reader.name() == "page")
                {
                    d->page.setPageId(attrs.value( QString( "pageid" ) ).toString().toUInt());
                    d->page.setTitle(attrs.value( QString( "title" ) ).toString());
                    d->page.setNs(attrs.value( QString( "ns" ) ).toString().toUInt());
                    d->page.setTouched(QDateTime::fromString(attrs.value( QString( "touched" ) ).toString(), "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'"));
                    d->page.setLastRevId(attrs.value( QString( "lastrevid" ) ).toString().toUInt());
                    d->page.setCounter(attrs.value( QString( "counter" ) ).toString().toUInt());
                    d->page.setLength(attrs.value( QString( "length" ) ).toString().toUInt());
                    d->page.setStarttimestamp(QDateTime::fromString(attrs.value( QString( "starttimestamp" ) ).toString(), "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'"));
                    d->page.setEditToken(attrs.value( QString( "edittoken" ) ).toString());
                    d->page.setTalkid(attrs.value( QString( "talkid" ) ).toString().toUInt());
                    d->page.setFullurl(QUrl(attrs.value( QString( "fullurl" ) ).toString()));
                    d->page.setEditurl(QUrl(attrs.value( QString( "editurl" ) ).toString()));
                    d->page.setReadable(attrs.value( QString( "readable" ) ).toString());
                    d->page.setPreload(attrs.value( QString( "preload" ) ).toString());
                }
                else if (reader.name() == "protection")
                {
                    protect.clear();
                }
                else if (reader.name() == "pr")
                {
                    QString expiry(attrs.value( QString( "expiry" ) ).toString());
                    QString level(attrs.value( QString( "level" ) ).toString());
                    QString type(attrs.value( QString( "type" ) ).toString());
                    QString source = "";

                    if(!attrs.value( QString( "source" ) ).toString().isEmpty())
                    {
                        source = attrs.value( QString( "source" ) ).toString();
                    }
                    else if(!attrs.value( QString( "cascade" ) ).toString().isEmpty())
                    {
                        source = attrs.value( QString( "cascade" ) ).toString();
                    }

                    Protection p;
                    p.setExpiry(expiry);
                    p.setLevel(level);
                    p.setType(type);
                    p.setSource(source);
                    protect.push_back(p);
                }
            }
            else if (token == QXmlStreamReader::EndElement)
            {
                if (reader.name() == "page")
                {
                    d->protections = protect;
                }
            }
        }
        if (!reader.hasError())
        {
            setError(KJob::NoError);
            emit protection(protect);
            emit page(d->page);
        }
        else
        {
            setError(Job::XmlError);
        }
    }
    else
    {
        setError(Job::NetworkError);
    }

    emitResult();
}

} // namespace mediawiki
