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
 * @author Copyright (C) 2011 by Alexandre Mendes
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

#ifndef MEDIAWIKI_UPLOAD_H
#define MEDIAWIKI_UPLOAD_H

// Qt includes

#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtNetwork/QNetworkCookieJar>
#include <QtCore/QUrl>

// Local includes

#include "mediawiki_export.h"
#include "job.h"
#include "queryinfo.h"

namespace mediawiki
{

class MediaWiki;
class UploadPrivate;

/**
 * @brief Upload job.
 *
 * Uses for upload files.
 */
class MEDIAWIKI_EXPORT Upload : public Job
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Upload)

public:

    enum
    {
        /**
         * @brief An internal error occurred.
         */
        InternalError= Job::UserDefinedError+1,
        /**
         * @brief The module is disabled.
         */
        UploadDisabled,

        /**
         * @brief The session key is invalid.
         */
        InvalidSessionKey,

        /**
         * @brief The current user can't upload.
         */
        BadAccess,

        /**
         * @brief A param is missing.
         */
        ParamMissing,

        /**
         * @brief No upload without logged in.
         */
        MustBeLoggedIn,

        /**
         * @brief
         */
        FetchFileError,

        /**
         * @brief No upload module set.
         */
        NoModule,

        /**
         * @brief The file submitted was empty.
         */
        EmptyFile,

        /**
         * @brief The file is missing an extension.
         */
        ExtensionMissing,

        /**
         * @brief The filename is too short.
         */
        TooShortFilename,

        /**
         * @brief Overwriting an existing file is not allowed.
         */
        OverWriting,

        /**
         * @brief Stashing temporary file failed.
         */
        StashFailed
    };

public:

    /**
     * @brief Constructs an Upload job.
     * @param parent the QObject parent
     */
    Upload(MediaWiki& mediawiki, QObject* const parent = 0);

    /**
     * @brief Destroys the Upload job.
     */
    virtual ~Upload();

    /**
     * @brief Starts the job asynchronously.
     */
    virtual void start();

    /**
     * @brief Set the target filename.
     * @param filename the filename
     */
    void setFilename(const QString&);

    /**
     * @brief Set the file.
     * @param file the file
     */
    void setFile(QIODevice* const);

    /**
     * @brief Set the upload comment. Also used as the initial page text for new files if text parameter not provided.
     * @param comment the comment
     */
    void setComment(const QString&);

    /**
     * @brief Set the information of the image. Use this template {{Information|Description=|Source=|Date=|Author=|Permission=|other_versions=}}.
     * @param text the text
     */
    void setText(const QString&);

private Q_SLOTS:

    void doWorkSendRequest(Page page);
    void doWorkProcessReply();
};

} // namespace mediawiki

#endif // UPLOAD_H
