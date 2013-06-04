#include "triangulation.h"
#include <cmath>

Triangulation::Triangulation()
{
    distance = 1;
}

QList<double> Triangulation::getLine(QList<int> id, QList<double> x, QList<double> y, QList<double> z, QList<double> r, QList<double> g, QList<double> b)
{
    QList<double> Line;

    QList<double> line1X;
    QList<double> line1Y;
    QList<double> line1Z;
    QList<double> line2X;
    QList<double> line2Y;
    QList<double> line2Z;
    QList<double> color1R;
    QList<double> color1G;
    QList<double> color1B;
    QList<double> color2R;
    QList<double> color2G;
    QList<double> color2B;
    Line.clear();
    int listCount = id.at(id.count()-1);
    int currentList = id.at(0);
    int nextList = 0;
    int currentId = 0;
    int tmpId = 0;

    int q = 0;
    QList<double> listaSrednich;
    double srednia;

    while(currentList <= listCount)
    {
//        CZYSZCZENIE LIST:
        line1X.clear();line1Y.clear();line1Z.clear();
        line2X.clear();line2Y.clear();line2Z.clear();
        color1R.clear();color1G.clear();color1B.clear();
        color2R.clear();color2G.clear();color2B.clear();
        currentId = tmpId;
//        WCZYTYWANIE LINI 1:
        while(id.at(currentId) == currentList)
        {
            line1X.append(x.at(currentId));
            line1Y.append(y.at(currentId));
            line1Z.append(z.at(currentId));
            color1R.append(r.at(currentId));
            color1G.append(g.at(currentId));
            color1B.append(b.at(currentId));
            currentId++;
        }

//        #####################LICZENIE SREDNIEJ
        q=0;
        while(q<line1X.count()-1)
        {
            srednia = (line1X.at(q) - line1X.at(q+1))*(line1X.at(q) - line1X.at(q+1));
            srednia += (line1Y.at(q) - line1Y.at(q+1))*(line1Y.at(q) - line1Y.at(q+1));
            srednia += (line1Z.at(q) - line1Z.at(q+1))*(line1Z.at(q) - line1Z.at(q+1));
            srednia = sqrt(srednia);
            listaSrednich.append(srednia);
            q++;
        }
        srednia = 0;
        for(int i=0; i<listaSrednich.count(); i++)
        {
            srednia += listaSrednich.at(i);
        }
        srednia = srednia / listaSrednich.count();
//        ######################################

//        USTAWIENIE AKTUALNEJ LISTY:
        if(currentList == listCount-1)
        {
            currentId = 0;
            nextList = id.at(0);
        }
        else
        {
            tmpId = currentId;
            nextList = currentList+1;
        }

//        WCZYTYWANIE LINI 2:
        while(id.at(currentId) == nextList)
        {
            line2X.append(x.at(currentId));
            line2Y.append(y.at(currentId));
            line2Z.append(z.at(currentId));
            color2R.append(r.at(currentId));
            color2G.append(g.at(currentId));
            color2B.append(b.at(currentId));
            currentId++;
        }

//        TRIANGULACJA:
        while(line1X.count()>1 && line2X.count()>1)
        {
            if(line1Y.at(0) >= line2Y.at(0))
            {
                if(sqrt((line1X.at(0) - line1X.at(1))*(line1X.at(0) - line1X.at(1)) +
                        (line1Y.at(0) - line1Y.at(1))*(line1Y.at(0) - line1Y.at(1)) +
                        (line1Z.at(0) - line1Z.at(1))*(line1Z.at(0) - line1Z.at(1))) < srednia*distance)
                {
                    Line.append(color1R.at(0));
                    Line.append(color1G.at(0));
                    Line.append(color1B.at(0));
                    Line.append(line1X.at(0));
                    Line.append(line1Y.at(0));
                    Line.append(line1Z.at(0));

                    Line.append(color1R.at(1));
                    Line.append(color1G.at(1));
                    Line.append(color1B.at(1));
                    Line.append(line1X.at(1));
                    Line.append(line1Y.at(1));
                    Line.append(line1Z.at(1));
                }

                if(sqrt((line1X.at(1) - line2X.at(0))*(line1X.at(1) - line2X.at(0)) +
                        (line1Y.at(1) - line2Y.at(0))*(line1Y.at(1) - line2Y.at(0)) +
                        (line1Z.at(1) - line2Z.at(0))*(line1Z.at(1) - line2Z.at(0))) < srednia*distance)
                {
                    Line.append(color1R.at(1));
                    Line.append(color1G.at(1));
                    Line.append(color1B.at(1));
                    Line.append(line1X.at(1));
                    Line.append(line1Y.at(1));
                    Line.append(line1Z.at(1));

                    Line.append(color2R.at(0));
                    Line.append(color2G.at(0));
                    Line.append(color2B.at(0));
                    Line.append(line2X.at(0));
                    Line.append(line2Y.at(0));
                    Line.append(line2Z.at(0));
                }

                if(sqrt((line2X.at(0) - line1X.at(0))*(line2X.at(0) - line1X.at(0)) +
                        (line2Y.at(0) - line1Y.at(0))*(line2Y.at(0) - line1Y.at(0)) +
                        (line2Z.at(0) - line1Z.at(0))*(line2Z.at(0) - line1Z.at(0))) < srednia*distance)
                {
                    Line.append(color2R.at(0));
                    Line.append(color2G.at(0));
                    Line.append(color2B.at(0));
                    Line.append(line2X.at(0));
                    Line.append(line2Y.at(0));
                    Line.append(line2Z.at(0));

                    Line.append(color1R.at(0));
                    Line.append(color1G.at(0));
                    Line.append(color1B.at(0));
                    Line.append(line1X.at(0));
                    Line.append(line1Y.at(0));
                    Line.append(line1Z.at(0));
                }

                color1R.removeAt(0);
                color1G.removeAt(0);
                color1B.removeAt(0);
                line1X.removeAt(0);
                line1Y.removeAt(0);
                line1Z.removeAt(0);
            }
            else
            {
                if(sqrt((line2X.at(0) - line2X.at(1))*(line2X.at(0) - line2X.at(1)) +
                        (line2Y.at(0) - line2Y.at(1))*(line2Y.at(0) - line2Y.at(1)) +
                        (line2Z.at(0) - line2Z.at(1))*(line2Z.at(0) - line2Z.at(1))) < srednia*distance)
                {
                    Line.append(color2R.at(0));
                    Line.append(color2G.at(0));
                    Line.append(color2B.at(0));
                    Line.append(line2X.at(0));
                    Line.append(line2Y.at(0));
                    Line.append(line2Z.at(0));

                    Line.append(color2R.at(1));
                    Line.append(color2G.at(1));
                    Line.append(color2B.at(1));
                    Line.append(line2X.at(1));
                    Line.append(line2Y.at(1));
                    Line.append(line2Z.at(1));
                }

                if(sqrt((line2X.at(1) - line1X.at(0))*(line2X.at(1) - line1X.at(0)) +
                        (line2Y.at(1) - line1Y.at(0))*(line2Y.at(1) - line1Y.at(0)) +
                        (line2Z.at(1) - line1Z.at(0))*(line2Z.at(1) - line1Z.at(0))) < srednia*distance)
                {
                    Line.append(color2R.at(1));
                    Line.append(color2G.at(1));
                    Line.append(color2B.at(1));
                    Line.append(line2X.at(1));
                    Line.append(line2Y.at(1));
                    Line.append(line2Z.at(1));

                    Line.append(color1R.at(0));
                    Line.append(color1G.at(0));
                    Line.append(color1B.at(0));
                    Line.append(line1X.at(0));
                    Line.append(line1Y.at(0));
                    Line.append(line1Z.at(0));
                }

                if(sqrt((line1X.at(0) - line2X.at(0))*(line1X.at(0) - line2X.at(0)) +
                        (line1Y.at(0) - line2Y.at(0))*(line1Y.at(0) - line2Y.at(0)) +
                        (line1Z.at(0) - line2Z.at(0))*(line1Z.at(0) - line2Z.at(0))) < srednia*distance)
                {
                    Line.append(color1R.at(0));
                    Line.append(color1G.at(0));
                    Line.append(color1B.at(0));
                    Line.append(line1X.at(0));
                    Line.append(line1Y.at(0));
                    Line.append(line1Z.at(0));

                    Line.append(color2R.at(0));
                    Line.append(color2G.at(0));
                    Line.append(color2B.at(0));
                    Line.append(line2X.at(0));
                    Line.append(line2Y.at(0));
                    Line.append(line2Z.at(0));
                }

                color2R.removeAt(0);
                color2G.removeAt(0);
                color2B.removeAt(0);
                line2X.removeAt(0);
                line2Y.removeAt(0);
                line2Z.removeAt(0);
            }
        }


        currentList++;
    }



    return Line;
}

