#ifndef PROCESSDELEGATE_H
#define PROCESSDELEGATE_H
#include <QStyledItemDelegate>

class ProcessDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ProcessDelegate();
protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // PROCESSDELEGATE_H
