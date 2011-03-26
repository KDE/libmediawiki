/*
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
#include <QtTest/QTest>

#include "libmediawikitest/fakeserver.h"

#include "mediawiki.h"
#include "queryimages.h"
#include "image.h"

using mediawiki::MediaWiki;
using mediawiki::QueryImages;
using mediawiki::Image;

Q_DECLARE_METATYPE(QList<QString>);
Q_DECLARE_METATYPE(QList<Image>);
Q_DECLARE_METATYPE(QList<QList<Image> >);


class QueryImagesTest : public QObject {

    Q_OBJECT

public slots:

    void imagesHandle(const QList<Image> & images) {
        imagesReceivedList.push_back(images);
    }

private slots:

    void init() {
        imagesReceivedList.clear();
    }

    void testConstructor() {
        QFETCH(QList<QString>, scenarios);
        QFETCH(QString, title);
        QFETCH(unsigned int, limit);
        QFETCH(QList<QList<Image> >, imagesExpectedList);

        // Constructs the fakeserver
        FakeServer fakeserver;
        for (int i = 0; i < scenarios.size(); ++i) {
            if (i == 0) {
                fakeserver.setScenario(scenarios[i]);
            } else {
                fakeserver.addScenario(scenarios[i]);
            }
        }
        fakeserver.startAndWait();

        // Prepare the job
        MediaWiki mediawiki(QUrl("http://127.0.0.1:12566"));
        QueryImages * job = new QueryImages(mediawiki);
        job->setTitle(title);
        job->setLimit(limit);
        connect(job, SIGNAL(images(const QList<Image> &)), this, SLOT(imagesHandle(const QList<Image> &)));
        job->exec();

        // Test job
        QCOMPARE(job->error(), int(KJob::NoError));

        // Test requests sent
        const QList<FakeServer::Request> requests = fakeserver.getRequest();
        QCOMPARE(requests.size(), imagesExpectedList.size());
        for (int i = 0; i < requests.size(); ++i) {
            QCOMPARE(requests[i].agent, mediawiki.userAgent());
            QCOMPARE(requests[i].type, QString("GET"));
            if (i == 0) {
                QCOMPARE(requests[i].value, QString("?format=xml&action=query&titles=") + title + QString("&prop=images&imlimit=") + QString::number(limit));
            } else {
                QCOMPARE(requests[i].value, QString("?format=xml&action=query&titles=") + title + QString("&prop=images&imlimit=") + QString::number(limit) + QString("&imcontinue=1234%7C") + imagesExpectedList[i][0].title().remove(0, 5));
            }
        }

        // Test pages received
        QCOMPARE(imagesReceivedList, imagesExpectedList);

        // Test fakeserver
        QVERIFY(fakeserver.isAllScenarioDone());
    }

    void testConstructor_data() {
        QTest::addColumn<QList<QString> >("scenarios");
        QTest::addColumn<QString>("title");
        QTest::addColumn<unsigned int>("limit");
        QTest::addColumn<QList<QList<Image> > >("imagesExpectedList");
        Image image, image2, image3;

        QTest::newRow("Page with no image")
                << (QList<QString>()
                        << "<?xml version=\"1.0\"?><api><query><pages><page pageid=\"736\" ns=\"1\" title=\"Title-1\"></page></pages></query></api>")
                << "Title-1"
                << 10u
                << (QList<QList<Image> >() << QList<Image>());

        image.setNamespaceId(46u);
        image.setTitle("File:Image-1-1");
        QTest::newRow("Page with one image")
                << (QList<QString>()
                        << "<?xml version=\"1.0\"?><api><query><pages><page pageid=\"736\" ns=\"1\" title=\"Title-1\"><images><im ns=\"46\" title=\"File:Image-1-1\" /></images></page></pages></query></api>")
                << "Title-1"
                << 10u
                << (QList<QList<Image> >() << (QList<Image>() << image));

        image2.setNamespaceId(9997u);
        image2.setTitle("File:Image-1-2");
        QTest::newRow("Page with two images")
                << (QList<QString>()
                        << "<?xml version=\"1.0\"?><api><query><pages><page pageid=\"736\" ns=\"1\" title=\"Title-1\"><images><im ns=\"46\" title=\"File:Image-1-1\" /><im ns=\"9997\" title=\"File:Image-1-2\" /></images></page></pages></query></api>")
                << "Title-1"
                << 10u
                << (QList<QList<Image> >() << (QList<Image>() << image << image2));

        image.setNamespaceId(8u);
        image.setTitle("File:Image-2-1");

        image2.setNamespaceId(8998u);
        image2.setTitle("File:Image-2-2");

        image3.setNamespaceId(38423283u);
        image3.setTitle("File:Image-2-3");

        QTest::newRow("Page with three images by two signals")
                << (QList<QString>()
                        << "<?xml version=\"1.0\"?><api><query><pages><page pageid=\"1234\" ns=\"5757\" title=\"Title-2\"><images><im ns=\"8\" title=\"File:Image-2-1\" /><im ns=\"8998\" title=\"File:Image-2-2\" /></images></page></pages></query><query-continue><images imcontinue=\"1234|Image-2-3\" /></query-continue></api>"
                        << "<?xml version=\"1.0\"?><api><query><pages><page pageid=\"1234\" ns=\"5757\" title=\"Title-2\"><images><im ns=\"38423283\" title=\"File:Image-2-3\" /></images></page></pages></query></api>")
                << "Title-2"
                << 2u
                << (QList<QList<Image> >()
                        << (QList<Image>() << image << image2)
                        << (QList<Image>() << image3));


    }

private:

    QList<QList<Image> >imagesReceivedList;

};

QTEST_MAIN(QueryImagesTest);

#include "queryimagestest.moc"
