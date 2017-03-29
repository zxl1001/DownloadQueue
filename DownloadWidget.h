/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    DownloadWidget.h
 * @brief
 *******************************************************************************
 */
#ifndef DOWNLOADWIDGET_H
#define DOWNLOADWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>

class DownloadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DownloadWidget(int index, const QString &fileName, const QString &url,QWidget *parent = 0);
    virtual ~DownloadWidget();

    int index() const;

signals:
    void start(int index);
    void stop(int index);
    void removeDown(int index);
public slots:
    void updateProgressBar(int index, qint64 val, qint64 total);
    void downloadFinished(int index);
    void updateStatus(const QString &status);
private slots:
    void startDown();
    void stopDown();
    void removeDown();
private:
    int m_index;
    QString m_fileUrl;
    QLabel *m_fileNameLabel;
    QLabel *m_statusLabel;
    QProgressBar *m_progressBar;
    QPushButton  *m_startDownload;
    QPushButton  *m_stopDownload;
    QPushButton  *m_rmDownload;

};

#endif // DOWNLOADWIDGET_H
