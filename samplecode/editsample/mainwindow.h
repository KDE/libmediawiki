/** ===========================================================
 * @file
 *
 * This file is a part of KDE project
 * <a href="https://projects.kde.org/projects/extragear/libs/libmediawiki">libmediawiki</a>
 *
 * @date   2011-03-22
 * @brief  a MediaWiki C++ interface for KDE
 *
 * @author Copyright (C) 2011-2013 by Gilles Caulier
 *         <a href="mailto:caulier dot gilles at gmail dot com">caulier dot gilles at gmail dot com</a>
 * @author Copyright (C) 2010 by Alexandre Mendes
 *         <a href="mailto:alex dot mendes1988 at gmail dot com">alex dot mendes1988 at gmail dot com</a>
 * @author Copyright (C) 2011 by Guillaume Hormiere
 *         <a href="mailto:hormiere dot guillaume at gmail dot com">hormiere dot guillaume at gmail dot com</a>
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt includes

#include <QMainWindow>
#include <QMessageBox>

// Local includes

#include "edit.h"
#include "mediawiki.h"
#include "login.h"
#include "queryinfo.h"
#include "queryrevision.h"

namespace Ui
{
    class MainWindow;
}

using namespace mediawiki;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget* const parent = 0);
    ~MainWindow();

private Q_SLOTS:

    void on_plainTextEdit_textChanged();
    void on_mPageEdit_textChanged(QString);
    void on_mWikiEdit_textChanged(QString);
    void on_pushButton1_clicked();
    void on_pushButton2_clicked();
    void loginHandle(KJob* login);
    void revisionHandle(const QList<Revision>& revisions);
    void editError(KJob* job);
    void revisionError(KJob* job);

private:

    Ui::MainWindow* ui;
    MediaWiki*      mediawiki;
};

#endif // MAINWINDOW_H
