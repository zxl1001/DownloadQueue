/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    DownloadWidget.cpp
 * @brief
 *******************************************************************************
 */
#include "DownloadWidget.h"
#include <QDebug>

DownloadWidget::DownloadWidget(int index, const QString &fileName, const QString &url, QWidget *parent)
    :QWidget(parent)
{
    m_index               = index;
    m_fileUrl             = url;
    m_fileNameLabel       = new QLabel(fileName,this);
    m_statusLabel         = new QLabel(tr("Waiting"),this);
    m_startDownload       = new QPushButton("Start", this);
    m_stopDownload        = new QPushButton("Stop", this);
    m_rmDownload          = new QPushButton("Remove", this);
    m_stopDownload->setDisabled(true);

    m_progressBar         = new QProgressBar(this);
    m_progressBar->setMinimum(0);
    m_progressBar->setMaximum(100);
    m_progressBar->setValue(0);



    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(m_fileNameLabel);
    layout->addWidget(m_progressBar);
    layout->addWidget(m_statusLabel);
    layout->addWidget(m_startDownload);
    layout->addWidget(m_stopDownload);
    layout->addWidget(m_rmDownload);
    setLayout(layout);

    connect(m_startDownload, SIGNAL(clicked()), this,SLOT(startDown()));
    connect(m_stopDownload, SIGNAL(clicked()), this,SLOT(stopDown()));
    connect(m_rmDownload, SIGNAL(clicked()), this,SLOT(removeDown()));
}

DownloadWidget::~DownloadWidget()
{

}

void DownloadWidget::updateProgressBar(int index ,qint64 val, qint64 total)
{
    if(total == 0)
    {
        return;
    }
    if(index == m_index)
    {
        int proc = val * 100 / total;
        m_progressBar->setValue(proc);
    }
}

void DownloadWidget::downloadFinished(int index)
{
    if(index == m_index)
    {
        qDebug()<<index<<"finished";
        m_startDownload->setDisabled(false);
        m_rmDownload->setDisabled(false);
        m_stopDownload->setDisabled(true);
        m_statusLabel->setText("finished");
    }
}

void DownloadWidget::updateStatus(const QString &status)
{
    m_statusLabel->setText(status);
}

void DownloadWidget::startDown()
{
    m_startDownload->setDisabled(true);
    m_rmDownload->setDisabled(true);
    m_stopDownload->setDisabled(false);
    emit start(m_index);
}

void DownloadWidget::stopDown()
{
    m_startDownload->setDisabled(false);
    m_rmDownload->setDisabled(false);
    m_stopDownload->setDisabled(true);
    emit stop(m_index);
}

void DownloadWidget::removeDown()
{
    m_startDownload->setDisabled(true);
    m_stopDownload->setDisabled(true);
    emit removeDown(m_index);
}

int DownloadWidget::index() const
{
    return m_index;
}
