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

#ifndef QUERYINFO_H
#define QUERYINFO_H

// Qt includes

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QDateTime>

// Local includes

#include "page.h"
#include "protection.h"
#include "mediawiki_export.h"
#include "job.h"

namespace mediawiki
{

class MediaWiki;
class QueryInfoPrivate;

/**
 * @brief QueryInfo job.
 *
 * Uses to send a request to get basic page information.
 */
class MEDIAWIKI_EXPORT QueryInfo : public Job
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QueryInfo)

public:

    /**
     * @brief Constructs a QueryInfo job.
     * @param mediawiki the mediawiki concerned by the job
     * @param parent the QObject parent
     */
    explicit QueryInfo(MediaWiki& mediawiki, QObject* const parent = 0);

    /**
     * @brief Destroys the QuerySiteInfoGeneral job.
     */
    virtual ~QueryInfo();

    /**
     * @brief Starts the job asynchronously.
     */
    virtual void start();

    /**
     * @brief Set the page name.
     * @param title the page name
     */
    void setPageName(const QString& title);

    /**
     * @brief Set the token to perform a data-modifying action on a page
     * @param token the token
     */
    void setToken(const QString& token);

    /**
     * @brief Set the page id.
     * @param id the page id
     */
    void setPageId(unsigned int id);

    /**
     * @brief Set the page revision
     * @param id the page revision
     */
    void setRevisionId(unsigned int id);

Q_SIGNALS:

    /**
     * @brief Provides a page
     * @param
     */
    void page(const Page& p);
    void protection(const QVector<Protection>& protect);

private Q_SLOTS:

    /**
     * @brief Send a request.
     */
    void doWorkSendRequest();

    void doWorkProcessReply();
};

} // namespace mediawiki

#endif // QUERYINFO_H
