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
    void addItemToListView(const DownloadItem &item);
    bool isExists(int code);

public slots:
    void start(int code);
    void stop(int code);
    void removeDown(int code);

    void progressChange(int code, qint64 val, qint64 total);
    void downloadFinished(int code);
    void downloadError(int code, const QString &err);
private slots:
    void on_tableView_clicked(const QModelIndex &index);

    void on_flushBtn_clicked();

private:
    Ui::MainWindow *ui;
    DownloadControl m_downloadCtrol;
    QStandardItemModel  *m_listModel;
    QVector<DownloadItem *> m_downloadItemList;
};

#endif // MAINWINDOW_H
