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
 * @author Copyright (C) 2011 by Manuel Campomanes
 *         <a href="mailto:campomanes dot manuel at gmail dot com">campomanes dot manuel at gmail dot com</a>
 * @author Copyright (C) 2010 by Vincent Garcia
 *         <a href="mailto:xavier dot vincent dot garcia at gmail dot com">xavier dot vincent dot garcia at gmail dot com</a>
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

#ifndef PARSE_H
#define PARSE_H

// Qt includes

#include <QtCore/QString>
#include <QtCore/QLocale>

// Local includes

#include "job.h"
#include "mediawiki_export.h"

namespace mediawiki
{

class MediaWiki;
class ParsePrivate;

class MEDIAWIKI_EXPORT Parse : public Job
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Parse)

public:

    /**
     * @brief Indicates all possible error conditions found during the processing of the job.
     */
    enum
    {
         /**
         * @brief An internal error occurred.
         */
        InternalError= Job::UserDefinedError+1,

        /**
         * @brief The page parameter cannot be used together with the text and title parameters
         */
        TooManyParams,

        /**
         * @brief The page you specified doesn't exist
         */
        MissingPage
    };

    explicit Parse(MediaWiki& mediawiki, QObject* const parent = 0);
    virtual ~Parse();

    void setText(const QString& param);

    void setTitle(const QString& param);

    void setPageName(const QString& param);

    void setUseLang(const QString& param);

    virtual void start();

    Q_SIGNALS:

    void result(const QString& text);

private Q_SLOTS:

    void doWorkSendRequest();
    void doWorkProcessReply();
};

} // namespace mediawiki

#endif // PARSE_H
