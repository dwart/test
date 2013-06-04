#include "glwidget.h"
#include "ui_glwidget.h"


GLWidget::GLWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GLWidget)
{
    ui->setupUi(this);


    db = new Database;
    objectInfoDialog = new dialogInfo(this);
    loadDB();

    glView = new GLView;
    ui->gridView->addWidget(glView);
    glView->setLayout(ui->gridView);
    glView->listId.clear();
    glView->listPointsX.clear();
    glView->listPointsY.clear();
    glView->listPointsZ.clear();
    glView->listColorsR.clear();
    glView->listColorsG.clear();
    glView->listColorsB.clear();

    glView->setCursor(Qt::CrossCursor);

    connect(ui->buttonNet, SIGNAL(clicked(bool)), glView, SLOT(setDrawNet(bool)));
    connect(ui->buttonXYZ, SIGNAL(clicked(bool)), glView, SLOT(setDrawXyz(bool)));


    connect(ui->buttonDbRefresh, SIGNAL(clicked()), this, SLOT(loadDB()));

    connect(ui->buttonDraw, SIGNAL(clicked()), this, SLOT(drawObject()));
    connect(ui->buttonInfo, SIGNAL(clicked()), this, SLOT(showObjectDetail()));
    connect(ui->buttonDelete, SIGNAL(clicked()), this, SLOT(deleteObject()));

    connect(ui->buttonPoint, SIGNAL(clicked()), this, SLOT(setPointType()));
    connect(ui->buttonLine, SIGNAL(clicked()), this, SLOT(setLineType()));
    connect(ui->buttonPolygon, SIGNAL(clicked()), this, SLOT(setPolygonType()));
    connect(ui->checkTetgen, SIGNAL(toggled(bool)), glView, SLOT(setTetgen(bool)));

    connect(ui->sliderBrightness, SIGNAL(valueChanged(int)), this, SLOT(setClearColor()));

    connect(ui->spinDistance, SIGNAL(valueChanged(double)),glView, SLOT(setDistance(double)));
}

GLWidget::~GLWidget()
{
    delete ui;
}


void GLWidget::loadDB()
{
    QStringList list = db->getDbList();
    ui->comboDB->clear();
    ui->comboDB->addItems(list);
    if(ui->comboDB->count()>0)
        ui->comboDB->setCurrentIndex(0);
}

void GLWidget::drawObject()
{
    if(ui->comboDB->count() <= 0)
    {
        QMessageBox::information(this, "Rysowanie obiektu", "Brak zapisanych obiektów");
        return;
    }
    if(ui->comboDB->currentIndex() < 0)
    {
        QMessageBox::information(this, "Rysowanie obiektu", "Nie wybrano obiektu");
        return;
    }

    ui->progressBar->setValue(ui->progressBar->minimum());
    ui->progressBar->setFormat("Wczytywanie obiektu...");
    ui->progressBar->setValue(ui->progressBar->maximum());
//    POBIERANIE PUNKTOW:
    QString nazwa = ui->comboDB->currentText();
    QStringList list = db->getDbDetail(nazwa);
    glView->listId.clear();
    glView->listPointsX.clear();
    glView->listPointsY.clear();
    glView->listPointsZ.clear();
    glView->listColorsR.clear();
    glView->listColorsG.clear();
    glView->listColorsB.clear();
    for(int i=8; i<list.count(); i+=8)
    {
        glView->listId.append(list.at(i+1).toInt());
        glView->listPointsX.append(list.at(i+2).toDouble());
        glView->listPointsY.append(list.at(i+3).toDouble());
        glView->listPointsZ.append(list.at(i+4).toDouble());
        glView->listColorsR.append(list.at(i+5).toDouble());
        glView->listColorsG.append(list.at(i+6).toDouble());
        glView->listColorsB.append(list.at(i+7).toDouble());
    }
    glView->newObject = true;
    ui->progressBar->setValue(ui->progressBar->minimum());

//    RYSOWANIE:
    if(ui->buttonPoint->isChecked())
    {
        ui->progressBar->setFormat("Rysowanie obiektu...");
        ui->progressBar->setValue(ui->progressBar->maximum());
        glView->drawType = 1;
        glView->updateGL();
        ui->progressBar->setFormat("%p%");
        ui->progressBar->setValue(ui->progressBar->maximum());
        return;
    }
    if(ui->buttonLine->isChecked())
    {
        ui->progressBar->setFormat("Rysowanie obiektu...");
        ui->progressBar->setValue(ui->progressBar->maximum());
        glView->drawType = 2;
        glView->updateGL();
        ui->progressBar->setFormat("%p%");
        ui->progressBar->setValue(ui->progressBar->maximum());
        return;
    }
    if(ui->buttonPolygon->isChecked())
    {
        ui->progressBar->setFormat("Rysowanie obiektu...");
        ui->progressBar->setValue(ui->progressBar->maximum());
        glView->drawType = 3;
        glView->updateGL();
        ui->progressBar->setFormat("%p%");
        ui->progressBar->setValue(ui->progressBar->maximum());
        return;
    }
}

