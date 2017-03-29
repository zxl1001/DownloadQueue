/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    ButtonDelegate.cpp
 * @brief
 *******************************************************************************
 */
#include "ButtonDelegate.h"
#include <QApplication>
#include <QStyle>
#include <QStyleHints>

ButtonDelegate::ButtonDelegate()
{

}

void ButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() >1 && index.column() <5)
    {
        QString title = index.data().toString();
        QStyleOptionButton btnOption;
        btnOption.rect = option.rect.adjusted(4,4,-4,-4);
        btnOption.text = title;
        btnOption.state = QStyle::State_Raised;//QStyle::State_Sunken : QStyle::State_Raised

        QApplication::style()->drawControl(QStyle::CE_PushButton,&btnOption,painter);
    }
    else
    {
        QStyledItemDelegate::paint(painter,option,index);
    }
}
