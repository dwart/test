#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringList>

class Database : public QObject
{
    Q_OBJECT
public:
    Database();
    ~Database();
    QSqlDatabase db;
    QStringList getDbList();
    QStringList getDbDetail(QString);
    void deleteDbObject(QString);


private:
    QStringList DbList;
    QStringList DbDetail;

public slots:
    void addNewTable(QString nazwa, QString opis);
    void addPoint(QString nazwa, int id, double x, double y, double z, double r, double g, double b);
    void addPoint(QString nazwa, int id, double x, double y, double z);
    bool addPoint(QString nazwa, QList<int> id, QList<double> x, QList<double> y, QList<double> z, QList<double> r, QList<double> g, QList<double> b);

signals:
    void notConnected();
};

#endif // DATABASE_H
