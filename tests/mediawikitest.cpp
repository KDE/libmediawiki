/** ===========================================================
 * @file
 *
 * This file is a part of KDE project
 * <a href="https://commits.kde.org/libmediawiki">libmediawiki</a>
 *
 * @date   2011-03-22
 * @brief  a MediaWiki C++ interface for KDE
 *
 * @author Copyright (C) 2010 by Alexandre Mendes
 *         <a href="mailto:alex dot mendes1988 at gmail dot com">alex dot mendes1988 at gmail dot com</a>
 * @author Copyright (C) 2010 by Ludovic Delfau
 *         <a href="mailto:ludovicdelfau at gmail dot com">ludovicdelfau at gmail dot com</a>
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

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtTest/QtTest>

#include "mediawiki.h"

using mediawiki::MediaWiki;

class MediaWikiTest : public QObject
{

    Q_OBJECT

private Q_SLOTS:

    void testConstructor() {
        QFETCH(QUrl, url);
        QFETCH(QString, customUserAgent);
        QFETCH(QString, userAgent);

        MediaWiki mediawiki(url, customUserAgent);

        QCOMPARE(mediawiki.url(), url);
        QCOMPARE(mediawiki.userAgent(), userAgent);
    }

    void testConstructor_data() {
        QTest::addColumn<QUrl>("url");
        QTest::addColumn<QString>("customUserAgent");
        QTest::addColumn<QString>("userAgent");

        QTest::newRow("") << QUrl(QStringLiteral("http://127.0.0.1:12566")) << QString() << QStringLiteral("mediawiki-silk");
        QTest::newRow("") << QUrl(QStringLiteral("commons.wikimedia.org/w/api.php")) << QString() << QStringLiteral("mediawiki-silk");
        QTest::newRow("") << QUrl(QStringLiteral("http://commons.wikimedia.org/w/api.php")) << QStringLiteral("test1") << QStringLiteral("test1-mediawiki-silk");
        QTest::newRow("") << QUrl(QStringLiteral("http://commons.wikimedia.org/w/api.php/")) << QStringLiteral("test2") << QStringLiteral("test2-mediawiki-silk");
    }

};

QTEST_MAIN(MediaWikiTest)

#include "mediawikitest.moc"
