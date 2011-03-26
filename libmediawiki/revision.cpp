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
 * @author Copyright (C) 2010 by Robin Bussenot
 *         <a href="mailto:bussenot dot robin at gmail dot com">bussenot dot robin at gmail dot com</a>
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

// Local includes.

#include "revision.h"

namespace mediawiki
{

class RevisionPrivate
{
public:

    int       revId;
    int       parentId;
    int       size;
    bool      minorRevision;
    QString   user;
    QDateTime timestamp;
    QString   comment;
    QString   content;
    QString   parseTree;
    QString   rollback;
};

Revision::Revision()
    : d(new RevisionPrivate())
{
    d->minorRevision = false;
    d->revId         = -1;
    d->parentId      = -1;
    d->size          = -1;
}

Revision::~Revision()
{
    delete d;
}

Revision::Revision( const Revision& other)
        : d(new RevisionPrivate(*(other.d)))
{
}

Revision & Revision::operator=(Revision other)
{
    std::swap(d, other.d);
    return *this;
}

void Revision::setRevisionId(int revisionId)
{
    d->revId=revisionId;
}

int Revision::revisionId() const
{
    return d->revId;
}

void Revision::setParentId(int parentId)
{
    d->parentId=parentId;
}

int Revision::parentId() const
{
    return d->parentId;
}

void Revision::setSize(int size)
{
    d->size=size;
}

int Revision::size() const
{
    return d->size;
}

void Revision::setMinorRevision(bool minorRevision)
{
    d->minorRevision=minorRevision;
}
bool Revision::minorRevision() const
{
    return d->minorRevision;
}

QDateTime Revision::timestamp() const
{
    return d->timestamp;
}

void Revision::setTimestamp(const QDateTime& timestamp)
{
    d->timestamp = timestamp;
}

QString Revision::user() const
{
    return d->user;
}

void Revision::setUser(const QString& user)
{
    d->user = user;
}

void Revision::setComment(const QString& com)
{
    d->comment = com;
}

QString Revision::comment() const
{
    return d->comment;
}

QString Revision::content() const
{
    return d->content;
}

void Revision::setContent(const QString& content)
{
    d->content=content;
}

void Revision::setParseTree(const QString& parseTree)
{
    d->parseTree=parseTree;
}

QString Revision::parseTree() const
{
    return d->parseTree;
}

void Revision::setRollback(const QString& rollback)
{
    d->parseTree=rollback;
}

QString Revision::rollback() const
{
    return d->rollback;
}

} // namespace mediawiki

bool operator==(const mediawiki::Revision& lhs, const mediawiki::Revision& rhs)
{
    return lhs.timestamp()     == rhs.timestamp()     &&
           lhs.user()          == rhs.user()          &&
           lhs.comment()       == rhs.comment()       &&
           lhs.content()       == rhs.content()       &&
           lhs.size()          == rhs.size()          &&
           lhs.minorRevision() == rhs.minorRevision() &&
           lhs.parseTree()     == rhs.parseTree()     &&
           lhs.parentId()      == rhs.parentId()      &&
           lhs.rollback()      == rhs.rollback()      &&
           lhs.revisionId()    == rhs.revisionId();
}
