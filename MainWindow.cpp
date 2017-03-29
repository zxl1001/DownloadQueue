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
    m_listModel->setHorizontalHeaderLabels(QStringList()<<"FileName"<<"Progress"<<"Start"<<"Stop"<<"Remove"<<"Status");
    ui->tableView->setModel(m_listModel);
    ui->tableView->setItemDelegateForColumn(1, new ProcessDelegate);
    ui->tableView->setItemDelegateForColumn(2, new ButtonDelegate);
    ui->tableView->setItemDelegateForColumn(3, new ButtonDelegate);
    ui->tableView->setColumnWidth(0,200);
    ui->tableView->setColumnWidth(1,300);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    search();
    connect(&m_downloadCtrol, SIGNAL(progressChange(int,qint64,qint64)), this,SLOT(progressChange(int,qint64,qint64)));
    connect(&m_downloadCtrol, SIGNAL(downloadFinished(int)), this,SLOT(downloadFinished(int)));
    connect(&m_downloadCtrol, SIGNAL(downloadError(int,QString)), this, SLOT(downloadError(int,QString)));
    return true;
}

void MainWindow::search()
{
    for(int i = 0; i < 5; ++i)
    {
        DownloadItem *item  = new DownloadItem(i+100);
        m_downloadItemList.append(item);
    }
    updateListView();
}

void MainWindow::updateListView()
{
    m_listModel->removeRows(0, m_listModel->rowCount());
    for(int i = 0; i < m_downloadItemList.count(); ++i)
    {
//        auto *w = m_widgets[i];
//        QStandardItem *item = new QStandardItem(""start);
//        m_listModel->appendRow(item);
//        ui->listView->setIndexWidget(m_listModel->index(i,0),w);
//        item->setSizeHint(w->size());
//        connect(w,SIGNAL(start(int)), this, SLOT(start(int)));
//        connect(w,SIGNAL(stop(int)), this, SLOT(stop(int)));
//        connect(w,SIGNAL(removeDown(int)), this,SLOT(removeDown(int)));
//        connect(this,SIGNAL(progressChange(int,qint64,qint64)), w, SLOT(updateProgressBar(int,qint64,qint64)));
//        connect(this,SIGNAL(downloadFinished(int)), w, SLOT(downloadFinished(int)));
        int code = m_downloadItemList[i]->index();
        QStandardItem *item0 = new QStandardItem(QString("2016-08-30_T_19-06-44.651_GMT_%1.m4v").arg(code));
        item0->setData(code,Qt::UserRole+1);
        m_listModel->setItem(i,0, item0);
        m_listModel->setItem(i,1,new QStandardItem("0"));
        m_listModel->setItem(i,2,new QStandardItem("Start"));
        m_listModel->setItem(i,3,new QStandardItem("Remove"));
        m_listModel->setItem(i,4,new QStandardItem("Waiting"));
    }
}

bool MainWindow::isExists()
{

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
        foreach (auto *item, m_downloadItemList) {
            if(item->index() == index)
            {
                m_downloadItemList.removeOne(item);
                delete item;
                item = Q_NULLPTR;
                break;
            }
        }
        updateListView();
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
        int idx = m_listModel->data(m_listModel->index(i,0),Qt::UserRole+1).toInt();
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
        int idx = m_listModel->data(m_listModel->index(i,0),Qt::UserRole+1).toInt();
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
        int idx = m_listModel->data(m_listModel->index(i,0),Qt::UserRole+1).toInt();
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
    int idx = m_listModel->data(m_listModel->index(index.row(),0),Qt::UserRole+1).toInt();
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
    search();
}
