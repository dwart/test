#include "vtkview.h"

#include <QMessageBox>
//#include <vtkUnstructuredGrid.h>
//#include <vtkXMLDataSetWriter.h>
//#include <vtkSimplePointsWriter.h>
//#include <vtkXMLPolyDataWriter.h>
VTKView::VTKView(QObject *parent) :
    QObject(parent)
{
////    vtk = new QVTKWidget();

//    colors = vtkUnsignedCharArray::New();
//        colors->SetNumberOfComponents(3);
//        colors->SetName("Colors");
//        addColor(255,255,255);
//        addColor(0,0,0);
//        addColor(0,0,0);
//        addColor(255,0,0);
////        addColor(0,0,255);
////        addColor(0,255,0);
//    points = vtkPoints::New();
//        addPoint(0,0,0);
//        addPoint(0,0,1);
//        addPoint(0,1,0);
//            addPoint(0,1,1);
////        addPoint(1,0,0);
////        addPoint(1,0,1);
////        addPoint(1,1,0);
//    object = vtkPolyData::New();
//        object->SetPoints(points);
//        object->GetPointData()->SetScalars(colors);
//    delaunay3D = vtkDelaunay3D::New();
//        delaunay3D->BoundingTriangulationOff();
//        delaunay3D->SetTolerance(0.001);
//        delaunay3D->SetInput(object);
//        delaunay3D->Update();

//        vtkSimplePointsWriter *writer = vtkSimplePointsWriter::New();
//        writer->SetFileName("/home/dwart/vtk.xyz");
//        writer->SetFileTypeToASCII();
//        writer->SetInputConnection(delaunay3D->GetOutputPort(0));
//        writer->Update();
//        writer->Write();

//        vtkXMLPolyDataWriter *w = vtkXMLPolyDataWriter::New();
//        w->SetFileName("/home/dwart/vtk.vtp");
//        w->SetCompressorTypeToNone();
//        w->SetInput(object);
//        w->Update();
//        w->Write();



////    delaunayMapper = vtkDataSetMapper::New();
////        delaunayMapper->SetInputConnection(delaunay3D->GetOutputPort());
////        delaunayMapper->Update();
////    objectActor = vtkActor::New();
////        objectActor->GetProperty()->SetRepresentationToPoints();
////        objectActor->SetMapper(delaunayMapper);
////    renderer = vtkRenderer::New();
////        renderer->SetBackground(0.2,0.2,0.2);
////        renderer->AddActor(objectActor);
////    renderWindow = vtkRenderWindow::New();
////        renderWindow = vtk->GetRenderWindow();
////        renderWindow->SetStereoTypeToDresden();
////        renderWindow->AddRenderer(renderer);
////        renderWindow->Render();

////    vtk->SetRenderWindow(renderWindow);
}



//void VTKView::addColor(int r, int g, int b)
//{
//    unsigned char color[3] = {(unsigned char)r, (unsigned char)g, (unsigned char)b};
//    colors->InsertNextTupleValue(color);
//}
//void VTKView::addColor(double r, double g, double b)
//{
//    unsigned char color[3] = {(unsigned char)(255.0*r), (unsigned char)(255.0*g), (unsigned char)(255.0*b)};
//    colors->InsertNextTupleValue(color);
//}
//void VTKView::addPoint(double x, double y, double z)
//{
//    points->InsertNextPoint(x, y, z);
//}
