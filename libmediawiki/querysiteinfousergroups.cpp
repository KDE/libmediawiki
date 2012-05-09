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
 * @author Copyright (C) 2010 by Ludovic Delfau
 *         <a href="mailto:ludovicdelfau at gmail dot com">ludovicdelfau at gmail dot com</a>
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

#include "querysiteinfousergroups.moc"

// Qt includes

#include <QtCore/QTimer>
#include <QtCore/QXmlStreamReader>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

// Local includes

#include "mediawiki.h"
#include "job_p.h"

namespace mediawiki
{

class QuerySiteinfoUsergroupsPrivate : public JobPrivate
{
public:

    QuerySiteinfoUsergroupsPrivate(MediaWiki& mediawiki, QNetworkAccessManager* const manager, bool includeNumber)
            : JobPrivate(mediawiki),
              manager(manager),
              includeNumber(includeNumber)
    {
    }

    QNetworkAccessManager* const manager;
    bool                         includeNumber;
};

QuerySiteinfoUsergroups::QuerySiteinfoUsergroups(MediaWiki& mediawiki, QObject* const parent)
    : Job(*new QuerySiteinfoUsergroupsPrivate(mediawiki, new QNetworkAccessManager(), false), parent)
{
}

QuerySiteinfoUsergroups::~QuerySiteinfoUsergroups()
{
}

void QuerySiteinfoUsergroups::setIncludeNumber(bool includeNumber)
{
    Q_D(QuerySiteinfoUsergroups);
    d->includeNumber = includeNumber;
}

void QuerySiteinfoUsergroups::start()
{
    QTimer::singleShot(0, this, SLOT(doWorkSendRequest()));
}

void QuerySiteinfoUsergroups::doWorkSendRequest()
{
    Q_D(QuerySiteinfoUsergroups);

    // Set the url
    QUrl url = d->mediawiki.url();
    url.addQueryItem("format", "xml");
    url.addQueryItem("action", "query");
    url.addQueryItem("meta",   "siteinfo");
    url.addQueryItem("siprop", "usergroups");
    if (d->includeNumber)
    {
        url.addQueryItem("sinumberingroup", QString());
    }

    // Set the request
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", d->mediawiki.userAgent().toUtf8());

    // Send the request
    d->reply = d->manager->get(request);
    connectReply();

    connect(d->reply, SIGNAL(finished()),
            this, SLOT(doWorkProcessReply()));
}

void QuerySiteinfoUsergroups::doWorkProcessReply()
{
    Q_D(QuerySiteinfoUsergroups);

    disconnect(d->reply, SIGNAL(finished()),
               this, SLOT(doWorkProcessReply()));

    if (d->reply->error() == QNetworkReply::NoError)
    {
        QList<UserGroup> results;
        QString          name;
        QList<QString>   rights;
        unsigned int     number = 0;
        QXmlStreamReader reader(d->reply);

        while (!reader.atEnd() && !reader.hasError())
        {
            QXmlStreamReader::TokenType token = reader.readNext();

            if (token == QXmlStreamReader::StartElement)
            {
                if (reader.name() == "group")
                {
                    name = reader.attributes().value("name").toString();
                    if (d->includeNumber)
                    {
                        number = reader.attributes().value("number").toString().toUInt();
                    }
                }
                else if (reader.name() == "rights")
                {
                    rights.clear();
                }
                else if (reader.name() == "permission")
                {
                    reader.readNext();
                    rights.push_back(reader.text().toString());
                }
            }
            else if (token == QXmlStreamReader::EndElement)
            {
                if (reader.name() == "group")
                {
                    UserGroup usergroup;
                    usergroup.setName(name);
                    usergroup.setRights(rights);

                    if(d->includeNumber)
                    {
                        usergroup.setNumber(number);
                    }

                    results.push_back(usergroup);
                }
            }
        }

        if (!reader.hasError())
        {
            setError(Job::NoError);
            emit usergroups(results);
        }
        else
        {
            setError(QuerySiteinfoUsergroups::XmlError);
        }
    }
    else
    {
        setError(QuerySiteinfoUsergroups::NetworkError);
    }

    emitResult();
}

} // namespace mediawiki
