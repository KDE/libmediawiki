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

#ifndef PROTECTION_H
#define PROTECTION_H

// Qt includes

#include <QtCore/QString>

// Local includes

#include "mediawiki_export.h"

namespace mediawiki
{

/**
 * @brief Protection info job.
 *
 * Represent protection parameters in a page.
 */
class MEDIAWIKI_EXPORT Protection
{

public:

    /**
     * @brief Constructs a protection.
     *
     * You can set parameters of the protection after.
     */
    Protection();

    /**
     * @brief Constructs an protection from an other protection.
     * @param other an other protection
     */
    Protection(const Protection& other);

    /**
     * @brief Destructs a protection.
     */
    ~Protection();

    /**
     * @brief Assingning an protection from an other protection.
     * @param other an other protection
     */
    Protection& operator=(Protection other);

    /**
     * @brief Returns true if this instance and other are equal, else false.
     * @param other instance to compare
     * @return true if there are equal, else false
     */
    bool operator==(const Protection& other) const;

    /**
     * @brief Set the protection type.
     * @param type the protection type
     */
    void setType(const QString& type);

    /**
     * @brief Get the protection type.
     * @return the protection type
     */
    QString type() const;

    /**
     * @brief Set the page protection level.
     * @param level the page protection level
     */
    void setLevel(const QString& level);

    /**
     * @brief Get the page protection level.
     * @return the page protection level
     */
    QString level() const;

    /**
     * @brief Set the expiry date.
     * @param expiry the expiry date
     */
    void setExpiry(const QString& expiry);

    /**
   ² * @brief Get the expiry date.
     * @return the expiry date
     */
    QString expiry() const;

    /**
     * @brief Set the source.
     * @param source the source
     */
    void setSource(const QString& source);

    /**
     * @brief Get the source.
     * @return the source
     */
    QString source() const;

private:

    class ProtectionPrivate;
    ProtectionPrivate* const d;
};

} // namespace mediawiki

#endif // PROTECTION_H
