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
 * @author Copyright (C) 2009 by Paolo de Vathaire
 *         <a href="mailto:paolo dot devathaire at gmail dot com">paolo dot devathaire at gmail dot com</a>
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

#ifndef MEDIAWIKIJOB_H
#define MEDIAWIKIJOB_H

// KDE includes

#include <kjob.h>

// Local includes

#include "mediawiki_export.h"

namespace mediawiki
{

class MediaWiki;
class JobPrivate;

/**
 * @brief The base class for all MediaWiki jobs.
 */
class MEDIAWIKI_EXPORT Job : public KJob
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Job)

public:

    /**
     * @brief Indicates all possible error conditions found during the processing of the job.
     */
    enum
    {
        NetworkError            = KJob::UserDefinedError + 1,
        XmlError,
        UserRequestDefinedError = KJob::UserDefinedError + 100
    };

public:

    /**
     * @brief Destructs the Job.
     */
    virtual ~Job();

    /**
     * @brief Aborts this job quietly.
     */
    virtual bool doKill();

protected:

    /**
     * @brief Constructs a Job by a private class.
     * @param dd a private class
     * @param parent the QObject parent
     */
    Job(JobPrivate& dd, QObject* const parent = 0);

    //TODO comment
    void connectReply();

    /**
     * @brief The private d pointer.
     */
    JobPrivate* const d_ptr;

private Q_SLOTS:

    void processUploadProgress(qint64 bytesReceived, qint64 bytesTotal);
};

} // namespace mediawiki

#endif // MEDIAWIKIJOB_H
