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

#include "edit.moc"

// Qt includes

#include <QtCore/QTimer>
#include <QtCore/QUrl>
#include <QtCore/QXmlStreamReader>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtCore/QCryptographicHash>
#include <QtNetwork/QNetworkCookieJar>
#include <QtCore/QStringList>

// Local includes

#include "mediawiki.h"
#include "queryinfo.h"
#include "job_p.h"

namespace mediawiki
{

class Result
{
public:

    unsigned int m_captchaId;
    QVariant     m_captchaQuestion;
    QString      m_captchaAnswer;
};

class EditPrivate : public JobPrivate
{
public:

    EditPrivate(MediaWiki& mediawiki)
        : JobPrivate(mediawiki)
    {
    }

    static int error(const QString& error)
    {
        QString temp = error;
        int ret      = 0;
        QStringList list;
        list    << "notext"
                << "invalidsection"
                << "protectedtitle"
                << "cantcreate"
                << "cantcreateanon"
                << "articleexists"
                << "noimageredirectanon"
                << "noimageredirect"
                << "spamdetected"
                << "filtered"
                << "contenttoobig"
                << "noeditanon"
                << "noedit"
                << "pagedeleted"
                << "emptypage"
                << "emptynewsection"
                << "editconflict"
                << "revwrongpage"
                << "undofailure";
        ret = list.indexOf(temp.remove(QChar('-')));
        if(ret == -1)
        {
            ret = 0;
        }
        return  ret + (int)Edit::TextMissing ;
    }

