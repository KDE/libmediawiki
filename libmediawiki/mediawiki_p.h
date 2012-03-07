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
 * @author Copyright (C) 2009 by Remi Benoit
 *         <a href="mailto:r3m1 dot benoit at gmail dot com">r3m1 dot benoit at gmail dot com</a>
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

#ifndef MEDIAWIKI_P_H
#define MEDIAWIKI_P_H

// Qt includes

#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkAccessManager>

namespace mediawiki
{

class MediaWiki::MediaWikiPrivate
{

public:

    MediaWikiPrivate(const QUrl& url, const QString& userAgent, QNetworkAccessManager* const manager)
        : url(url),
          userAgent(userAgent),
          manager(manager)
    {
    }

    ~MediaWikiPrivate()
    {
        delete manager;
    }

public:

    static const QString         POSTFIX_USER_AGENT;

    const QUrl                   url;
    const QString                userAgent;
    QNetworkAccessManager* const manager;
};

const QString MediaWiki::MediaWikiPrivate::POSTFIX_USER_AGENT("mediawiki-silk");

} // namespace mediawiki

#endif // MEDIAWIKI_P_H
