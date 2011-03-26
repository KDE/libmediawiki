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

// C++ includes

#include <algorithm>

// Local includes

#include "image.h"

namespace mediawiki
{

class ImagePrivate
{
public:

    qint64  namespaceId;
    QString title;
};

Image::Image()
    : d(new ImagePrivate())
{
    d->namespaceId = -1;
}

Image::Image(const Image& other)
    : d(new ImagePrivate(*(other.d)))
{}

Image::~Image()
{
    delete d;
}

Image& Image::operator=(Image other)
{
    std::swap(d, other.d);
    return *this;
}

qint64 Image::namespaceId() const
{
    return d->namespaceId;
}

void Image::setNamespaceId(qint64 namespaceId)
{
    d->namespaceId = namespaceId;
}

QString Image::title() const
{
    return d->title;
}

void Image::setTitle(const QString& title)
{
    d->title = title;
}

} // namespace mediawiki

bool operator==(const mediawiki::Image& lhs, const mediawiki::Image& rhs)
{
    return lhs.namespaceId() == rhs.namespaceId() &&
           lhs.title()       == rhs.title();
}
