#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QGridLayout>
#include <QFrame>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->showMaximized();

    connect(ui->actionZamknij, SIGNAL(triggered()), this, SLOT(Zamknij()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), this, SLOT(AboutQt()));
    connect(ui->actionOProgramie, SIGNAL(triggered()), this, SLOT(About()));

    QTimer::singleShot(250, this, SLOT(initialize()));
}

MainWindow::~MainWindow()
{
    delete scanner;
    delete glWidget;
    delete ui;
}

void MainWindow::sleep(int ms)
{
    QTime dieTime = QTime::currentTime().addMSecs(ms);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents);
}

void MainWindow::initialize()
{
    ui->labelStart->setText("Wczytuję moduły skanera...");
    sleep(500);
    scanner = new Scanner;
    ui->layoutScanner->addWidget(scanner);
    scanner->mcuConnect->sendMessage("c");
    scanner->mcuConnect->sendMessage("c");

    ui->labelStart->setText("Wczytuję moduły obrazów...");
    sleep(350);
    photoWidget = new PhotoWidget;
    ui->layoutPhoto->addWidget(photoWidget);

    ui->labelStart->setText("Wczytuję moduły widoku 3D...");
    sleep(50);
    glWidget = new GLWidget;
    ui->layoutGL->addWidget(glWidget);
    glWidget->setCursor(Qt::CrossCursor);


    connect(photoWidget, SIGNAL(newTable(QString,QString)), glWidget->db, SLOT(addNewTable(QString,QString)));
    connect(photoWidget, SIGNAL(newPoint(QString,QList<int>,QList<double>,QList<double>,QList<double>,QList<double>,QList<double>,QList<double>)), this, SLOT(pointAdd(QString,QList<int>,QList<double>,QList<double>,QList<double>,QList<double>,QList<double>,QList<double>)));

    ui->labelStart->hide();
}


void MainWindow::Zamknij()
{
    QApplication::closeAllWindows();
}
void MainWindow::About()
{
    QString text;
    text =  "<center><br><b>3DScanner</b><br>";
    text += "\"Generowanie modeli trójwymiarowych<br>";
    text += "z wykorzystaniem skanera 3D.\"<br>";
    text += "<br>Praca magisterska pod kierunkiem<br>";
    text += "dr. Rafała Stęgierskiego<br>";
    text += "<br>Katolicki Uniwerystet Lubelski Jana Pawła II<br>";
    text += "Wydział Matematyki, Informatyki<br>";
    text += "i Architektury Krajobrazu<br>";
    text += "<br> Damian Walec <br>";
    text += "2012/2013</center>";

    QMessageBox::information(this, "O programie", text);
}
void MainWindow::AboutQt()
{
    QApplication::aboutQt();
}


void MainWindow::pointAdd(QString nazwa, QList<int> id, QList<double> x, QList<double> y, QList<double> z, QList<double> r, QList<double> g, QList<double> b)
{
    if(glWidget->db->addPoint(nazwa, id, x,y,z, r,g,b))
        photoWidget->przerabiac = true;
}