QList<double> Triangulation::getTriangle(QList<int> id, QList<double> x, QList<double> y, QList<double> z, QList<double> r, QList<double> g, QList<double> b)
{
    QList<double> Triangle;

    QList<double> line1X;
    QList<double> line1Y;
    QList<double> line1Z;
    QList<double> line2X;
    QList<double> line2Y;
    QList<double> line2Z;
    QList<double> color1R;
    QList<double> color1G;
    QList<double> color1B;
    QList<double> color2R;
    QList<double> color2G;
    QList<double> color2B;
    Triangle.clear();
    int listCount = id.at(id.count()-1);
    int currentList = id.at(0);
    int nextList = 0;
    int currentId = 0;
    int tmpId = 0;

    while(currentList <= listCount)
    {
//        CZYSZCZENIE LIST:
        line1X.clear();line1Y.clear();line1Z.clear();
        line2X.clear();line2Y.clear();line2Z.clear();
        color1R.clear();color1G.clear();color1B.clear();
        color2R.clear();color2G.clear();color2B.clear();
        currentId = tmpId;
//        WCZYTYWANIE LINI 1:
        while(id.at(currentId) == currentList)
        {
            line1X.append(x.at(currentId));
            line1Y.append(y.at(currentId));
            line1Z.append(z.at(currentId));
            color1R.append(r.at(currentId));
            color1G.append(g.at(currentId));
            color1B.append(b.at(currentId));
            currentId++;
        }

//        USTAWIENIE AKTUALNEJ LISTY:
        if(currentList == listCount-1)
        {
            currentId = 0;
            nextList = id.at(0);
        }
        else
        {
            tmpId = currentId;
            nextList = currentList+1;
        }

//        WCZYTYWANIE LINI 2:
        while(id.at(currentId) == nextList)
        {
            line2X.append(x.at(currentId));
            line2Y.append(y.at(currentId));
            line2Z.append(z.at(currentId));
            color2R.append(r.at(currentId));
            color2G.append(g.at(currentId));
            color2B.append(b.at(currentId));
            currentId++;
        }

//        TRIANGULACJA:
        while(line1X.count()>1 && line2X.count()>1)
        {
            if(line1Y.at(0) >= line2Y.at(0))
            {
                Triangle.append(color1R.at(0));
                Triangle.append(color1G.at(0));
                Triangle.append(color1B.at(0));
                Triangle.append(line1X.at(0));
                Triangle.append(line1Y.at(0));
                Triangle.append(line1Z.at(0));

                Triangle.append(color1R.at(1));
                Triangle.append(color1G.at(1));
                Triangle.append(color1B.at(1));
                Triangle.append(line1X.at(1));
                Triangle.append(line1Y.at(1));
                Triangle.append(line1Z.at(1));

                Triangle.append(color2R.at(0));
                Triangle.append(color2G.at(0));
                Triangle.append(color2B.at(0));
                Triangle.append(line2X.at(0));
                Triangle.append(line2Y.at(0));
                Triangle.append(line2Z.at(0));

                color1R.removeAt(0);
                color1G.removeAt(0);
                color1B.removeAt(0);
                line1X.removeAt(0);
                line1Y.removeAt(0);
                line1Z.removeAt(0);
            }
            else
            {
                Triangle.append(color2R.at(0));
                Triangle.append(color2G.at(0));
                Triangle.append(color2B.at(0));
                Triangle.append(line2X.at(0));
                Triangle.append(line2Y.at(0));
                Triangle.append(line2Z.at(0));

                Triangle.append(color2R.at(1));
                Triangle.append(color2G.at(1));
                Triangle.append(color2B.at(1));
                Triangle.append(line2X.at(1));
                Triangle.append(line2Y.at(1));
                Triangle.append(line2Z.at(1));

                Triangle.append(color1R.at(0));
                Triangle.append(color1G.at(0));
                Triangle.append(color1B.at(0));
                Triangle.append(line1X.at(0));
                Triangle.append(line1Y.at(0));
                Triangle.append(line1Z.at(0));

                color2R.removeAt(0);
                color2G.removeAt(0);
                color2B.removeAt(0);
                line2X.removeAt(0);
                line2Y.removeAt(0);
                line2Z.removeAt(0);
            }
        }


        currentList++;
    }

    return Triangle;
}










