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

#ifndef MEDIAWIKIJOB_P_H
#define MEDIAWIKIJOB_P_H

#include "mediawiki.h"
#include "mediawiki_p.h"

namespace mediawiki
{

class JobPrivate
{
public:

    explicit JobPrivate(MediaWiki& mediawiki)
        : mediawiki(mediawiki),
          manager(mediawiki.d_ptr->manager)
    {
    }

    MediaWiki&                   mediawiki;
    QNetworkAccessManager* const manager;
    QNetworkReply*               reply;
};

} // namespace mediawiki

#endif // MEDIAWIKIJOB_P_H
