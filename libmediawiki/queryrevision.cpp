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
 * @author Copyright (C) 2011 by Manuel Campomanes
 *         <a href="mailto:campomanes dot manuel at gmail dot com">campomanes dot manuel at gmail dot com</a>
 * @author Copyright (C) 2010 by Hormiere Guillaume
 *         <a href="mailto:hormiere dot guillaume at gmail dot com">hormiere dot guillaume at gmail dot com</a>
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

#include "queryrevision.moc"

// Qt includes

#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <QtCore/QUrl>
#include <QtCore/QXmlStreamReader>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

// Local includes

#include "mediawiki.h"
#include "job_p.h"

namespace mediawiki
{

class QueryRevisionPrivate : public JobPrivate
{

public:

    QueryRevisionPrivate(MediaWiki& mediawiki)
         : JobPrivate(mediawiki)
    {
    }

    QMap<QString, QString> requestParameter;
};

QueryRevision::QueryRevision(MediaWiki& mediawiki, QObject* const parent)
    : Job(*new QueryRevisionPrivate(mediawiki), parent)
{
}

QueryRevision::~QueryRevision()
{
}

void QueryRevision::start()
{
    QTimer::singleShot(0, this, SLOT(doWorkSendRequest()));
}

void QueryRevision::setPageName(const QString& pageName)
{
    Q_D(QueryRevision);
    d->requestParameter["titles"] = pageName;
}

void QueryRevision::setProperties(Properties properties)
{
    Q_D(QueryRevision);
    QString buff;

    if(properties & QueryRevision::Ids)
    {
        buff.append("ids");
    }

    if(properties & QueryRevision::Flags)
    {
        if (buff.length())
            buff.append('|');

        buff.append("flags");
    }

    if(properties & QueryRevision::Timestamp)
    {
        if (buff.length())
            buff.append('|');

        buff.append("timestamp");
    }

    if(properties & QueryRevision::User)
    {
        if (buff.length())
            buff.append('|');

        buff.append("user");
    }

    if(properties & QueryRevision::Comment)
    {
        if (buff.length())
            buff.append('|');

        buff.append("comment");
    }

    if(properties & QueryRevision::Size)
    {
        if (buff.length())
            buff.append('|');

        buff.append("size");
    }

    if(properties & QueryRevision::Content)
    {
        if (buff.length())
            buff.append('|');

        buff.append("content");
    }

    d->requestParameter["rvprop"] = buff;
}

void QueryRevision::setPageId(unsigned int pageId)
{
    Q_D(QueryRevision);
    d->requestParameter["pageids"] = QString::number(pageId);
}

void QueryRevision::setRevisionId(unsigned int revisionId)
{
    Q_D(QueryRevision);
    d->requestParameter["revids"] = QString::number(revisionId);
}

void QueryRevision::setLimit(int limit)
{
    Q_D(QueryRevision);
    d->requestParameter["rvlimit"] = QString::number(limit);
}

void QueryRevision::setStartId(int startId)
{
    Q_D(QueryRevision);
    d->requestParameter["rvstartid"] = QString::number(startId);
}

void QueryRevision::setEndId(int endId)
{
    Q_D(QueryRevision);
    d->requestParameter["rvendid"] = QString::number(endId);
}

void QueryRevision::setStartTimestamp(const QDateTime& start)
{
    Q_D(QueryRevision);
    d->requestParameter["rvstart"] = start.toString("yyyy-MM-ddThh:mm:ssZ");
}

void QueryRevision::setEndTimestamp(const QDateTime& end)
{
    Q_D(QueryRevision);
    d->requestParameter["rvend"] = end.toString("yyyy-MM-ddThh:mm:ssZ");
}

void QueryRevision::setUser(const QString& user)
{
    Q_D(QueryRevision);
    d->requestParameter["rvuser"] = user;
}

void QueryRevision::setExcludeUser(const QString& excludeUser)
{
    Q_D(QueryRevision);
    d->requestParameter["rvexcludeuser"] = excludeUser;
}

void QueryRevision::setDirection(QueryRevision::Direction direction)
{
    Q_D(QueryRevision);

    if (direction == QueryRevision::Older)
    {
        d->requestParameter["rvdir"] = QString("older");
    }
    else if (direction == QueryRevision::Newer)
    {
        d->requestParameter["rvdir"] = QString("newer");
    }
}

void QueryRevision::setGenerateXML(bool generateXML)
{
    Q_D(QueryRevision);

    if (generateXML)
    {
        d->requestParameter["rvgeneratexml"] = QString("on");
    }
}

void QueryRevision::setSection(int section)
{
    Q_D(QueryRevision);
    d->requestParameter["rvsection"] = QString::number(section);
}

void QueryRevision::setToken(QueryRevision::Token token)
{
    Q_D(QueryRevision);

    if (QueryRevision::Rollback == token)
    {
        d->requestParameter["rvtoken"] = QString("rollback");
    }
}

void QueryRevision::setExpandTemplates(bool expandTemplates)
{
    Q_D(QueryRevision);

    if (expandTemplates)
    {
        d->requestParameter["rvexpandtemplates"] = QString("on");
    }
}
void QueryRevision::doWorkSendRequest()
{
    Q_D(QueryRevision);

    // Set the url
    QUrl url = d->mediawiki.url();
    url.addQueryItem("format", "xml");
    url.addQueryItem("action", "query");
    url.addQueryItem("prop",   "revisions");

    QMapIterator<QString, QString> i(d->requestParameter);

    while (i.hasNext())
    {
        i.next();
        url.addEncodedQueryItem(QByteArray(i.key().toAscii()),           // TODO : check UTF-8 support
                                QByteArray(i.value().toAscii()));        // TODO : check UTF-8 support
    }

    // Set the request
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", d->mediawiki.userAgent().toUtf8());

    setPercent(25); // Request ready.

    // Send the request
    d->reply = d->manager->get(request);
    connectReply();

    connect(d->reply, SIGNAL(finished()), 
            this, SLOT(doWorkProcessReply()));

    setPercent(50); // Request sent.
}

void QueryRevision::doWorkProcessReply()
{
    Q_D(QueryRevision);

    disconnect(d->reply, SIGNAL(finished()),
               this, SLOT(doWorkProcessReply()));

    setPercent(75); // Response received.

    if (d->reply->error() == QNetworkReply::NoError)
    {
        QList<Revision> results;
        Revision        tempR;
        QString         replytmp = QString::fromUtf8(d->reply->readAll());

        if (d->requestParameter.contains("rvgeneratexml"))
        {
            for (int i = replytmp.indexOf("parsetree"); i != -1; i = replytmp.indexOf("parsetree", i+1))
            {
                int count = 0;

                while (count < 2)
                {
                    if (replytmp[i] == '"' && replytmp[i-1] != '\\') count++;
                    if (replytmp[i] == '<')                          replytmp[i] = char(255);
                    if (replytmp[i] == '>')                          replytmp[i] = char(254);
                    ++i;
                }
            }
        }

        QXmlStreamReader reader(replytmp);

        while (!reader.atEnd() && !reader.hasError())
        {
            QXmlStreamReader::TokenType token = reader.readNext();

            if (token == QXmlStreamReader::StartElement)
            {
                if (reader.name() == "page" && d->requestParameter.contains("rvtoken"))
                {
                    tempR.setRollback(reader.attributes().value("rollbacktoken").toString());
                }

                if (reader.name() == "rev")
                {
                    if (d->requestParameter.contains("rvprop"))
                    {
                        QString rvprop = d->requestParameter["rvprop"];

                        if (rvprop.contains("ids"))
                        {
                            tempR.setRevisionId(reader.attributes().value("revid").toString().toInt());
                            tempR.setParentId(reader.attributes().value("parentid").toString().toInt());}

                            if (rvprop.contains("size"))
                                tempR.setSize(reader.attributes().value("size").toString().toInt());

                            if (rvprop.contains("minor"))
                                tempR.setMinorRevision(true);

                            if (rvprop.contains("user"))
                                tempR.setUser(reader.attributes().value("user").toString());

                            if (rvprop.contains("timestamp"))
                                tempR.setTimestamp(QDateTime::fromString(reader.attributes().value("timestamp").toString(),"yyyy-MM-ddThh:mm:ssZ"));

                            if (rvprop.contains("comment"))
                                tempR.setComment(reader.attributes().value("comment").toString());

                            if (d->requestParameter.contains("rvgeneratexml"))
                                tempR.setParseTree(reader.attributes().value("parsetree").toString());

                            if (rvprop.contains("content"))
                                tempR.setContent(reader.readElementText());
                        }

                        results << tempR;
                    }
                    else if (reader.name() == "error")
                    {
                        if (reader.attributes().value("code").toString() == QString("rvrevids"))
                            this->setError(this->WrongRevisionId);
                        else if (reader.attributes().value("code").toString() == QString("rvmultpages"))
                            this->setError(this->MultiPagesNotAllowed);
                        else if (reader.attributes().value("code").toString() == QString("rvaccessdenied"))
                            this->setError(this->TitleAccessDenied);
                        else if (reader.attributes().value("code").toString() == QString("rvbadparams"))
                            this->setError(this->TooManyParams);
                        else if (reader.attributes().value("code").toString() == QString("rvnosuchsection"))
                            this->setError(this->SectionNotFound);

                        d->reply->close();
                        d->reply->deleteLater();
                        //emit revision(QList<Revision>());
                        emitResult();
                        return;
                    }
              }
        }
        if (!reader.hasError())
        {
            setError(KJob::NoError);

            for (int i = 0; i < results.length(); i++)
            {
                results[i].setParseTree(results[i].parseTree().replace(QChar(254), '>'));
                results[i].setParseTree(results[i].parseTree().replace(QChar(255), '<'));
            }

            emit revision(results);
            setPercent(100); // Response parsed successfully.
        }
        else
        {
            setError(XmlError);
            d->reply->close();
            d->reply->deleteLater();
            //emit revision(QList<Revision>());
        }
    }
    else
    {
        setError(NetworkError);
        d->reply->close();
        d->reply->deleteLater();
        //emit revision(QList<Revision>());
    }

    emitResult();
}

} // namespace mediawiki
