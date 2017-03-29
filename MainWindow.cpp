/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    MainWindow.cpp
 * @brief
 *******************************************************************************
 */
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "processdelegate.h"
#include "ButtonDelegate.h"
#include <QMessageBox>

#define ITEM_CODE Qt::UserRole + 1

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if(!init())
    {
        qApp->quit();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::init()
{
    m_listModel = new QStandardItemModel(this);
    if(!m_listModel)
    {
        return false;
    }
    m_listModel->setHorizontalHeaderLabels(QStringList()<<"FileName"<<"Progress"<<"Start"<<"Remove"<<"Status");
    ui->tableView->setModel(m_listModel);
    ui->tableView->setItemDelegateForColumn(1, new ProcessDelegate);
    ui->tableView->setItemDelegateForColumn(2, new ButtonDelegate);
    ui->tableView->setItemDelegateForColumn(3, new ButtonDelegate);
    ui->tableView->setColumnWidth(0,200);
    ui->tableView->setColumnWidth(1,300);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(&m_downloadCtrol, SIGNAL(progressChange(int,qint64,qint64)), this,SLOT(progressChange(int,qint64,qint64)));
    connect(&m_downloadCtrol, SIGNAL(downloadFinished(int)), this,SLOT(downloadFinished(int)));
    connect(&m_downloadCtrol, SIGNAL(downloadError(int,QString)), this, SLOT(downloadError(int,QString)));
    return true;
}



void MainWindow::addItemToListView(const DownloadItem &item)
{
    if(isExists(item.getCode()))
    {
        qDebug()<<"addItemToListView(int code) element is exists!" << item.getCode();
        return;
    }
    QList<QStandardItem*> list;
    QStandardItem *item0 = new QStandardItem(QString("2016-08-30_T_19-06-44.651_GMT_%1.m4v").arg(item.getCode()));
    item0->setData(item.getCode(),ITEM_CODE);
    list.append(item0);
    list.append(new QStandardItem("0"));
    list.append(new QStandardItem("Start"));
    list.append(new QStandardItem("Remove"));
    list.append(new QStandardItem("Waiting"));
    m_listModel->appendRow(list);
}

bool MainWindow::isExists(int code)
{
    for(int i=0; i<m_listModel->rowCount(); ++i)
    {
        bool ok = false;
        int idx = m_listModel->index(i,0).data(ITEM_CODE).toInt(&ok);
        if(idx == code)
        {
            return true;
        }
    }
    return false;
}

void MainWindow::start(int index)
{
    m_downloadCtrol.startDownload(index);
    qDebug()<<"启动下载:"<<index<<m_downloadCtrol.queueCount();
}

void MainWindow::stop(int index)
{
    m_downloadCtrol.stopDownload(index);
}

void MainWindow::removeDown(int index)
{
    int idx = m_downloadCtrol.remove(index);
    qDebug()<<"停止下载"<<index<<idx<<m_downloadCtrol.queueCount();
    if(idx >= 0)
    {
        for(int i=0; i<m_listModel->rowCount(); ++i)
        {
            int idx  = m_listModel->data(m_listModel->index(i,0), ITEM_CODE).toInt();
            if(idx == index)
            {
                m_listModel->removeRow(i);
            }
        }
    }
}

void MainWindow::progressChange(int index, qint64 val, qint64 total)
{
    if(total == 0)
    {
        return;
    }
    for(int i=0; i<m_listModel->rowCount(); ++i)
    {
        int idx = m_listModel->data(m_listModel->index(i,0),ITEM_CODE).toInt();
        if(idx == index)
        {
            int proc = val * 100 / total;
            m_listModel->setData(m_listModel->index(i,1),proc);
            return;
        }
    }
}

void MainWindow::downloadFinished(int index)
{
    for(int i=0; i<m_listModel->rowCount(); ++i)
    {
        int idx = m_listModel->data(m_listModel->index(i,0),ITEM_CODE).toInt();
        if(idx == index)
        {
            m_listModel->setData(m_listModel->index(i,2), QString("Start"));
            m_listModel->setData(m_listModel->index(i,4),QString("Finished!"));
            return;
        }
    }
}

void MainWindow::downloadError(int index, const QString &err)
{
//    qDebug()<<"Error: index:"<<index<<" message:"<<err;
//    QMessageBox::warning(Q_NULLPTR, tr("Warnging"), tr("Downlaod error! Index:%1, msg: %2").arg(index).arg(err));
    for(int i=0; i<m_listModel->rowCount(); ++i)
    {
        int idx = m_listModel->data(m_listModel->index(i,0),ITEM_CODE).toInt();
        if(idx == index)
        {
//            stop(index);// if it has error the stop download
            m_listModel->setData(m_listModel->index(i,2), "Start", Qt::EditRole);
            m_listModel->setData(m_listModel->index(i,4),err);
            return;
        }
    }
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    int idx = m_listModel->data(m_listModel->index(index.row(),0),ITEM_CODE).toInt();
    if(index.column() == 2)
    {
        qDebug()<<"Start btn:"<<idx<<index.data(Qt::DisplayRole).toString();
        if(index.data().toString() == "Start")
        {
            m_listModel->setData(index,"Stop",Qt::EditRole);
            start(idx);
        }
        else
        {
            m_listModel->setData(index, "Start", Qt::EditRole);
            stop(idx);
        }
    }
    else if(index.column() == 3)
    {
        QString status = m_listModel->data(m_listModel->index(index.row(), 2),Qt::DisplayRole).toString();
        if(status != "Start")
        {
            QMessageBox::warning(Q_NULLPTR, tr("Ｗarning"),tr("Place stop it first!"));
            return;
        }
        removeDown(idx);
    }

}

void MainWindow::on_flushBtn_clicked()
{
    for(int i=0; i<5; ++i)
    {
        int code = i + 100;
        DownloadItem item(code);
        item.setUrl("http://10.69.143.81/2016-08-30_T_19-06-44.651_GMT.m4v");
        item.setSaveFileName(QString("/home/zxl/zxl/2016-08-30_T_19-06-44.651_GMT_%1.m4v").arg(code));
        addItemToListView(item);
    }
}
