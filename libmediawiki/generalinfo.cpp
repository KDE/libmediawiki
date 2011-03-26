/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2011-03-22
 * @brief  a MediaWiki C++ interface for KDE
 *
 * @author Copyright (C) 2011 by Gilles Caulier
 *         <a href="mailto:caulier dot gilles at gmail dot com">caulier dot gilles at gmail dot com</a>
 * @author Copyright (C) 2011 by Manuel Campomanes
 *         <a href="mailto:campomanes dot manuel at gmail dot com">campomanes dot manuel at gmail dot com</a>
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

// C++ includes

#include <algorithm>

// Local includes

#include "mediawiki.h"
#include "generalinfo.h"

namespace mediawiki
{

class GeneralinfoPrivate
{
public:

    QString   mainPage;
    QString   siteName;
    QString   generator;
    QString   phpVersion;
    QString   phpApi;
    QString   dataBaseType;
    QString   dataBaseVersion;
    QString   rev;
    QString   cas;
    QString   licence;
    QString   language;
    QString   fallBack8bitEncoding;
    QString   writeApi;
    QString   timeZone;
    QString   timeOffset;
    QString   articlePath;
    QString   scriptPath;
    QString   script;
    QString   variantArticlePath;
    QString   wikiId;

    QUrl      serverUrl;
    QUrl      url;

    QDateTime time;
};

Generalinfo::Generalinfo()
    : d(new GeneralinfoPrivate())
{}

Generalinfo::Generalinfo(const Generalinfo& other)
    : d(new GeneralinfoPrivate(*(other.d)))
{}

Generalinfo::~Generalinfo()
{
    delete d;
}

Generalinfo& Generalinfo::operator=(Generalinfo& other)
{
    std::swap(d, other.d);
    return *this;
}

QString Generalinfo::mainPage() const
{
    return d->mainPage;
}

void Generalinfo::setMainPage(const QString& mainPage)
{
    d->mainPage = mainPage;
}

QUrl Generalinfo::url() const
{
    return d->url;
}

void Generalinfo::setUrl(const QUrl& url)
{
    d->url = url;
}

QString Generalinfo::siteName() const
{
    return d->siteName;
}

void Generalinfo::setSiteName(const QString& siteName)
{
    d->siteName = siteName;
}

QString Generalinfo::generator() const
{
    return d->generator;
}

void Generalinfo::setGenerator(const QString& generator)
{
    d->generator = generator;
}

QString Generalinfo::phpVersion() const
{
    return d->phpVersion;
}

void Generalinfo::setPhpVersion(const QString& phpVersion)
{
    d->phpVersion = phpVersion;
}

QString Generalinfo::phpApi() const
{
    return d->phpApi;
}

void Generalinfo::setPhpApi(const QString& phpApi)
{
    d->phpApi = phpApi;
}

QString Generalinfo::dataBaseType() const
{
    return d->dataBaseType;
}

void Generalinfo::setDataBaseType(const QString& dataBaseType)
{
    d->dataBaseType = dataBaseType;
}

QString Generalinfo::dataBaseVersion() const
{
    return d->dataBaseVersion;
}

void Generalinfo::setDataBaseVersion(const QString& dataBaseVersion)
{
    d->dataBaseVersion = dataBaseVersion;
}

QString Generalinfo::rev() const
{
    return d->rev;
}

void Generalinfo::setRev(const QString& rev)
{
    d->rev = rev;
}

QString Generalinfo::cas() const
{
    return d->cas;
}

void Generalinfo::setCas(const QString& cas)
{
    d->cas = cas;
}

QString Generalinfo::licence() const
{
    return d->licence;
}

void Generalinfo::setLicence(const QString& licence)
{
    d->licence = licence;
}

QString Generalinfo::language() const
{
    return d->language;
}

void Generalinfo::setLanguage(const QString& language)
{
    d->language = language;
}

QString Generalinfo::fallBack8bitEncoding() const
{
    return d->fallBack8bitEncoding;
}

void Generalinfo::setFallBack8bitEncoding(const QString& fallBack8bitEncoding)
{
    d->fallBack8bitEncoding = fallBack8bitEncoding;
}

QString Generalinfo::writeApi() const
{
    return d->writeApi;
}

void Generalinfo::setWriteApi(const QString& writeApi)
{
    d->writeApi = writeApi;
}

QString Generalinfo::timeZone() const
{
    return d->timeZone;
}

void Generalinfo::setTimeZone(const QString& timeZone)
{
    d->timeZone = timeZone;
}

QString Generalinfo::timeOffset() const
{
    return d->timeOffset;
}

void Generalinfo::setTimeOffset(const QString& timeOffset)
{
    d->timeOffset = timeOffset;
}

QString Generalinfo::articlePath() const
{
    return d->articlePath;
}

void Generalinfo::setArticlePath(const QString& articlePath)
{
    d->articlePath = articlePath;
}

QString Generalinfo::scriptPath() const
{
    return d->scriptPath;
}

void Generalinfo::setScriptPath(const QString& scriptPath)
{
    d->scriptPath = scriptPath;
}

QString Generalinfo::script() const
{
    return d->script;
}

void Generalinfo::setScript(const QString& script)
{
    d->script = script;
}

QString Generalinfo::variantArticlePath() const
{
    return d->variantArticlePath;
}

void Generalinfo::setVariantArticlePath(const QString& variantArticlePath)
{
    d->variantArticlePath = variantArticlePath;
}

QUrl Generalinfo::serverUrl() const
{
    return d->serverUrl;
}

void Generalinfo::setServerUrl(const QUrl& serverUrl)
{
    d->serverUrl = serverUrl;
}

QString Generalinfo::wikiId() const
{
    return d->wikiId;
}

void Generalinfo::setWikiId(const QString& wikiId)
{
    d->wikiId = wikiId;
}

QDateTime Generalinfo::time() const
{
    return d->time;
}

void Generalinfo::setTime(const QDateTime& time)
{
    d->time = time;
}

} // namespace mediawiki

bool operator==(const mediawiki::Generalinfo& lhs, const mediawiki::Generalinfo& rhs)
{
    return lhs.mainPage()             == rhs.mainPage()             &&
           lhs.url()                  == rhs.url()                  &&
           lhs.siteName()             == rhs.siteName()             &&
           lhs.generator()            == rhs.generator()            &&
           lhs.phpVersion()           == rhs.phpVersion()           &&
           lhs.phpApi()               == rhs.phpApi()               &&
           lhs.dataBaseType()         == rhs.dataBaseType()         &&
           lhs.dataBaseVersion()      == rhs.dataBaseVersion()      &&
           lhs.rev()                  == rhs.rev()                  &&
           lhs.cas()                  == rhs.cas()                  &&
           lhs.licence()              == rhs.licence()              &&
           lhs.language()             == rhs.language()             &&
           lhs.fallBack8bitEncoding() == rhs.fallBack8bitEncoding() &&
           lhs.writeApi()             == rhs.writeApi()             &&
           lhs.timeZone()             == rhs.timeZone()             &&
           lhs.timeOffset()           == rhs.timeOffset()           &&
           lhs.articlePath()          == rhs.articlePath()          &&
           lhs.scriptPath()           == rhs.scriptPath()           &&
           lhs.script()               == rhs.script()               &&
           lhs.variantArticlePath()   == rhs.variantArticlePath()   &&
           lhs.serverUrl()            == rhs.serverUrl()            &&
           lhs.wikiId()               == rhs.wikiId()               &&
           lhs.time()                 == rhs.time();
}
