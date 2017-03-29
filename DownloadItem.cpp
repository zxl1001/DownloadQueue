/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    DownloadItem.cpp
 * @brief
 *******************************************************************************
 */
#include "DownloadItem.h"

DownloadItem::DownloadItem(int idx)
{
    m_index = idx;
}

int DownloadItem::index() const
{
    return m_index;
}

void DownloadItem::setIndex(int index)
{
    m_index = index;
}

QString DownloadItem::saveFileName() const
{
    return m_saveFileName;
}

void DownloadItem::setSaveFileName(const QString &saveFileName)
{
    m_saveFileName = saveFileName;
}

QString DownloadItem::getUrl() const
{
    return url;
}

void DownloadItem::setUrl(const QString &value)
{
    url = value;
}
