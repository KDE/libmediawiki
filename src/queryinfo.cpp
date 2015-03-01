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

#include "queryinfo.h"


// Qt includes

#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>
#include <QtCore/QXmlStreamReader>
#include <QtCore/QRegExp>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkCookie>
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
    d->requestParameter[QStringLiteral("titles")] = title;
}

void QueryInfo::setToken(const QString& token)
{
    Q_D(QueryInfo);
    d->requestParameter[QStringLiteral("intoken")] = token;
}

void QueryInfo::setPageId(unsigned int id)
{
    Q_D(QueryInfo);
    d->requestParameter[QStringLiteral("pageids")] = QString::number(id);
}

void QueryInfo::setRevisionId(unsigned int id)
{
    Q_D(QueryInfo);
    d->requestParameter[QStringLiteral("revids")] = QString::number(id);
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
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("format"), QStringLiteral("xml"));
    query.addQueryItem(QStringLiteral("action"), QStringLiteral("query"));
    query.addQueryItem(QStringLiteral("prop"),   QStringLiteral("info"));
    query.addQueryItem(QStringLiteral("inprop"), QStringLiteral("protection|talkid|watched|subjectid|url|readable|preload"));

    QMapIterator<QString, QString> i(d->requestParameter);
    while (i.hasNext())
    {
        i.next();
        query.addQueryItem(i.key(),              // TODO : check UTF-8 support
                           i.value());           // TODO : check UTF-8 support
    }
    url.setQuery(query);

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
        QString content = QString::fromUtf8(d->reply->readAll());
        QRegExp regex(QStringLiteral("&(?!\\w+;)"));
        content.replace(regex, QStringLiteral("&amp;"));
        QXmlStreamReader reader(content);
        QVector<Protection> protect;

        while (!reader.atEnd() && !reader.hasError())
        {
            QXmlStreamReader::TokenType token = reader.readNext();
            QXmlStreamAttributes attrs = reader.attributes();

            if (token == QXmlStreamReader::StartElement)
            {
                if (reader.name() == QLatin1String("page"))
                {
                    d->page.setPageId(attrs.value(QStringLiteral("pageid")).toString().toUInt());
                    d->page.setTitle(attrs.value(QStringLiteral("title")).toString());
                    d->page.setNs(attrs.value(QStringLiteral("ns")).toString().toUInt());
                    d->page.setTouched(QDateTime::fromString(attrs.value(QStringLiteral("touched")).toString(), QStringLiteral("yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'")));
                    d->page.setLastRevId(attrs.value(QStringLiteral("lastrevid")).toString().toUInt());
                    d->page.setCounter(attrs.value(QStringLiteral("counter")).toString().toUInt());
                    d->page.setLength(attrs.value(QStringLiteral("length")).toString().toUInt());
                    d->page.setStarttimestamp(QDateTime::fromString(attrs.value(QStringLiteral("starttimestamp")).toString(), QStringLiteral("yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'")));
                    d->page.setEditToken(attrs.value(QStringLiteral("edittoken")).toString());
                    d->page.setTalkid(attrs.value(QStringLiteral("talkid")).toString().toUInt());
                    d->page.setFullurl(QUrl(attrs.value(QStringLiteral("fullurl")).toString()));
                    d->page.setEditurl(QUrl(attrs.value(QStringLiteral("editurl")).toString()));
                    d->page.setReadable(attrs.value(QStringLiteral("readable")).toString());
                    d->page.setPreload(attrs.value(QStringLiteral("preload")).toString());
                }
                else if (reader.name() == QLatin1String("protection"))
                {
                    protect.clear();
                }
                else if (reader.name() == QLatin1String("pr"))
                {
                    QString expiry(attrs.value(QStringLiteral("expiry")).toString());
                    QString level(attrs.value(QStringLiteral("level")).toString());
                    QString type(attrs.value(QStringLiteral("type")).toString());
                    QString source;

                    if(!attrs.value(QStringLiteral("source")).toString().isEmpty())
                    {
                        source = attrs.value(QStringLiteral("source")).toString();
                    }
                    else if(!attrs.value(QStringLiteral("cascade")).toString().isEmpty())
                    {
                        source = attrs.value(QStringLiteral("cascade")).toString();
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
                if (reader.name() == QLatin1String("page"))
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
