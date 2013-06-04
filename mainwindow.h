#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "scanner.h"
#include "glwidget.h"
#include "photowidget.h"
#include <QMenuBar>
#include <QWidget>
#include <QMessageBox>
#include <QTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    Scanner *scanner;
    GLWidget *glWidget;
    PhotoWidget *photoWidget;

    void sleep(int ms);


private slots:
    void initialize();
    void Zamknij();
    void About();
    void AboutQt();
    void pointAdd(QString nazwa, QList<int> id, QList<double> x, QList<double> y, QList<double> z, QList<double> r, QList<double> g, QList<double> b);

};

#endif // MAINWINDOW_H
