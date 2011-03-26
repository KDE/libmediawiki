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

// C++ includes

#include <algorithm>

// Local includes

#include "imageinfo.h"

namespace mediawiki
{

class ImageinfoPrivate
{
public:

    QDateTime                timestamp;
    QString                  user;
    QString                  comment;
    QUrl                     url;
    QUrl                     descriptionUrl;
    QUrl                     thumbUrl;
    qint64                   thumbWidth;
    qint64                   thumbHeight;
    qint64                   size;
    qint64                   width;
    qint64                   height;
    QString                  sha1;
    QString                  mime;
    QHash<QString, QVariant> metadata;
};

Imageinfo::Imageinfo()
    : d(new ImageinfoPrivate())
{
    d->thumbWidth  = -1;
    d->thumbHeight = -1;
    d->size        = -1;
    d->width       = -1;
    d->height      = -1;
}

Imageinfo::Imageinfo(const Imageinfo& other)
    : d(new ImageinfoPrivate(*(other.d)))
{}

Imageinfo::~Imageinfo()
{
    delete d;
}

Imageinfo& Imageinfo::operator=(Imageinfo other)
{
    std::swap(d, other.d);
    return *this;
}

QDateTime Imageinfo::timestamp() const
{
    return d->timestamp;
}

void Imageinfo::setTimestamp(const QDateTime& timestamp)
{
    d->timestamp = timestamp;
}

QString Imageinfo::user() const
{
    return d->user;
}

void Imageinfo::setUser(const QString& user)
{
    d->user = user;
}

QString Imageinfo::comment() const
{
    return d->comment;
}

void Imageinfo::setComment(const QString& comment)
{
    d->comment = comment;
}

QUrl Imageinfo::url() const
{
    return d->url;
}

void Imageinfo::setUrl(const QUrl& url)
{
    d->url = url;
}

QUrl Imageinfo::descriptionUrl() const
{
    return d->url;
}

void Imageinfo::setDescriptionUrl(const QUrl& descriptionUrl)
{
    d->descriptionUrl = descriptionUrl;
}

QUrl Imageinfo::thumbUrl() const
{
    return d->thumbUrl;
}

void Imageinfo::setThumbUrl(const QUrl& thumbUrl)
{
    d->thumbUrl = thumbUrl;
}

qint64 Imageinfo::thumbWidth() const
{
    return d->thumbWidth;
}

void Imageinfo::setThumbWidth(qint64 thumbWidth)
{
    d->thumbWidth = thumbWidth;
}

qint64 Imageinfo::thumbHeight() const
{
    return d->thumbHeight;
}

void Imageinfo::setThumbHeight(qint64 thumbHeight)
{
    d->thumbHeight = thumbHeight;
}

qint64 Imageinfo::size() const
{
    return d->size;
}

void Imageinfo::setSize(qint64 size)
{
    d->size = size;
}

qint64 Imageinfo::width() const
{
    return d->width;
}

void Imageinfo::setWidth(qint64 width)
{
    d->width = width;
}

qint64 Imageinfo::height() const
{
    return d->height;
}

void Imageinfo::setHeight(qint64 height)
{
    d->height = height;
}

QString Imageinfo::sha1() const
{
    return d->sha1;
}

void Imageinfo::setSha1(const QString& sha1)
{
    d->sha1 = sha1;
}

QString Imageinfo::mime() const
{
    return d->mime;
}

void Imageinfo::setMime(const QString& mime)
{
    d->mime = mime;
}

const QHash<QString, QVariant>& Imageinfo::metadata() const
{
    return d->metadata;
}

QHash<QString, QVariant>& Imageinfo::metadata()
{
    return d->metadata;
}

void Imageinfo::setMetadata(const QHash<QString, QVariant>& metadata)
{
     d->metadata = metadata;
}

} // namespace mediawiki

bool operator==(const mediawiki::Imageinfo& lhs, const mediawiki::Imageinfo& rhs)
{
    return lhs.timestamp()      == rhs.timestamp()      &&
           lhs.user()           == rhs.user()           &&
           lhs.comment()        == rhs.comment()        &&
           lhs.url()            == rhs.url()            &&
           lhs.descriptionUrl() == rhs.descriptionUrl() &&
           lhs.thumbUrl()       == rhs.thumbUrl()       &&
           lhs.thumbWidth()     == rhs.thumbWidth()     &&
           lhs.thumbHeight()    == rhs.thumbHeight()    &&
           lhs.size()           == rhs.size()           &&
           lhs.width()          == rhs.width()          &&
           lhs.height()         == rhs.height()         &&
           lhs.sha1()           == rhs.sha1()           &&
           lhs.mime()           == rhs.mime()           &&
           lhs.metadata()       == rhs.metadata();
}
