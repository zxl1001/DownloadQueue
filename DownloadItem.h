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


class DownloadItem
{
public:
    explicit DownloadItem(int idx);
    int index() const;
    void setIndex(int index);

private:
    int m_index;
};

#endif // DOWNLOADITEM_H
