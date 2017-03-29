/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    DownloadItem.h
 * @brief
 *******************************************************************************
 */
#ifndef DOWNLOADITEM_H
#define DOWNLOADITEM_H
#include <QString>

class DownloadItem
{
public:
    explicit DownloadItem(int idx);

    QString saveFileName() const;
    void setSaveFileName(const QString &saveFileName);

    QString getUrl() const;
    void setUrl(const QString &value);

    int getCode() const;
    void setCode(int code);

private:
    int m_code;
    QString m_saveFileName;
    QString url;
};

#endif // DOWNLOADITEM_H
