#ifndef GLVIEW_H
#define GLVIEW_H

#include <QWidget>
#include <QGLWidget>
#include <QtOpenGL>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <QList>
#include <QWheelEvent>
#include <QGridLayout>
#include <QList>
#include <QFile>
#include "triangulation.h"

class GLView : public QGLWidget
{
    Q_OBJECT

public:
    GLView();
    ~GLView();

    int drawType;
    QList<int> listId;                          // Lista id dla punktów i obiektów
    QList<double> listPointsX;                   // Lista punktów obiektu X
    QList<double> listPointsY;                   // Lista punktów obiektu Y
    QList<double> listPointsZ;                   // Lista punktów obiektu Z
    QList<double> listColorsR;                   // Lista kolorów obiektu R
    QList<double> listColorsG;                   // Lista kolorów obiektu G
    QList<double> listColorsB;                   // Lista kolorów obiektu B

    bool newObject;
    GLuint object;
    QList<double> line;
    QList<double> triangle;

    QList<int> selectedPoints;
    double clearColor;
private:
    bool isDrawNet, isDrawXyz;
    int zoom;                                   // Pozycja kamery w osi Z
    int xScreen;                                // Pozycja kamery w osi X
    int yScreen;                                // Pozycja kamery w osi Y
    float rotX, rotY, rotZ;                     // Zmienne zapamiętujące obrót
    QPoint lastPos;                             // Pozycja kursora
    bool tetgen;                                // Triangulacja z wykorzystaniem tetgen

    void makeObject();
    void drawNet();
    void drawXyz();

    Triangulation *triangulation;


protected:
    void initializeGL();                        // Inicjalizacja openGL'a
    void resizeGL(int width, int height);
    void paintGL();                             // Funkcja rysująca
    void normalizeAngle(int *angle);            // Normalizowanie kąta
    void mousePressEvent(QMouseEvent *event);   // Funkcja obsługująca klik myszy
    void mouseMoveEvent(QMouseEvent *event);    // Funkcja obsługująca ruch myszy
    void wheelEvent(QWheelEvent *event);        // Funkcja obsługująca scroll myszki


public slots:
    void setXRotation(int angle);               // Ustawienie obrotu w osi X
    void setYRotation(int angle);               // Ustawienie obrotu w osi Y
    void setZRotation(int angle);               // Ustawienie obrotu w osi Z
    void setDrawNet(bool x){isDrawNet = x; updateGL();}
    void setDrawXyz(bool x){isDrawXyz = x; updateGL();}

    void setTetgen(bool b);

    double foundText(int param, QString text);
    void setDistance(double d) {triangulation->distance = d;}
};
#endif // GLVIEW_H
