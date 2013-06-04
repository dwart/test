#ifndef GLWIDGET_H
#define GLWIDGET_H

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
#include "database.h"
#include "dialoginfo.h"
#include "triangulation.h"
#include "glview.h"




namespace Ui {
class GLWidget;
}



class GLWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();

    GLView *glView;
    Database *db;
    dialogInfo *objectInfoDialog;

private:
    Ui::GLWidget *ui;

private slots:
    void loadDB();

    void drawObject();
    void showObjectDetail();
    void deleteObject();

    void setPointType();
    void setLineType();
    void setPolygonType();

    void selectCurrentPoint();
    void hideCurrentPoint();

    void setClearColor();
};

#endif // GLWIDGET_H