    QUrl                   baseUrl;
    QMap<QString, QString> requestParameter;
    Result                 result;
};

Edit::Edit(MediaWiki& media, QObject* parent)
    : Job(*new EditPrivate(media), parent)
{}

void Edit::setUndoAfter(int undoafter)
{
    Q_D(Edit);
    d->requestParameter["undoafter"] = QString::number(undoafter);
}

void Edit::setUndo(int undo)
{
    Q_D(Edit);
    d->requestParameter["undo"] = QString::number(undo);
}

void Edit::setPrependText(const QString& prependText)
{
    Q_D(Edit);
    d->requestParameter["prependtext"] = prependText;
    d->requestParameter["md5"] = "";
}

void Edit::setAppendText(const QString& appendText)
{
    Q_D(Edit);
    d->requestParameter["appendtext"] = appendText;
    d->requestParameter["md5"]        = "";
}

void Edit::setPageName(const QString& pageName)
{
    Q_D(Edit);
    d->requestParameter["title"] = pageName;
}

void Edit::setToken(const QString& token)
{
    Q_D(Edit);
    d->requestParameter["token"] = token;
}

void Edit::setBaseTimestamp(const QDateTime& baseTimestamp)
{
    Q_D(Edit);
    d->requestParameter["basetimestamp"] = baseTimestamp.toString("yyyy-MM-ddThh:mm:ssZ");
}

void Edit::setStartTimestamp(const QDateTime& startTimestamp)
{
    Q_D(Edit);
    d->requestParameter["starttimestamp"] = startTimestamp.toString("yyyy-MM-ddThh:mm:ssZ");
}

void Edit::setText(const QString& text)
{
    Q_D(Edit);
    d->requestParameter["text"] = text;
    d->requestParameter["md5"]  = "";
}

void Edit::setRecreate(bool recreate)
{
    Q_D(Edit);
    if(recreate)
    {
        d->requestParameter["recreate"] = "on";
        d->requestParameter["md5"]      = "";
    }
}

void Edit::setCreateonly(bool createonly)
{
    Q_D(Edit);
    if(createonly)
    {
        d->requestParameter["createonly"] = "on";
        d->requestParameter["md5"]        = "";
    }
}

void Edit::setNocreate(bool norecreate)
{
    Q_D(Edit);
    if(norecreate)
    {
        d->requestParameter["nocreate"] = "on";
        d->requestParameter["md5"]      = "";
    }
}

void Edit::setMinor(bool minor)
{
    Q_D(Edit);
    if(minor)
        d->requestParameter["minor"] = "on";
    else
        d->requestParameter["notminor"] = "on";
}

void Edit::setSection(const QString& section)
{
    Q_D(Edit);
    d->requestParameter["section"] = section;
}

void Edit::setSummary(const QString& summary)
{
    Q_D(Edit);
    d->requestParameter["summary"] = summary;
}

void Edit::setWatchList(Edit::Watchlist watchlist)
{
    Q_D(Edit);
    switch(watchlist) {
    case Edit::watch:
        d->requestParameter["watchlist"] = QString("watch");
        break;
    case Edit::unwatch:
        d->requestParameter["watchlist"] = QString("unwatch");
        break;
    case Edit::nochange:
        d->requestParameter["watchlist"] = QString("nochange");
        break;
    case Edit::preferences:
        d->requestParameter["watchlist"] = QString("preferences");
        break;
    }
}

Edit::~Edit()
{
}

void Edit::start()
{
    Q_D(Edit);
    QueryInfo* info = new QueryInfo(d->mediawiki,this);
    info->setPageName(d->requestParameter["title"]);
    info->setToken("edit");
    connect(info, SIGNAL(page(Page)),
            this, SLOT(doWorkSendRequest(Page)));
    info->start();

}

void Edit::doWorkSendRequest(Page page)
{
    Q_D(Edit);
    d->requestParameter["token"] = page.pageEditToken();
    // Set the url
    QUrl    url = d->mediawiki.url();
            url.addQueryItem("format", "xml");
            url.addQueryItem("action", "edit");

    // Add params
    if(d->requestParameter.contains("md5"))
    {
        QString textmd5 = "";
        QString text    = "";
        if(d->requestParameter.contains("prependtext"))
        {
            textmd5 += d->requestParameter["prependtext"];
            text    += textmd5;
            text.replace(QString("$"), QString("%24"));
            text.replace(QString("&"), QString("%26"));
            text.replace(QString("+"), QString("%2B"));
            text.replace(QString(","), QString("%2C"));
            text.replace(QString("/"), QString("%2F"));
            text.replace(QString(":"), QString("%3A"));
            text.replace(QString(";"), QString("%3B"));
            text.replace(QString("="), QString("%3D"));
            text.replace(QString("?"), QString("%3F"));
            text.replace(QString("@"), QString("%40"));
            d->requestParameter["prependtext"] = text;
        }
        if(d->requestParameter.contains("appendtext"))
        {
            textmd5 += d->requestParameter["appendtext"];
            text    += textmd5;
            text.replace(QString("$"), QString("%24"));
            text.replace(QString("&"), QString("%26"));
            text.replace(QString("+"), QString("%2B"));
            text.replace(QString(","), QString("%2C"));
            text.replace(QString("/"), QString("%2F"));
            text.replace(QString(":"), QString("%3A"));
            text.replace(QString(";"), QString("%3B"));
            text.replace(QString("="), QString("%3D"));
            text.replace(QString("?"), QString("%3F"));
            text.replace(QString("@"), QString("%40"));
            d->requestParameter["appendtext"] = text;
         }
        if(d->requestParameter.contains("text"))
        {
            textmd5 = d->requestParameter["text"];
            text    = textmd5;
            text.replace(QString("$"), QString("%24"));
            text.replace(QString("&"), QString("%26"));
            text.replace(QString("+"), QString("%2B"));
            text.replace(QString(","), QString("%2C"));
            text.replace(QString("/"), QString("%2F"));
            text.replace(QString(":"), QString("%3A"));
            text.replace(QString(";"), QString("%3B"));
            text.replace(QString("="), QString("%3D"));
            text.replace(QString("?"), QString("%3F"));
            text.replace(QString("@"), QString("%40"));
            d->requestParameter["text"] = text;
        }
        QByteArray hash = QCryptographicHash::hash(textmd5.toUtf8(),QCryptographicHash::Md5);
        d->requestParameter["md5"] = hash.toHex();
    }

    QMapIterator<QString, QString> i(d->requestParameter);
    while (i.hasNext())
    {
        i.next();
        if(i.key() != "token")
            url.addQueryItem(i.key(),i.value());
    }

    QByteArray cookie = "";
    QList<QNetworkCookie> mediawikiCookies = d->manager->cookieJar()->cookiesForUrl(d->mediawiki.url());
    for(int i = 0 ; i < mediawikiCookies.size(); ++i)
    {
        cookie += mediawikiCookies.at(i).toRawForm(QNetworkCookie::NameAndValueOnly);
        cookie += ';';
    }
    // Add the token
    QString token = d->requestParameter["token"];
    token.replace("+", "%2B");
    token.replace("\\", "%5C");
    url.addQueryItem(QByteArray("token"), token);
    d->baseUrl = url;

    // Set the request
    QNetworkRequest request( url );
    request.setRawHeader("User-Agent", d->mediawiki.userAgent().toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader( "Cookie", cookie );

    setPercent(25); // Request ready.

    // Send the request
    d->reply = d->manager->post( request, url.toString().toUtf8() );
    connectReply();
    connect( d->reply, SIGNAL(finished()),
             this, SLOT(finishedEdit()) );

    setPercent(50); // Request sent.
}

void Edit::finishedEdit()
{
    Q_D(Edit);
    disconnect(d->reply, SIGNAL(finished()),
               this, SLOT(finishedEdit()));

    setPercent(75); // Response received.

    if ( d->reply->error() != QNetworkReply::NoError )
    {
        this->setError(this->NetworkError);
        d->reply->close();
        d->reply->deleteLater();
        emitResult();
        return;
    }
    QXmlStreamReader reader( d->reply );
    while ( !reader.atEnd() && !reader.hasError() )
    {
        QXmlStreamReader::TokenType token = reader.readNext();
        if ( token == QXmlStreamReader::StartElement )
        {
            QXmlStreamAttributes attrs = reader.attributes();
            if ( reader.name() == QString( "edit" ) )
            {
                if ( attrs.value( QString( "result" ) ).toString() == "Success" )
                {
                    setPercent(100); // Response parsed successfully.
                    this->setError(KJob::NoError);
                    d->reply->close();
                    d->reply->deleteLater();
                    emitResult();
                    return;
                }
                else if ( attrs.value( QString( "result" ) ).toString() == "Failure" )
                {
                    this->setError(KJob::NoError);
                    reader.readNext();
                    attrs = reader.attributes();
                    d->result.m_captchaId = attrs.value( QString( "id" ) ).toString().toUInt() ;
                    if (!attrs.value( QString( "question" ) ).isEmpty())
                        d->result.m_captchaQuestion = QVariant(attrs.value( QString( "question" ) ).toString()) ;
                    else if (!attrs.value( QString( "url" ) ).isEmpty())
                        d->result.m_captchaQuestion = QVariant(attrs.value( QString( "url" ) ).toString()) ;
                }
            }
            else if ( reader.name() == QString( "error" ) )
            {
                this->setError(EditPrivate::error(attrs.value( QString( "code" ) ).toString()));
                d->reply->close();
                d->reply->deleteLater();
                emitResult();
                return;
            }
        }
        else if ( token == QXmlStreamReader::Invalid && reader.error() != QXmlStreamReader::PrematureEndOfDocumentError)
        {
            this->setError(this->XmlError);
            d->reply->close();
            d->reply->deleteLater();
            emitResult();
            return;
        }
    }
    d->reply->close();
    d->reply->deleteLater();
    emit resultCaptcha(d->result.m_captchaQuestion);
}

void Edit::finishedCaptcha(const QString& captcha)
{
    Q_D(Edit);
    d->result.m_captchaAnswer = captcha;
    QUrl url                  = d->baseUrl;
    url.addQueryItem("CaptchaId", QString::number(d->result.m_captchaId));
    url.addQueryItem("CaptchaAnswer", d->result.m_captchaAnswer);
    QString data      = url.toString();
    QByteArray cookie = "";
    QList<QNetworkCookie> mediawikiCookies = d->manager->cookieJar()->cookiesForUrl(d->mediawiki.url());
    for(int i = 0 ; i < mediawikiCookies.size(); ++i)
    {
        cookie += mediawikiCookies.at(i).toRawForm(QNetworkCookie::NameAndValueOnly);
        cookie += ';';
    }

    // Set the request
    QNetworkRequest request( url );
    request.setRawHeader("User-Agent", d->mediawiki.userAgent().toUtf8());
    request.setRawHeader( "Cookie", cookie );
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    // Send the request
    d->reply = d->manager->post( request, data.toUtf8() );
    connect( d->reply, SIGNAL(finished()),
             this, SLOT(finishedEdit()) );
}

} // namespace mediawiki
