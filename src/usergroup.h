/** ===========================================================
 * @file
 *
 * This file is a part of KDE project
 * <a href="https://commits.kde.org/libmediawiki">libmediawiki</a>
 *
 * @date   2011-03-22
 * @brief  a MediaWiki C++ interface for KDE
 *
 * @author Copyright (C) 2011-2012 by Gilles Caulier
 *         <a href="mailto:caulier dot gilles at gmail dot com">caulier dot gilles at gmail dot com</a>
 * @author Copyright (C) 2010 by Remi Benoit
 *         <a href="mailto:r3m1 dot benoit at gmail dot com">r3m1 dot benoit at gmail dot com</a>
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

#ifndef USERGROUP_H
#define USERGROUP_H

// Qt includes

#include <QString>
#include <QList>

// Local includes

#include "mediawiki_export.h"

namespace mediawiki
{

/**
 * @brief A user group.
 */
class MEDIAWIKI_EXPORT UserGroup
{

public:

    /**
     * Constructs a user group.
     */
    UserGroup();

    /**
     * @brief Constructs a user group from an other user group.
     * @param other an other user group
     */
    UserGroup(const UserGroup& other);

    /**
     * @brief Destructs a user group.
     */
    ~UserGroup();

    /**
     * @brief Assingning a user group from an other user group.
     * @param other an other user group
     */
    UserGroup& operator=(UserGroup other);

    /**
     * @brief Returns true if this instance and other are equal, else false.
     * @param other instance to compare
     * @return true if there are equal, else false
     */
    bool operator==(const UserGroup& other) const;

    /**
     * @brief Returns the name of the user group.
     * @return the name of the user group
     */
    QString name() const;

    /**
     * @brief Set the name of the user group.
     * @param name the name of the user group
     */
    void setName(const QString& name);

    /**
     * @brief Returns rights of the user group.
     * @return rights of the user group
     */
    const QList<QString>& rights() const;

    /**
     * @brief Returns rights of the user group.
     * @return rights of the user group
     */
    QList<QString>& rights();

    /**
     * @brief Set rights of the user group.
     * @param rights rights of the user group
     */
     void setRights(const QList<QString>& rights);

    /**
     * @brief Returns the numbers of users in the user group.
     * @return the numbers of users in the user group
     */
    qint64 number() const;

    /**
     * @brief Set the number of users in the user group.
     * @param number the number of users in the user group
     */
    void setNumber(qint64 number) ;

private:

    class UserGroupPrivate;
    UserGroupPrivate* const d;
};

} // namespace mediawiki

#endif // USERGROUP_H
