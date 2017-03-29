/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    ButtonDelegate.h
 * @brief
 *******************************************************************************
 */
#ifndef BUTTONDELEGATE_H
#define BUTTONDELEGATE_H
#include <QStyledItemDelegate>

class ButtonDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ButtonDelegate();
protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // BUTTONDELEGATE_H
