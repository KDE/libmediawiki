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
 * @author Copyright (C) 2010 by Vincent Garcia
 *         <a href="mailto:xavier dot vincent dot garcia at gmail dot com">xavier dot vincent dot garcia at gmail dot com</a>
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

#include "parse.moc"

// Qt includes

#include <QtCore/QTimer>
#include <QtCore/QXmlStreamReader>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QDebug>

// Local includes

#include "job_p.h"
#include "mediawiki.h"

namespace mediawiki
{

class ParsePrivate : public JobPrivate
{

public:

    ParsePrivate(MediaWiki& mediawiki)
        : JobPrivate(mediawiki)
    {
    }

    QMap<QString, QString> requestParameter;
};

Parse::Parse(MediaWiki& mediawiki, QObject* const parent)
    : Job(*new ParsePrivate(mediawiki), parent)
{
}

Parse::~Parse()
{
}

void Parse::setText(const QString& param)
{
    Q_D(Parse);
    d->requestParameter["text"] = param;
}

void Parse::setTitle(const QString& param)
{
    Q_D(Parse);
    d->requestParameter["title"] = param;
}

void Parse::setPageName(const QString& param)
{
    Q_D(Parse);
    d->requestParameter["page"] = param;
}

void Parse::setUseLang(const QString& param)
{
    Q_D(Parse);
    d->requestParameter["uselang"] = param;
}

void Parse::start()
{
    QTimer::singleShot(0, this, SLOT(doWorkSendRequest()));
}

void Parse::doWorkSendRequest()
{
    Q_D(Parse);

    // Set the url
    QUrl url = d->mediawiki.url();
    url.addQueryItem("format", "xml");
    url.addQueryItem("action", "parse");

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

    // Send the request
    d->reply = d->manager->get(request);
    connectReply();
    connect(d->reply, SIGNAL(finished()),
            this, SLOT(doWorkProcessReply()));
}

void Parse::doWorkProcessReply()
{
    Q_D(Parse);
    disconnect(d->reply, SIGNAL(finished()),
               this, SLOT(doWorkProcessReply()));

    if (d->reply->error() == QNetworkReply::NoError)
    {
        QXmlStreamReader reader(d->reply);
        QString text;

        while (!reader.atEnd() && !reader.hasError())
        {
            QXmlStreamReader::TokenType token = reader.readNext();

            if (token == QXmlStreamReader::StartElement)
            {
                if (reader.name() == "text")
                {
                    text = reader.text().toString();
                    setError(Parse::NoError);
                }
                else if (reader.name() == "error")
                {
                    if (reader.attributes().value("code").toString() == QString("params"))
                        this->setError(this->TooManyParams);
                    else if (reader.attributes().value("code").toString() == QString("missingtitle"))
                        this->setError(this->MissingPage);

                    d->reply->close();
                    d->reply->deleteLater();
                    emitResult();
                    return;
                }
            }
        }

        if (!reader.hasError())
        {
            emit result(text);
        }
        else
        {
            setError(Parse::XmlError);
        }
    }
    else
    {
        setError(Parse::NetworkError);
    }

    d->reply->close();
    d->reply->deleteLater();
    emitResult();
}

} // namespace mediawiki
