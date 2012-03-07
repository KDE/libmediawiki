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
 * @author Copyright (C) 2009 by Richard Moore
 *         <a href="mailto:rich at kde dot org">rich at kde dot org</a>
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

#include "mediawiki.h"
#include "mediawiki_p.h"

namespace mediawiki
{

MediaWiki::MediaWiki(const QUrl& url, const QString& customUserAgent)
    : d_ptr(new MediaWikiPrivate(url,
                                 (customUserAgent.isEmpty() ? QString() 
                                                            : (customUserAgent + '-')) + MediaWikiPrivate::POSTFIX_USER_AGENT,
                                 new QNetworkAccessManager()))
{
}

MediaWiki::~MediaWiki()
{
    delete d_ptr;
}

QUrl MediaWiki::url() const
{
    return d_ptr->url;
}

QString MediaWiki::userAgent() const
{
    return d_ptr->userAgent;
}

QNetworkAccessManager* MediaWiki::manager() const
{
    return d_ptr->manager;
}

} // namespace mediawiki
