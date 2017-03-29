/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    DownloadControl.cpp
 * @brief
 *******************************************************************************
 */
#include "DownloadControl.h"
#include <QDebug>

DownloadControl::DownloadControl()
{

}

DownloadControl::~DownloadControl()
{
    foreach (auto *item, m_queue) {
        if(item->status() == Download::DownloadStatus::Downloading)
        {
            item->stop();
        }
        delete item;
        item = Q_NULLPTR;
    }
    m_queue.clear();
}

const QList<Download *> &DownloadControl::queue() const
{
    return m_queue;
}

void DownloadControl::startDownload(int index)
{
    foreach (auto *item, m_queue)
    {
        if(item->index() == index)
        {

            if(item->status() == Download::DownloadStatus::Finished)
            {
                qDebug()<<__FILE__<<__LINE__<<"在队列中已存在.且已经下载完成"<<item->index();
                emit downloadFinished(item->index());
                return;
            }
            qDebug()<<__FILE__<<__LINE__<<"在队列中已存在.不需要添加"<<item->index();
            item->restart();
            return;
        }
    }
    Download *item  = new Download(index);
    m_queue.append(item);
    connect(item, SIGNAL(downloadProgressChange(int,qint64,qint64)), this, SIGNAL(progressChange(int,qint64,qint64)));
    connect(item, SIGNAL(downloadError(int,QString)), this, SLOT(error(int,QString)));
    connect(item, SIGNAL(downloadFinished(int)), this, SIGNAL(downloadFinished(int)));
    item->startDownload("http://10.69.143.81/2016-08-30_T_19-06-44.651_GMT.m4v", QString("/home/zxl/zxl/2016-08-30_T_19-06-44.651_GMT_%1.m4v").arg(item->index()), 0, -1);
}

void DownloadControl::stopDownload(int index)
{
    foreach(auto *item, m_queue)
    {
        if(item->index() == index)
        {
            item->stop();
        }
    }
}

int DownloadControl::remove(int index)
{
    foreach (auto *item, m_queue) {
        if(item->index() == index)
        {
            item->stop();
            if(m_queue.removeOne(item))
            {
                delete item;
                item = Q_NULLPTR;
                return index;
            }
            else
            {
                return -1;
            }

        }
    }
    return 0;
}

int DownloadControl::queueCount()
{
    return m_queue.count();
}

void DownloadControl::error(int index, const QString &err)
{
    qDebug()<<"Error: index:"<<index<<" message:"<<err;
}
