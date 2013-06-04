#-------------------------------------------------
#
# Project created by QtCreator 2013-03-02T16:21:10
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += opengl

LIBS += -lglut -lGLU

INCLUDEPATH += /usr/local/include/opencv2/
LIBS += -L/usr/local/lib \
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_imgproc \
    -lopencv_features2d \
    -lopencv_calib3d


# VTK:
#LIBS    += -L/usr/lib/vtk-5.10 -lvtkCommon -lvtkRendering -lvtkGraphics -lvtkImaging -lvtkIO -lvtksys -lvtkViews -lvtkWidgets -lvtkInfovis  -lvtkFiltering -lvtkDICOMParser #-lvtkalglib -lvtkverdict -lvtkmetaio -lvtkNetCDF -lvtksqlite -lvtkexoIIc -lvtkftgl -lvtkHybrid -lQVTK -lvtklibxml2 -lvtkpng -lvtkpng -lvtktiff -lvtkzlib -lvtkjpeg -lvtkexpat -lvtkfreetype -lvtkQtChart
#INCLUDEPATH += /usr/include/vtk-5.10
# ---------------------------------------

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3DScanner
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    database.cpp \
    dialoginfo.cpp \
    triangulation.cpp \
    scanner.cpp \
    photowidget.cpp \
    stopbutton.cpp \
    vtkview.cpp \
    glview.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    database.h \
    dialoginfo.h \
    triangulation.h \
    scanner.h \
    photowidget.h \
    stopbutton.h \
    vtkview.h \
    glview.h

FORMS    += mainwindow.ui \
    glwidget.ui \
    dialoginfo.ui \
    scanner.ui \
    photowidget.ui

RESOURCES += \
    Zasoby.qrc
