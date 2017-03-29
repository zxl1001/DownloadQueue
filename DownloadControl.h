/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    DownloadControl.h
 * @brief
 *******************************************************************************
 */
#ifndef DOWNLOADCONTROL_H
#define DOWNLOADCONTROL_H

#include "Download.h"
#include <QObject>
#include <QList>

class DownloadControl : public QObject
{
    Q_OBJECT
public:
    DownloadControl();
    virtual ~DownloadControl();
    const QList<Download *> &queue() const;
    void startDownload(int index);
    void stopDownload(int index);
    /**
     * @ remove one item in download queue
     * @param index: queue code index
     * @return:
     *  0:not exits
     *  -1: exits in queue but remove fail
     *  >0: exits in queue and remove success.
     */
    int  remove(int index);

    int queueCount();
signals:
    void progressChange(int index, qint64 val, qint64 total);
    void downloadFinished(int index);
    void downloadError(int index, const QString &err);
public slots:

private:
    QList<Download *> m_queue;

};

#endif // DOWNLOADCONTROL_H
