/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    Download.cpp
 * @brief
 *******************************************************************************
 */
#include "Download.h"
#include <QFileInfo>
#include <QDir>

Download::Download(int index, QObject *parent)
    :QObject(parent)
{
    m_index = index;
    m_netManager = new QNetworkAccessManager(this);
}

Download::~Download()
{

}

void Download::startDownload(const QString url, QString saveFile, qint64 statPos, qint64 endPos, qint64 readySize)
{
    if(m_status == DownloadStatus::Downloading)
    {
          errorInfo = tr("is downloading a file!");
          emit downloadError(m_index, errorInfo);
          return;
    }

    if(!m_netManager)
    {
        errorInfo = tr("Network manager object is NULL!");
        emit downloadError(m_index, errorInfo);
        return;
    }

    m_saveFileNames = saveFile;
    m_url = url;
    m_startPos = statPos;
    m_endPos  = endPos;
    m_readySize = readySize;

    QFileInfo saveFileInfo(m_saveFileNames);
    if(!saveFileInfo.absoluteDir().exists())
    {
        errorInfo = QString("The path[%1] not is exist!").arg(m_saveFileNames);
        emit downloadError(m_index,errorInfo);
        return;
    }
    m_saveFile.setFileName(saveFileInfo.absoluteFilePath());
    if(!m_saveFile.open(QIODevice::WriteOnly))
    {
        errorInfo = QString("The file to be saved was opened fail! File name is:%1").arg(saveFileInfo.fileName());
        emit downloadError(m_index,errorInfo);
        return;
    }

    QNetworkRequest request;
    request.setUrl(QUrl(m_url));
    m_reply = m_netManager->get(request);

    //connect signal
    connect(m_reply, SIGNAL(finished()),this, SLOT(finished()));
    connect(m_reply, SIGNAL(readyRead()), this, SLOT(readRead()));
    connect(m_reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this,SLOT(errorSlot(QNetworkReply::NetworkError)));

    m_status = DownloadStatus::Downloading;
}

void Download::setIndex(int index)
{
    m_index = index;
}

int Download::index() const
{
    return m_index;
}

Download::DownloadStatus Download::status() const
{
    return m_status;
}

void Download::stop()
{
    if(m_status != DownloadStatus::Downloading)
    {
        errorInfo = "is not downloading";
        emit downloadError(m_index, errorInfo);
        return;
    }

    if(m_saveFile.isOpen())
    {
        m_saveFile.flush();
    }

    m_saveFile.close();

    if(m_reply)
    {
        m_reply->abort();
        m_reply->deleteLater();;
    }

    m_status = DownloadStatus::Stoped;

}

void Download::restart()
{
    if(m_status != DownloadStatus::Stoped)
    {
        errorInfo = "is not stoped!";
        emit downloadError(m_index, errorInfo);
        return;
    }
    m_saveFile.seek(m_startPos);
    startDownload(m_url, m_saveFileNames, m_startPos, m_endPos, m_readySize);
}

void Download::errorSlot(QNetworkReply::NetworkError code)
{
    if(m_status == DownloadStatus::Downloading)
    {
        return;
    }
    errorInfo = QString("QNetworkReply::NetworkError :%1\n%2").arg((int)code).arg(m_reply->errorString());
}
void Download::readRead()
{
    //write info to file
    QByteArray buffer = m_reply->readAll();
    m_saveFile.write(buffer, buffer.length());
}

void Download::finished()
{
    if(m_status != DownloadStatus::Downloading)
    {
        return;
    }
    m_saveFile.flush();
    m_saveFile.close();
    m_reply->deleteLater();
    m_status = DownloadStatus::Finished;
    emit downloadFinished(m_index);
}

void Download::downloadProgress(qint64 val, qint64 total)
{
    emit downloadProgressChange(m_index, val, total);
}
