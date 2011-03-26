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
 * @author Copyright (C) 2010 by Joris Munoz
 *         <a href="mailto:munozjoris at gmail dot com">munozjoris at gmail dot com</a>
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

// Qt includes

#include <QtCore/QUrl>

// Local includes

#include "page.h"

namespace mediawiki
{

class PagePrivate
{
public:

    unsigned int m_pageid;
    unsigned int m_ns;
    unsigned int m_lastrevid;
    unsigned int m_counter;
    unsigned int m_length;
    unsigned int m_talkid;
    QString      m_edittoken;
    QString      m_title;
    QString      m_readable;
    QString      m_preload;
    QUrl         m_fullurl;
    QUrl         m_editurl;
    QDateTime    m_touched;
    QDateTime    m_starttimestamp;
};

Page::Page()
    :d(new PagePrivate())
{
}

Page::~Page()
{
    delete d;
}

Page::Page( const Page& other)
        : d(new PagePrivate(*(other.d)))
{
}

Page& Page::operator=(Page other)
{
    std::swap(d, other.d);
    return *this;
}

void Page::setPageId(unsigned int id)
{
    d->m_pageid=id;
}

unsigned int Page::pageId() const
{
    return d->m_pageid;
}

void Page::setTitle(const QString& title)
{
    d->m_title=title;
}

QString Page::pageTitle() const
{
    return d->m_title;
}

void Page::setNs(unsigned int ns) const
{
    d->m_ns=ns;
}

unsigned int Page::pageNs() const
{
    return d->m_ns;
}

void Page::setLastRevId(unsigned int lastRevId) const
{
    d->m_lastrevid=lastRevId;
}

unsigned int Page::pageLastRevId() const
{
    return d->m_lastrevid;
}

void Page::setCounter(unsigned int counter) const
{
    d->m_counter=counter;
}

unsigned int Page::pageCounter() const
{
    return d->m_counter;
}

void Page::setLength(unsigned int length) const
{
     d->m_length=length;
}

unsigned int Page::pageLength() const
{
     return d->m_length;
}

void Page::setEditToken(const QString& editToken)
{
    d->m_edittoken=editToken;
}

QString Page::pageEditToken() const
{
    return d->m_edittoken;
}

void Page::setTalkid(unsigned int talkid) const
{
     d->m_talkid=talkid;
}

unsigned int Page::pageTalkid() const
{
     return d->m_talkid;
}

void Page::setFullurl(const QUrl& fullurl)
{
    d->m_fullurl=fullurl;
}

QUrl Page::pageFullurl() const
{
    return d->m_fullurl;
}

void Page::setEditurl(const QUrl& editurl)
{
    d->m_editurl=editurl;
}

QUrl Page::pageEditurl() const
{
    return d->m_editurl;
}

void Page::setReadable(const QString& readable)
{
    d->m_readable=readable;
}

QString Page::pageReadable() const
{
    return d->m_readable;
}

void Page::setPreload(const QString& preload)
{
    d->m_preload=preload;
}

QString Page::pagePreload() const
{
    return d->m_preload;
}

void Page::setTouched(const QDateTime& touched)
{
    d->m_touched=touched;
}

QDateTime Page::pageTouched() const
{
    return d->m_touched;
}

void Page::setStarttimestamp(const QDateTime& starttimestamp)
{
    d->m_starttimestamp=starttimestamp;
}

QDateTime Page::pageStarttimestamp() const
{
    return d->m_starttimestamp;
}

} // namespace mediawiki;

bool operator==(const mediawiki::Page& lhs,const mediawiki::Page& rhs)
{
    return lhs.pageId()             == rhs.pageId()        &&
           lhs.pageTitle()          == rhs.pageTitle()     &&
           lhs.pageNs()             == rhs.pageNs()        &&
           lhs.pageLastRevId()      == rhs.pageLastRevId() &&
           lhs.pageCounter()        == rhs.pageCounter()   &&
           lhs.pageLength()         == rhs.pageLength()    &&
           lhs.pageEditToken()      == rhs.pageEditToken() &&
           lhs.pageTalkid()         == rhs.pageTalkid()    &&
           lhs.pageFullurl()        == rhs.pageFullurl()   &&
           lhs.pageEditurl()        == rhs.pageEditurl()   &&
           lhs.pageReadable()       == rhs.pageReadable()  &&
           lhs.pagePreload()        == rhs.pagePreload()   &&
           lhs.pageTouched()        == rhs.pageTouched()   &&
           lhs.pageStarttimestamp() == rhs.pageStarttimestamp();
}
