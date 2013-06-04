#include "dialoginfo.h"
#include "ui_dialoginfo.h"
#include <QTableWidget>

dialogInfo::dialogInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogInfo)
{
    ui->setupUi(this);
    connect(ui->tableWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(selectRow()));
    connect(ui->buttonUncheckAll, SIGNAL(clicked()), this, SLOT(uncheckAll()));

}

dialogInfo::~dialogInfo()
{
    delete ui;
}


void dialogInfo::blockTableSignals(bool x)
{
    ui->tableWidget->blockSignals(x);
}

void dialogInfo::setInfo(QString text)
{
    ui->textInfo->setText(text);
}

void dialogInfo::clearTable()
{
    ui->tableWidget->clear();
}

void dialogInfo::setRowCount(int x)
{
    ui->tableWidget->setRowCount(x);
}

void dialogInfo::addRow(int row, QString id, QString x, QString y, QString z, QString r, QString g, QString b)
{
    ui->tableWidget->setItem(row,0, new QTableWidgetItem(id));
    ui->tableWidget->setItem(row,1, new QTableWidgetItem(x));
    ui->tableWidget->setItem(row,2, new QTableWidgetItem(y));
    ui->tableWidget->setItem(row,3, new QTableWidgetItem(z));
    ui->tableWidget->setItem(row,4, new QTableWidgetItem(r));
    ui->tableWidget->setItem(row,5, new QTableWidgetItem(g));
    ui->tableWidget->setItem(row,6, new QTableWidgetItem(b));
}

double dialogInfo::getCurrentX()
{
    int row = ui->tableWidget->currentRow();
    QString item = ui->tableWidget->item(row, 1)->text();
    return item.toDouble();
}

double dialogInfo::getCurrentY()
{
    int row = ui->tableWidget->currentRow();
    QString item = ui->tableWidget->item(row, 2)->text();
    return item.toDouble();
}

double dialogInfo::getCurrentZ()
{
    int row = ui->tableWidget->currentRow();
    QString item = ui->tableWidget->item(row, 3)->text();
    return item.toDouble();
}

void dialogInfo::selectRow()
{
    selectedPoints.clear();
    for(int i=0; i<ui->tableWidget->selectionModel()->selectedRows().count(); i++)
    {
        selectedPoints.append(ui->tableWidget->selectionModel()->selectedRows().at(i).row());
    }

    emit selectedItem();
}

void dialogInfo::uncheckAll()
{
    selectedPoints.clear();
    ui->tableWidget->selectionModel()->clearSelection();
    emit selectedItem();
}