void GLWidget::showObjectDetail()
{
    ui->progressBar->setValue(ui->progressBar->minimum());
    ui->progressBar->setFormat("Wczytywanie danych z bazy danych...");
    ui->progressBar->setValue(ui->progressBar->maximum());
    if(ui->comboDB->currentIndex() < 0)
    {
        QMessageBox::information(this, "Wczytywanie danych", "Nie wybrano obiektu");
        ui->progressBar->setFormat("%p%");
        return;
    }
    delete objectInfoDialog;
    objectInfoDialog = new dialogInfo(this);
    connect(objectInfoDialog, SIGNAL(selectedItem()), this, SLOT(selectCurrentPoint()));
    connect(objectInfoDialog, SIGNAL(finished(int)), this, SLOT(hideCurrentPoint()));

    objectInfoDialog->setWindowTitle(ui->comboDB->currentText());

    QStringList list = db->getDbDetail(ui->comboDB->currentText());

    int rowCount = list.count() / 8;
    objectInfoDialog->setRowCount(rowCount-1);

    objectInfoDialog->setInfo(list[0]);
    int x;
    ui->progressBar->setFormat("Wczytuję szczegóły obiektu %p%");
    ui->progressBar->setMaximum(rowCount/1000);
    ui->progressBar->setValue(0);

    objectInfoDialog->blockTableSignals(true);

    for(int i=1; i<rowCount; i++)
    {
        x = i*8;
        objectInfoDialog->addRow(i-1, list[x+1], list[x+2], list[x+3], list[x+4], list[x+5], list[x+6], list[x+7]);
        if(i%1000==1)
            ui->progressBar->setValue(ui->progressBar->value()+1);
    }

    objectInfoDialog->blockTableSignals(false);

    objectInfoDialog->show();
    ui->progressBar->setFormat("%p%");
    ui->progressBar->setValue(ui->progressBar->maximum());
}

void GLWidget::deleteObject()
{
    if(ui->comboDB->currentIndex() < 0)
    {
        QMessageBox::information(this, "Usuwanie obiektu", "Nie wybrano obiektu");
        return;
    }

    int del = QMessageBox::question(this, "Usuwanie obiektu", "Napewno chcesz usunąć model\n" + ui->comboDB->currentText() + " ?",
                                    QMessageBox::Ok, QMessageBox::Abort);
    if (del != QMessageBox::Ok)
        return;

    ui->progressBar->setFormat("Usuwanie obiektu " + ui->comboDB->currentText() + "... Proszę czekać...");
    ui->progressBar->setValue(ui->progressBar->minimum());
    db->deleteDbObject(ui->comboDB->currentText());
    loadDB();
    ui->progressBar->setFormat("Obiekt usunięto pomyślnie");
    ui->progressBar->setValue(ui->progressBar->maximum());
}








void GLWidget::setPointType()
{
    ui->progressBar->setValue(ui->progressBar->minimum());
    ui->progressBar->setFormat("Rysowanie...");
    ui->progressBar->setValue(ui->progressBar->maximum());

    glView->drawType = 1;
    glView->newObject = true;
    ui->buttonPoint->setChecked(true);
    ui->buttonLine->setChecked(false);
    ui->buttonPolygon->setChecked(false);
    if(!glView->listId.isEmpty())
        glView->updateGL();

    ui->progressBar->setValue(ui->progressBar->minimum());
    ui->progressBar->setFormat("%p%");
    ui->progressBar->setValue(ui->progressBar->maximum());
}
void GLWidget::setLineType()
{
    ui->progressBar->setValue(ui->progressBar->minimum());
    ui->progressBar->setFormat("Rysowanie...");
    ui->progressBar->setValue(ui->progressBar->maximum());

    glView->drawType = 2;
    glView->newObject = true;
    ui->buttonPoint->setChecked(false);
    ui->buttonLine->setChecked(true);
    ui->buttonPolygon->setChecked(false);
    if(!glView->listId.isEmpty())
        glView->updateGL();

    ui->progressBar->setValue(ui->progressBar->minimum());
    ui->progressBar->setFormat("%p%");
    ui->progressBar->setValue(ui->progressBar->maximum());
}
void GLWidget::setPolygonType()
{
    ui->progressBar->setValue(ui->progressBar->minimum());
    ui->progressBar->setFormat("Rysowanie...");
    ui->progressBar->setValue(ui->progressBar->maximum());

    glView->drawType = 3;
    glView->newObject = true;
    ui->buttonPoint->setChecked(false);
    ui->buttonLine->setChecked(false);
    ui->buttonPolygon->setChecked(true);
    if(!glView->listId.isEmpty())
        glView->updateGL();

    ui->progressBar->setValue(ui->progressBar->minimum());
    ui->progressBar->setFormat("%p%");
    ui->progressBar->setValue(ui->progressBar->maximum());
}

void GLWidget::selectCurrentPoint()
{
    glView->selectedPoints = objectInfoDialog->selectedPoints;
    glView->updateGL();
    glView->updateGL();
}
void GLWidget::hideCurrentPoint()
{
    glView->selectedPoints.clear();
    glView->updateGL();
    glView->updateGL();
}


void GLWidget::setClearColor()
{
    glView->clearColor = (double)ui->sliderBrightness->value() / 100;
    glView->updateGL();
}
