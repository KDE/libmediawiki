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
 * @author Copyright (C) 2010 by Ludovic Delfau
 *         <a href="mailto:ludovicdelfau at gmail dot com">ludovicdelfau at gmail dot com</a>
 * @author Copyright (C) 2010 by Paolo de Vathaire
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

#ifndef MEDIAWIKI_QUERYIMAGES_H
#define MEDIAWIKI_QUERYIMAGES_H

// KDE includes

#include <kjob.h>

// Local includes

#include "job.h"
#include "mediawiki_export.h"
#include "image.h"

namespace mediawiki
{

class MediaWiki;
class QueryImagesPrivate;

/**
 * @brief Query images job.
 *
 * Gets a list of all images used on pages.
 */
class MEDIAWIKI_EXPORT QueryImages : public Job
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QueryImages)

public:

    /**
     * @brief Indicates all possible error conditions found during the processing of the job.
     */
    enum
    {
        /**
         * @brief A network error has occurred.
         */
        NetworkError = KJob::UserDefinedError + 1,

        /**
         * @brief A XML error has occurred.
         */
        XmlError
    };

public:

    /**
     * @brief Constructs a query images job.
     * @param mediawiki the mediawiki concerned by the job
     * @param parent the QObject parent
     */
    explicit QueryImages(MediaWiki& mediawiki, QObject* const parent = 0);

    /**
     * @brief Destroys a query images job.
     */
    virtual ~QueryImages();

    /**
     * @brief Set the title.
     * @param title the title of the page
     */
    void setTitle(const QString& title);

    /**
     * @brief Set the limit.
     * @param limit number of images returned by one signal #pages()
     */
    void setLimit(unsigned int limit);

    /**
     * @brief Starts the job asynchronously.
     */
    virtual void start();

Q_SIGNALS:

    /**
     * @brief Provides a list of all images used on pages
     *
     * This signal can be emitted several times.
     *
     * @param pages list of all images used on pages
     */
    void images(const QList<Image> & images);

private Q_SLOTS:

    void doWorkSendRequest();
    void doWorkProcessReply();
};

} // namespace mediawiki

#endif // MEDIAWIKI_QUERYIMAGES_H
