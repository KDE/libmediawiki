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

#ifndef MEDIAWIKI_MEDIAWIKI_H
#define MEDIAWIKI_MEDIAWIKI_H

// Qt includes

#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkCookieJar>

// Local includes

#include "mediawiki_export.h"

namespace mediawiki
{

/**
 * @brief Provides access to wiki powered by MediaWiki.
 */
class MEDIAWIKI_EXPORT MediaWiki
{

public:

    /**
     * @brief Constructs a MediaWiki by its url api.
     * @param url the url api of the wiki
     * @param customUserAgent you can specify the user agent to use
                              which will be concatenated with the postfix user agent
     *                        else the postfix user agent is used only
     */
    explicit MediaWiki(const QUrl& url, const QString& customUserAgent = QString());

    /**
     * @brief Destructs the MediaWiki.
     */
    ~MediaWiki();

    /**
     * @brief Returns the url api of the wiki.
     * @returns the url api of the wiki
     */
    QUrl url() const;

    /**
     * @brief Returns the user agent of the wiki.
     * @return the user agent of the wiki
     */
    QString userAgent() const;

    /**
     * @brief Returns the network manager instance of the wiki.
     * @return the network manager instance of the wiki
     */
    QNetworkAccessManager* manager() const;

private:

    class MediaWikiPrivate;
    MediaWikiPrivate* const d_ptr;

    friend class JobPrivate;
};

} // namespace mediawiki

#endif // MEDIAWIKI_MEDIAWIKI_H
