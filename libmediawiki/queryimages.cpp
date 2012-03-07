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

#include "queryimages.moc"

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
    : Job(*new QueryImagesPrivate(mediawiki, "10"), parent)
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
    url.addQueryItem("format",  "xml");
    url.addQueryItem("action",  "query");
    url.addQueryItem("titles",  d->title);
    url.addQueryItem("prop",    "images");
    url.addQueryItem("imlimit", d->limit);
    if (!d->imcontinue.isNull())
    {
        url.addQueryItem("imcontinue", d->imcontinue);
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
                if (reader.name() == "images")
                {
                    if (reader.attributes().value("imcontinue").isNull())
                    {
                        imagesReceived.clear();
                    }
                    else
                    {
                        d->imcontinue = reader.attributes().value("imcontinue").toString();
                    }
                }
                else if (reader.name() == "im")
                {
                    Image image;
                    image.setNamespaceId( reader.attributes().value("ns").toString().toUInt());
                    image.setTitle(reader.attributes().value("title").toString());
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
