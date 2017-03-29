/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    MainWindow.h
 * @brief
 *******************************************************************************
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "DownloadControl.h"
#include "DownloadItem.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool init();
    void search();
    void updateListView();

public slots:
    void start(int index);
    void stop(int index);
    void removeDown(int index);

    void progressChange(int index, qint64 val, qint64 total);
    void downloadFinished(int index);
private slots:
    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    DownloadControl m_downloadCtrol;
    QStandardItemModel  *m_listModel;
    QVector<DownloadItem *> m_downloadItemList;
};

#endif // MAINWINDOW_H