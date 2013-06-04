#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include <QList>
class Triangulation
{
public:
    Triangulation();
    double distance;

    QList<double> getLine(QList<int> id, QList<double> x, QList<double> y, QList<double> z, QList<double> r, QList<double> g, QList<double> b);
    QList<double> getTriangle(QList<int> id, QList<double> x, QList<double> y, QList<double> z, QList<double> r, QList<double> g, QList<double> b);
};

#endif // TRIANGULATION_H
