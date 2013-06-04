#ifndef DIALOGINFO_H
#define DIALOGINFO_H

#include <QDialog>

namespace Ui {
class dialogInfo;
}

class dialogInfo : public QDialog
{
    Q_OBJECT
    
public:
    explicit dialogInfo(QWidget *parent = 0);
    ~dialogInfo();
    
    void setInfo(QString text);
    void clearTable();
    void setRowCount(int x);
    void addRow(int row, QString id, QString x, QString y, QString z, QString r, QString g, QString b);

    double getCurrentX();
    double getCurrentY();
    double getCurrentZ();

    QList<int> selectedPoints;


    void blockTableSignals(bool);
private:
    Ui::dialogInfo *ui;

private slots:
    void selectRow();
    void uncheckAll();

signals:
    void selectedItem();
};

#endif // DIALOGINFO_H
