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

#include "job_p.h"
#include "job.moc"

// Qt includes

#include <QtNetwork/QNetworkReply>

// Local include

#include "mediawiki.h"

namespace mediawiki
{

Job::Job(JobPrivate& dd, QObject* const parent)
    : KJob(parent),
      d_ptr(&dd)
{
    setCapabilities(Job::Killable);
}

Job::~Job()
{
    delete d_ptr;
}

bool Job::doKill()
{
    Q_D(Job);
    if (d->reply != 0)
    {
        d->reply->abort();
    }
    return true;
}

void Job::connectReply()
{
    Q_D(Job);
    connect(d->reply, SIGNAL(uploadProgress(qint64,qint64)),
            this, SLOT(processUploadProgress(qint64,qint64)));
}

void Job::processUploadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    setTotalAmount(Job::Bytes, bytesTotal);
    setProcessedAmount(Job::Bytes, bytesReceived);
}

} // namespace mediawiki
