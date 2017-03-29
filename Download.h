/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    Download.h
 * @brief
 *******************************************************************************
 */
#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>

class Download : public QObject
{
    Q_OBJECT
public:
    enum class DownloadStatus
    {
        Waiting, Downloading, Finished, Stoped
    };
    explicit Download(int index, QObject *parent = 0);
    virtual ~Download();
    void startDownload(const QString url, QString saveFile, qint64 statPos, qint64 endPos, qint64 readySize = 0);

    void setIndex(int index);
    int index() const;

    DownloadStatus status() const;

signals:
    void downloadFinished(int index);
    void downloadError(int index, const QString &downloadError);
    void downloadProgressChange(int index, qint64 val, qint64 total);

public slots:
    void stop();
    void restart();

private slots:
    void finished();
    void readRead();
    void errorSlot(QNetworkReply::NetworkError code);
    void downloadProgress(qint64, qint64);

private:
    int m_index;
    QString m_url;
    QString   m_saveFileNames;
    QFile     m_saveFile;
    qint64 m_startPos;
    qint64 m_endPos;
    qint64 m_readySize;
    QNetworkAccessManager *m_netManager;
    QNetworkReply *m_reply;
    DownloadStatus m_status;
    QString errorInfo;

};
#endif // DOWNLOAD_H
