#ifndef VTKVIEW_H
#define VTKVIEW_H

#include <QObject>

////#include <QVTKWidget.h>
//#include <vtkProperty.h>
//#include <vtkUnsignedCharArray.h>
//#include <vtkPoints.h>
//#include <vtkPolyData.h>
//#include <vtkDelaunay3D.h>
//#include <vtkDataSetMapper.h>
//#include <vtkLineSource.h>

////   SPRAWDZIĆ!!!!!!!!
////**********************************************************************
//#include <vtkUnsignedCharArray.h>
//#include <vtkPoints.h>
//#include <vtkPolyData.h>
//#include <vtkPolyDataMapper.h>
//#include <vtkActor.h>
//#include <vtkRenderer.h>
//#include <vtkRenderWindow.h>
//#include <vtkDelaunay3D.h>

//#include <vtkPointData.h>
//#include <vtkDataSet.h>

//// -----------
//#include <vtkRenderer.h>
//#include <vtkRenderWindow.h>
//#include <vtkPolyDataMapper.h>
//#include <vtkActor.h>

//#include <vtkVectorText.h>
//#include <vtkLineSource.h>

//// TRIANGULACJA:
//#include <vtkSmartPointer.h>
//#include <vtkCleanPolyData.h>
//#include <vtkDelaunay3D.h>
//#include <vtkXMLDataSetWriter.h>
//#include <vtkXMLPolyDataReader.h>
//#include <vtkSmartPointer.h>
//#include <vtkCellArray.h>
//#include <vtkCellData.h>
//#include <vtkLine.h>
//#include <vtkProperty.h>
//#include <vtkPointSource.h>
//#include <vtkVertexGlyphFilter.h>
//#include <vtkPointData.h>
//#include <vtkPolyDataMapper.h>
//#include <vtkPolyData.h>
//#include <vtkPointData.h>
//#include <vtkDataCompressor.h>
//#include <vtkPointLocator.h>
//#include <vtkDataSetMapper.h>
//#include <vtkRenderWindowInteractor.h>
//// --------------

class VTKView : public QObject
{
    Q_OBJECT
public:
    explicit VTKView(QObject *parent = 0);

//    QVTKWidget *vtk;

    int drawType;
    QList<int> listId;                           // Lista id dla punktów i obiektów
    QList<double> listPointsX;                   // Lista punktów obiektu X
    QList<double> listPointsY;                   // Lista punktów obiektu Y
    QList<double> listPointsZ;                   // Lista punktów obiektu Z
    QList<double> listColorsR;                   // Lista kolorów obiektu R
    QList<double> listColorsG;                   // Lista kolorów obiektu G
    QList<double> listColorsB;                   // Lista kolorów obiektu B


private:
//    vtkUnsignedCharArray *colors;               // Kolory obiektu
//    vtkPoints *points;                          // Punkty obiektu
//    vtkPolyData *object;                        // Główny obiekt
//    vtkDelaunay3D *delaunay3D;                  // Triangulacja
//    vtkDataSetMapper *delaunayMapper;           // Mapper triangulacji
//    vtkActor *objectActor;                      // Aktor obiektu
//    vtkRenderer *renderer;                      // Renderer
//    vtkRenderWindow *renderWindow;              // Wyrenderowane okno

//    vtkLineSource *netObject;
//    vtkPolyDataMapper *netMapper;
//    vtkActor *netActor;
//    vtkPoints *ptsNet;                          // Punkty siatki


    void addColor(int r, int g, int b);
    void addColor(double r, double g, double b);
    void addPoint(double x, double y, double z);
    
signals:
    
public slots:
    
};

#endif // VTKVIEW_H
