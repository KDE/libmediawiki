/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2011-03-22
 * @brief  a MediaWiki C++ interface for KDE
 *
 * @author Copyright (C) 2011 by Gilles Caulier
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

#include "protection.h"
#include <algorithm>

namespace mediawiki
{

class ProtectionInfo
{
public:

    QString type;
    QString level;
    QString expiry;
    QString source;
};

Protection::Protection()
    : d(new ProtectionInfo())
{
    d->type   = "";
    d->level  = "";
    d->expiry = "";
    d->source = "";
}

Protection::~Protection()
{
    delete d;
}

Protection::Protection(const Protection& other)
    : d(new ProtectionInfo(*(other.d)))
{
}

Protection& Protection::operator=(Protection other)
{
    std::swap(d, other.d);
    return *this;
}

void Protection::setType(const QString& type)
{
    d->type = type;
}

QString Protection::type() const
{
    return d->type;
}

void Protection::setLevel(const QString& level)
{
    d->level = level;
}

QString Protection::level() const
{
    return d->level;
}

void Protection::setExpiry(const QString& expiry)
{
    d->expiry = expiry;
}

QString Protection::expiry() const
{
    return d->expiry;
}

void Protection::setSource(const QString& source)
{
    d->source = source;
}

QString Protection::source() const
{
    return d->source;
}

} // namespace mediawiki

bool operator==(const mediawiki::Protection& ptc, const mediawiki::Protection& ptc2)
{
    return ptc.type()   == ptc2.type()   &&
           ptc.level()  == ptc2.level()  &&
           ptc.expiry() == ptc2.expiry() &&
           ptc.source() == ptc2.source();
}
