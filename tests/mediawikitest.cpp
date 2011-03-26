/*
 *   Copyright 2010 by Guillaume Hormiere <hormiere.guillaume@gmail.com>
 *   Copyright 2010 by Ludovic Delfau <ludovicdelfau@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtTest/QtTest>

#include "mediawiki.h"

using mediawiki::MediaWiki;

class MediaWikiTest : public QObject
{

    Q_OBJECT

private slots:

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

        QTest::newRow("") << QUrl("http://127.0.0.1:12566") << QString() << "mediawiki-silk";
        QTest::newRow("") << QUrl("commons.wikimedia.org/w/api.php") << "" << "mediawiki-silk";
        QTest::newRow("") << QUrl("http://commons.wikimedia.org/w/api.php") << "test1" << "test1-mediawiki-silk";
        QTest::newRow("") << QUrl("http://commons.wikimedia.org/w/api.php/") << "test2" << "test2-mediawiki-silk";
    }

};

QTEST_MAIN(MediaWikiTest);

#include "mediawikitest.moc"
