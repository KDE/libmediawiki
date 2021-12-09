/** ===========================================================
 * @file
 *
 * This file is a part of KDE project
 * <a href="https://commits.kde.org/libmediawiki">libmediawiki</a>
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

#include "queryimages.h"


// Qt includes

#include <QTimer>
#include <QUrl>
#include <QUrlQuery>
#include <QXmlStreamReader>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

// Local includes

#include "mediawiki.h"
#include "job_p.h"

namespace mediawiki
{

class QueryImagesPrivate : public JobPrivate
{
public:

    QueryImagesPrivate(MediaWiki& mediawiki, const QString& limit)
        : JobPrivate(mediawiki),
          limit(limit)
    {
    }

    QString title;
    QString limit;
    QString imcontinue;
};

QueryImages::QueryImages(MediaWiki& mediawiki, QObject* const parent)
    : Job(*new QueryImagesPrivate(mediawiki, QStringLiteral("10")), parent)
{
}

QueryImages::~QueryImages()
{
}

void QueryImages::setTitle(const QString& title)
{
    Q_D(QueryImages);
    d->title = title;
}

void QueryImages::setLimit(unsigned int limit)
{
    Q_D(QueryImages);
    d->limit = QString::number(limit);
}

void QueryImages::start()
{
    QTimer::singleShot(0, this, SLOT(doWorkSendRequest()));
}

void QueryImages::doWorkSendRequest()
{
    Q_D(QueryImages);

    // Set the url
    QUrl url = d->mediawiki.url();
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("format"),  QStringLiteral("xml"));
    query.addQueryItem(QStringLiteral("action"),  QStringLiteral("query"));
    query.addQueryItem(QStringLiteral("titles"),  d->title);
    query.addQueryItem(QStringLiteral("prop"),    QStringLiteral("images"));
    query.addQueryItem(QStringLiteral("imlimit"), d->limit);
    if (!d->imcontinue.isNull())
    {
        query.addQueryItem(QStringLiteral("imcontinue"), d->imcontinue);
    }
    url.setQuery(query);

    // Set the request
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", d->mediawiki.userAgent().toUtf8());

    // Send the request
    d->reply = d->manager->get(request);
    connectReply();
    connect(d->reply, SIGNAL(finished()),
            this, SLOT(doWorkProcessReply()));
}

void QueryImages::doWorkProcessReply()
{
    Q_D(QueryImages);

    disconnect(d->reply, SIGNAL(finished()),
               this, SLOT(doWorkProcessReply()));

    if (d->reply->error() == QNetworkReply::NoError)
    {
        QList<Image> imagesReceived;
        d->imcontinue.clear();
        QXmlStreamReader reader(d->reply);

        while (!reader.atEnd() && !reader.hasError())
        {
            QXmlStreamReader::TokenType token = reader.readNext();
            if (token == QXmlStreamReader::StartElement)
            {
                if (reader.name() == QLatin1String("images"))
                {
                    if (reader.attributes().value(QStringLiteral("imcontinue")).isNull())
                    {
                        imagesReceived.clear();
                    }
                    else
                    {
                        d->imcontinue = reader.attributes().value(QStringLiteral("imcontinue")).toString();
                    }
                }
                else if (reader.name() == QLatin1String("im"))
                {
                    Image image;
                    image.setNamespaceId( reader.attributes().value(QStringLiteral("ns")).toString().toUInt());
                    image.setTitle(reader.attributes().value(QStringLiteral("title")).toString());
                    imagesReceived.push_back(image);
                }
            }
        }

        if (!reader.hasError())
        {
            emit images(imagesReceived);

            if (!d->imcontinue.isNull())
            {
                QTimer::singleShot(0, this, SLOT(doWorkSendRequest()));
                return;
            }
            else
            {
                setError(KJob::NoError);
            }
        }
        else
        {
            setError(QueryImages::XmlError);
        }
    }
    else
    {
        setError(QueryImages::NetworkError);
    }

    emitResult();
}

} // namespace mediawiki;
