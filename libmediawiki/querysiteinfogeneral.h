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
 * @author Copyright (C) 2010 by Manuel Campomanes
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

#ifndef QUERYSITEINFOGENERAL_H
#define QUERYSITEINFOGENERAL_H

// Qt includes

#include <QtCore/QList>
#include <QtCore/QString>

// Local includes

#include "mediawiki_export.h"
#include "job.h"
#include "generalinfo.h"

namespace mediawiki
{

class MediaWiki;
class QuerySiteInfoGeneralPrivate;

/**
 * @brief QuerySiteInfoGeneral job.
 *
 * Uses for fetch a generals information about the wiki.
 */
class MEDIAWIKI_EXPORT QuerySiteInfoGeneral : public Job
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QuerySiteInfoGeneral)

public:

    enum
    {
        IncludeAllDenied = Job::UserDefinedError + 1
    };

public:

    /**
     * @brief Constructs a QuerySiteInfoGeneral job.
     * @param mediawiki the mediawiki concerned by the job
     * @param parent the QObject parent
     */
    explicit QuerySiteInfoGeneral(MediaWiki& mediawiki, QObject* const parent = 0);

    /**
     * @brief Destroys the QuerySiteInfoGeneral job.
     */
    virtual ~QuerySiteInfoGeneral();

    /**
     * @brief Starts the job asynchronously.
     */
    virtual void start();

Q_SIGNALS:

    /**
     * @brief Provide general info.
     * @param generalinfo the general info
     */
    void result(const Generalinfo& generalinfo);

private Q_SLOTS:

    void doWorkSendRequest();
    void doWorkProcessReply();
};

} // namespace mediawiki

#endif // QUERYSITEINFOGENERAL_H
