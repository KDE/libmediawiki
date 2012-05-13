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

#include "usergroup.h"

// C++ includes

#include <algorithm>

namespace mediawiki
{

class UserGroup::UserGroupPrivate
{
public:

    unsigned int   number;

    QString        name;

    QList<QString> rights;
};

UserGroup::UserGroup()
    : d(new UserGroupPrivate())
{
    d->number = -1;
}

UserGroup::UserGroup(const UserGroup& other)
    : d(new UserGroupPrivate(*(other.d)))
{
}

UserGroup::~UserGroup()
{
    delete d;
}

UserGroup& UserGroup::operator=(UserGroup other)
{
    *d = *other.d;
    return *this;
}

bool UserGroup::operator==(const mediawiki::UserGroup& other) const
{
    return number() == other.number() &&
           rights() == other.rights() &&
           name()   == other.name() ;
}

QString UserGroup::name() const
{
    return d->name;
}

void UserGroup::setName(const QString& name)
{
    d->name = name;
}

const QList<QString>& UserGroup::rights() const
{
    return d->rights;
}

QList<QString>& UserGroup::rights()
{
    return d->rights;
}

void UserGroup::setRights(const QList<QString>& rights)
{
    d->rights = rights;
}

qint64 UserGroup::number() const
{
    return d->number;
}

void UserGroup::setNumber(qint64 number)
{
    d->number = number;
}

} // namespace mediawiki
