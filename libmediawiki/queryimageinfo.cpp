/** ===========================================================
 * @file
 *
 * This file is a part of KDE project
 * <a href="https://projects.kde.org/projects/extragear/libs/libmediawiki">libmediawiki</a>
 *
 * @date   2011-03-22
 * @brief  a MediaWiki C++ interface for KDE
 *
 * @author Copyright (C) 2011-2013 by Gilles Caulier
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

#include "queryimageinfo.moc"

// Qt includes

#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtCore/QXmlStreamReader>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

// Local includes

#include "mediawiki.h"
#include "job_p.h"

namespace mediawiki
{

class QueryImageinfoPrivate : public JobPrivate
{

public:

    QueryImageinfoPrivate(MediaWiki& mediawiki)
        : JobPrivate(mediawiki)
    {
        onlyOneSignal = false;
    }

    static inline qint64 toQInt64(const QString& qstring)
    {
        bool   ok;
        qint64 result = qstring.toLongLong(&ok);
        return (ok ? result : -1);
    }

    static inline void addQueryItemIfNotNull(QUrl& url, const QString& key, const QString& value)
    {
        if (!value.isNull())
        {
            url.addQueryItem(key, value);
        }
    }

public:

    bool    onlyOneSignal;

    QString title;
    QString iiprop;
    QString limit;
    QString begin;
    QString end;
    QString width;
    QString height;
};

QueryImageinfo::QueryImageinfo(MediaWiki& mediawiki, QObject* const parent)
    : Job(*new QueryImageinfoPrivate(mediawiki), parent)
{
    Q_D(QueryImageinfo);
    Q_UNUSED(d);
}

QueryImageinfo::~QueryImageinfo()
{
}

void QueryImageinfo::setTitle(const QString& title)
{
    Q_D(QueryImageinfo);
    d->title = title;
}

void QueryImageinfo::setProperties(Properties properties)
{
    Q_D(QueryImageinfo);

    QString iiprop;

    if (properties & QueryImageinfo::Timestamp)
    {
        iiprop.append("timestamp|");
    }
    if (properties & QueryImageinfo::User)
    {
        iiprop.append("user|");
    }
    if (properties & QueryImageinfo::Comment)
    {
        iiprop.append("comment|");
    }
    if (properties & QueryImageinfo::Url)
    {
        iiprop.append("url|");
    }
    if (properties & QueryImageinfo::Size)
    {
        iiprop.append("size|");
    }
    if (properties & QueryImageinfo::Sha1)
    {
        iiprop.append("sha1|");
    }
    if (properties & QueryImageinfo::Mime)
    {
        iiprop.append("mime|");
    }
    if (properties & QueryImageinfo::Metadata)
    {
        iiprop.append("metadata|");
    }

    iiprop.chop(1);
    d->iiprop = iiprop;
}

void QueryImageinfo::setLimit(unsigned int limit)
{
    Q_D(QueryImageinfo);
    d->limit = (limit > 0u) ? QString::number(limit) : QString();
}

void QueryImageinfo::setOnlyOneSignal(bool onlyOneSignal)
{
    Q_D(QueryImageinfo);
    d->onlyOneSignal = onlyOneSignal;
}

void QueryImageinfo::setBeginTimestamp(const QDateTime& begin)
{
    Q_D(QueryImageinfo);
    d->begin = begin.toString("yyyy-MM-dd'T'hh:mm:ss'Z'");
}

void QueryImageinfo::setEndTimestamp(const QDateTime& end)
{
    Q_D(QueryImageinfo);
    d->end = end.toString("yyyy-MM-dd'T'hh:mm:ss'Z'");
}

void QueryImageinfo::setWidthScale(unsigned int width)
{
    Q_D(QueryImageinfo);
    d->width = (width > 0u) ? QString::number(width) : QString();
}

void QueryImageinfo::setHeightScale(unsigned int height)
{
    Q_D(QueryImageinfo);
    d->height = (height > 0u) ? QString::number(height) : QString();

    if (d->width.isNull())
    {
        d->width = d->height;
    }
}

void QueryImageinfo::start()
{
    QTimer::singleShot(0, this, SLOT(doWorkSendRequest()));
}

void QueryImageinfo::doWorkSendRequest()
{
    Q_D(QueryImageinfo);

    // Set the url
    QUrl url = d->mediawiki.url();
    url.addQueryItem("format", "xml");
    url.addQueryItem("action", "query");
    url.addQueryItem("titles", d->title); //FIXME: Job error because title is required
    url.addQueryItem("prop",   "imageinfo");
    QueryImageinfoPrivate::addQueryItemIfNotNull(url, "iiprop",      d->iiprop);
    QueryImageinfoPrivate::addQueryItemIfNotNull(url, "iilimit",     d->limit);
    QueryImageinfoPrivate::addQueryItemIfNotNull(url, "iistart",     d->begin);
    QueryImageinfoPrivate::addQueryItemIfNotNull(url, "iiend",       d->end);
    QueryImageinfoPrivate::addQueryItemIfNotNull(url, "iiurlwidth",  d->width);
    QueryImageinfoPrivate::addQueryItemIfNotNull(url, "iiurlheight", d->height);

    // Set the request
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", d->mediawiki.userAgent().toUtf8());

    // Send the request
    d->reply = d->manager->get(request);
    connectReply();

    connect(d->reply, SIGNAL(finished()),
            this, SLOT(doWorkProcessReply()));
}

void QueryImageinfo::doWorkProcessReply()
{
    Q_D(QueryImageinfo);

    disconnect(d->reply, SIGNAL(finished()),
               this, SLOT(doWorkProcessReply()));

    d->begin.clear();

    if (d->reply->error() == QNetworkReply::NoError)
    {
        QXmlStreamReader reader(d->reply);
        QList<Imageinfo> imageinfos;
        Imageinfo imageinfo;

        while (!reader.atEnd() && !reader.hasError())
        {
            QXmlStreamReader::TokenType token = reader.readNext();

            if (token == QXmlStreamReader::StartElement)
            {
                if (reader.name() == "imageinfo")
                {
                    if (!reader.attributes().value("iistart").isNull())
                    {
                        d->begin = reader.attributes().value("iistart").toString();
                    }
                }
                else if (reader.name() == "ii")
                {
                    imageinfo.setTimestamp(QDateTime::fromString(reader.attributes().value("timestamp").toString(), "yyyy-MM-dd'T'hh:mm:ss'Z'"));
                    imageinfo.setUser(reader.attributes().value("user").toString());
                    imageinfo.setComment(reader.attributes().value("comment").toString());
                    imageinfo.setUrl(QUrl::fromPercentEncoding(reader.attributes().value("url").toString().toLocal8Bit()));
                    imageinfo.setDescriptionUrl(QUrl::fromPercentEncoding(reader.attributes().value("descriptionurl").toString().toLocal8Bit()));
                    imageinfo.setThumbUrl(QUrl::fromPercentEncoding(reader.attributes().value("thumburl").toString().toLocal8Bit()));
                    imageinfo.setThumbWidth(QueryImageinfoPrivate::toQInt64(reader.attributes().value("thumbwidth").toString()));
                    imageinfo.setThumbHeight(QueryImageinfoPrivate::toQInt64(reader.attributes().value("thumbheight").toString()));
                    imageinfo.setSize(QueryImageinfoPrivate::toQInt64(reader.attributes().value("size").toString()));
                    imageinfo.setWidth(QueryImageinfoPrivate::toQInt64(reader.attributes().value("width").toString()));
                    imageinfo.setHeight(QueryImageinfoPrivate::toQInt64(reader.attributes().value("height").toString()));
                    imageinfo.setSha1(reader.attributes().value("sha1").toString());
                    imageinfo.setMime(reader.attributes().value("mime").toString());
                }
                else if (reader.name() == "metadata")
                {
                    if (!reader.attributes().isEmpty())
                    {
                        imageinfo.metadata()[reader.attributes().value("name").toString()] = reader.attributes().value("value").toString();
                    }
                }
            }
            else if (token == QXmlStreamReader::EndElement)
            {
                if (reader.name() == "ii")
                {
                    imageinfos.push_back(imageinfo);
                    imageinfo = Imageinfo();
                }
            }
        }

        if (!reader.hasError())
        {
            emit result(imageinfos);

            if (d->begin.isNull() || d->onlyOneSignal)
            {
                setError(KJob::NoError);
            }
            else
            {
                QTimer::singleShot(0, this, SLOT(doWorkSendRequest()));
                return;
            }
        }
        else
        {
            setError(QueryImageinfo::XmlError);
        }
    }
    else
    {
        setError(QueryImageinfo::NetworkError);
    }

    emitResult();
}

} // namespace mediawiki
