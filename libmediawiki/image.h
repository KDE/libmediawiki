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

#ifndef MEDIAWIKI_IMAGE_H
#define MEDIAWIKI_IMAGE_H

// Qt includes

#include <QtCore/QString>

// Local includes

#include "mediawiki_export.h"

namespace mediawiki
{

/**
 * @brief A image.
 */
class MEDIAWIKI_EXPORT Image
{

public:

    /**
     * @brief Constructs a image.
     */
    Image();

    /**
     * @brief Constructs an image from an other image.
     * @param other an other image
     */
    Image(const Image& other);

    /**
     * @brief Destructs an image.
     */
    ~Image();

    /**
     * @brief Assingning an image from an other image.
     * @param other an other image
     */
    Image& operator=(Image other);

    /**
     * @brief Returns true if this instance and other are equal, else false.
     * @param other instance to compare
     * @return true if there are equal, else false
     */
    bool operator==(const Image& other) const;

    /**
     * @brief Returns the namespace id of the image.
     * @return the namespace id of the image
     */
    qint64 namespaceId() const;

    /**
     * @brief Set the namespace id.
     * @param namespaceId the namespace id of the image
     */
    void setNamespaceId(qint64 namespaceId);

    /**
     * @brief Returns the title of the image.
     * @return the title of the image
     */
    QString title() const;

    /**
     * @brief Set the title.
     * @param title the title of the image
     */
    void setTitle(const QString& title);

private:

    class ImagePrivate;
    ImagePrivate* const d;
};

} // namespace mediawiki

#endif // MEDIAWIKI_IMAGE_H
