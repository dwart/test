#include "database.h"

Database::Database()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./Database.db");
    if(db.open())
        QSqlQuery query("CREATE TABLE IF NOT EXISTS DB(nazwa TEXT, opis TEXT, id INTEGER, x DOUBLE, y DOUBLE, z DOUBLE , r DOUBLE DEFAULT 1, g DOUBLE DEFAULT 1, b DOUBLE DEFAULT 1)");
    else
        emit notConnected();


//    addNewTable("test", "Trochę większy obiekt");
//    for(int i=1; i<=20; i++)
//        addPoint("test",1,(double)-20,(double)i,(double)20, (double)0.8, (double)0, (double)0);
//    for(int i=1; i<=20; i++)
//        addPoint("test",2,(double)20,(double)i,(double)20, (double)0,(double)0.8,(double)0);
//    for(int i=1; i<=20; i++)
//        addPoint("test",3,(double)20,(double)i,(double)-20, (double)0.8,(double)0,(double)0);
//    for(int i=1; i<=20; i++)
//        addPoint("test",4,(double)-20,(double)i,(double)-20, (double)0,(double)0,(double)0.8);

//    addNewTable("Losowe6", "Losowe punkty");
//    double x, y, z, r, g, b;
//    for(int i=1; i<20; i++)
//    {
//        x = rand()%100000;
//        x /= 10000;
//        y = rand()%100000;
//        y /= 10000;
//        z = rand()%100000;
//        z /= 10000;
//        r = rand()%1000;
//        r /= 1000;
//        g = rand()%1000;
//        g /= 1000;
//        b = rand()%1000;
//        b /= 1000;
//        addPoint("Losowe6" , 1, x,y,z,r,g,b);
//    }
}
Database::~Database()
{
    db.close();
}


QStringList Database::getDbList()
{
    DbList.clear();
    QSqlQuery query("SELECT * FROM DB GROUP BY nazwa");
    while(query.next())
        DbList.append(query.value(0).toString());
    return DbList;
}

QStringList Database::getDbDetail(QString x)
{
    DbDetail.clear();
    QString polecenie = "SELECT * FROM DB WHERE nazwa='" + x + "'";// ORDER BY id";
    QSqlQuery query(polecenie);


    while(query.next())
    {
        DbDetail.append(query.value(1).toString());     // opis
        DbDetail.append(query.value(2).toString());     // id
        DbDetail.append(query.value(3).toString());     // x
        DbDetail.append(query.value(4).toString());     // y
        DbDetail.append(query.value(5).toString());     // z
        if(query.value(6).toString() != "")
            DbDetail.append(query.value(6).toString()); // r
        else
            DbDetail.append("null");                    // r
        if(query.value(7).toString() != "")
            DbDetail.append(query.value(7).toString()); // g
        else
            DbDetail.append("null");                    // g
        if(query.value(8).toString() != "")
            DbDetail.append(query.value(8).toString()); // b
        else
            DbDetail.append("null");                    // b
    }

    return DbDetail;
}

void Database::deleteDbObject(QString x)
{
    QString polecenie = "DELETE FROM DB WHERE nazwa='" + x + "'";
    QSqlQuery query(polecenie);
}


void Database::addNewTable(QString nazwa, QString opis)
{
    QString polecenie = "INSERT INTO DB (nazwa, opis) VALUES ('" + nazwa + "', '" + opis +"')";
    QSqlQuery query(polecenie);
}

void Database::addPoint(QString nazwa, int id, double x, double y, double z, double r, double g, double b)
{
    QString polecenie = "INSERT INTO DB (nazwa,id,x,y,z,r,g,b) VALUES ('"+ nazwa +"'," + QString::number(id) + "," +
            QString::number(x) + ", " + QString::number(y)+ "," + QString::number(z) + "," +
               QString::number(r)+ ", " + QString::number(g)+ ", " + QString::number(b) + ")";
    QSqlQuery query(polecenie);
}

void Database::addPoint(QString nazwa, int id, double x, double y, double z)
{
    QString polecenie = "INSERT INTO DB (nazwa,id,x,y,z) VALUES ('"+ nazwa +"'," + QString::number(id) + "," +
               QString::number(x) + ", " + QString::number(y)+ "," + QString::number(z) + ")";
    QSqlQuery query(polecenie);
}

bool Database::addPoint(QString nazwa, QList<int> id, QList<double> x, QList<double> y, QList<double> z, QList<double> r, QList<double> g, QList<double> b)
{
//    QString polecenie;
//    polecenie = "BEGIN;";
//    for(int i=0; i<id.count(); i++)
//    {
//        polecenie += "INSERT INTO DB (nazwa,id,x,y,z,r,g,b) VALUES ('"+ nazwa +"'," + QString::number(id.at(i)) + "," +
//                QString::number(x.at(i)) + ", " + QString::number(y.at(i))+ "," + QString::number(z.at(i)) + "," +
//                   QString::number(r.at(i))+ ", " + QString::number(g.at(i))+ ", " + QString::number(b.at(i)) + ");";
//    }
//    polecenie += "COMMIT;";

//    QSqlQuery query(polecenie);

    QSqlQuery query;
    query.prepare("BEGIN;");
    query.exec();
    for(int i=0; i<id.count(); i++)
    {
        query.prepare("INSERT INTO DB (nazwa,id,x,y,z,r,g,b) VALUES (:nazwa, :id, :x, :y, :z, :r, :g, :b)");
        query.bindValue(":nazwa", nazwa);
        query.bindValue(":id", QString::number(id.at(i)));
        query.bindValue(":x", QString::number(x.at(i)));
        query.bindValue(":y", QString::number(y.at(i)));
        query.bindValue(":z", QString::number(z.at(i)));
        query.bindValue(":r", QString::number(r.at(i)));
        query.bindValue(":g", QString::number(g.at(i)));
        query.bindValue(":b", QString::number(b.at(i)));
        query.exec();
    }
    query.prepare("COMMIT;");
    query.exec();

    return true;
}
