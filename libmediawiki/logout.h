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

#ifndef MEDIAWIKI_LOGOUT_H
#define MEDIAWIKI_LOGOUT_H

// Local includes

#include "job.h"
#include "mediawiki_export.h"

namespace mediawiki
{

class MediaWiki;
class LogoutPrivate;

/**
 * @brief Logout job.
 *
 * Uses for log out a user.
 */
class MEDIAWIKI_EXPORT Logout : public Job
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Logout)

public:

    /**
     * @brief Constructs a Logout job.
     * @param parent the QObject parent
     */
    explicit Logout(MediaWiki& mediawiki, QObject* const parent = 0);

    /**
     * @brief Destroys the Logout job.
     */
    virtual ~Logout();

    /**
     * @brief Starts the job asynchronously.
     */
    virtual void start();

private Q_SLOTS:

    void doWorkSendRequest();
    void doWorkProcessReply();
};

} // namespace mediawiki

#endif // LOGOUT_H
