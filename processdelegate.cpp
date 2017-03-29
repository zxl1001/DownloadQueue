#include "processdelegate.h"
#include <QApplication>
#include <QStyle>

ProcessDelegate::ProcessDelegate()
{

}

void ProcessDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 1)
    {
        int progress = index.data().toInt();
        QStyleOptionProgressBar progressBarOption;
        progressBarOption.rect = option.rect.adjusted(4,4,-4,-4);
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 100;
        progressBarOption.progress = progress;
        progressBarOption.text = QString::number(progress) + "%";
        progressBarOption.textVisible = true;

        QApplication::style()->drawControl(QStyle::CE_ProgressBar,&progressBarOption,painter);
    }
    else
    {
        QStyledItemDelegate::paint(painter,option,index);
    }
}
