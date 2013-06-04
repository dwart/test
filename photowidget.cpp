#include "photowidget.h"
#include "ui_photowidget.h"

#include <QTimer>
PhotoWidget::PhotoWidget(QWidget *parent) : QWidget(parent), ui(new Ui::PhotoWidget)
{
    ui->setupUi(this);

    QTimer::singleShot(250, this, SLOT(initialize()));
}

PhotoWidget::~PhotoWidget()
{
    delete ui;
}

void PhotoWidget::initialize()
{
    buttonStop = new QPushButton;
    buttonStop->setText("Przerwij");
    buttonStop->setCheckable(true);

    directory = new QDir;
    directory->setPath(directory->currentPath() + "/Scans");
    ui->buttonOpenDir->setToolTip(directory->path());
    loadDirs();

    currentPhoto = new Label("", 0);
    ui->layoutCurrentPhoto->addWidget(currentPhoto);
    currentPhoto->setAlignment(Qt::AlignCenter);

    frameList = new QWidget;
    layoutList = new QHBoxLayout;
    frameList->setLayout(layoutList);
    ui->listArea->setWidget(frameList);
    frameList->show();

    connect(ui->buttonOpenDir, SIGNAL(clicked()), this, SLOT(openDir()));
    connect(ui->comboListDir, SIGNAL(currentIndexChanged(int)), this, SLOT(loadPhotos()));

    connect(ui->buttonSkeletonization, SIGNAL(clicked()), this, SLOT(skeletonization()));
    connect(ui->buttonImprove, SIGNAL(clicked()), this, SLOT(improvePics()));
    connect(ui->buttonGenerate, SIGNAL(clicked()), this, SLOT(generatePoints()));


    ui->frameSkieletDetail->setVisible(false);
    connect(ui->buttonShowSkieletDetal, SIGNAL(clicked()), this, SLOT(showSkieletDetail()));
    connect(ui->sliderSkieletSegm, SIGNAL(valueChanged(int)), this, SLOT(changeSliderSkieletValue()));
    connect(ui->buttonSkieletSegm, SIGNAL(clicked()), this, SLOT(showSegmentation()));
    connect(ui->sliderLaserBegin, SIGNAL(valueChanged(int)), this, SLOT(showLinesBegin()));
    connect(ui->sliderLaserEnd, SIGNAL(valueChanged(int)), this, SLOT(showLinesEnd()));
    connect(ui->checkShowBeginEnd, SIGNAL(clicked()), this, SLOT(showLinesBegin()));
//    loadPhotos();
    ui->comboListDir->setCurrentIndex(-1);
}


void PhotoWidget::openDir()
{
    QString dirName = QFileDialog::getExistingDirectory(this, "Otwórz katalog", directory->path());
    if(dirName.isEmpty())
        return;
    directory->setPath(dirName);
    ui->buttonOpenDir->setToolTip(dirName);
    loadDirs();
}

void PhotoWidget::loadDirs()
{
    QStringList list = directory->entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    ui->comboListDir->clear();
    ui->comboListDir->addItems(list);
}

bool PhotoWidget::makeMiniPhoto(QDir *dir, QStringList list)
{
    QImage img;
    for(int i=0; i<list.count(); i++)
    {
        img.load(dir->absolutePath() + "/" + list.at(i));
        img = img.scaled(140,80, Qt::KeepAspectRatio, Qt::FastTransformation);
        if(!img.save(dir->absolutePath() + "/mini/" + list.at(i), "PNG"))
            return false;
    }
    return true;
}

void PhotoWidget::loadPhotos()
{
    ui->progressBar->setFormat("Wczytuje obrazy %p%");
    QString path = directory->absolutePath() + "/" + ui->comboListDir->currentText();
    QDir *dir = new QDir(path);
    path = dir->absolutePath();
    QStringList filtr;
    filtr.append("*.png");
    QStringList list = dir->entryList(filtr, QDir::Files, QDir::Name);

    ui->progressBar->setValue(0);
    if(list.count() > 100)
        ui->progressBar->setMaximum(list.count()/50);
    else
        ui->progressBar->setMaximum(list.count());

    for(int i=0; i<listPhoto.count(); i++)
    {
        delete listPhoto.at(i);
    }
    listPhoto.clear();
    ui->listArea->update();

    if(!QDir(dir->absolutePath() + "/mini/").exists())
    {
        ui->progressBar->setValue(ui->progressBar->minimum());
        ui->progressBar->setFormat("Tworzę miniaturki obrazów... Proszę czekać....");
        ui->progressBar->setValue(ui->progressBar->maximum());
        QTime dieTime = QTime::currentTime().addMSecs(200);
        while( QTime::currentTime() < dieTime )
            QCoreApplication::processEvents(QEventLoop::AllEvents);

        dir->mkdir(dir->absolutePath() + "/mini");
        if(!makeMiniPhoto(dir, list))
        {
            QMessageBox::warning(this, "Tworzenie miniaturek", "Wystąpił błąd przy tworzeniu miniaturek!");
            return;
        }
    }
    ui->progressBar->setValue(ui->progressBar->minimum());
    ui->progressBar->setFormat("Wczytuje obrazy %p%");

    for(int i=0; i<list.count(); i++)
    {
        QString t = dir->absolutePath() + "/" + list.at(i);
        QString mini = dir->absolutePath() + "/mini/" + list.at(i);
        QImage img;
//        img.load(t);
        img.load(mini);
        Label *l = new Label(t, this);
        l->path = t;
        l->setMaximumSize(140,80);
        l->setMinimumSize(140,80);
//        l->setPixmap(QPixmap::fromImage(img).scaled(l->width(), l->height(), Qt::KeepAspectRatio, Qt::FastTransformation));
        l->setPixmap(QPixmap::fromImage(img));
        l->setMaximumWidth(l->pixmap()->width());
        l->setMinimumWidth(l->pixmap()->width());
        listPhoto.append(l);
        connect(l, SIGNAL(clicked()), this, SLOT(listPhotoClicked()));

        if(list.count()>100)
            ui->progressBar->setValue(ui->progressBar->value()+1);
        else
        {
            if(!((i+1)%50))
                ui->progressBar->setValue(ui->progressBar->value()+1);
        }
    }
    ui->progressBar->setValue(ui->progressBar->maximum());

    for(int i=0; i<listPhoto.count(); i++)
    {
        layoutList->addWidget(listPhoto.at(i));
    }
    ui->listArea->update();
    if(listPhoto.count() < 1)
    {
        currentPhoto->setText("Brak plików w folderze!");
    }
    else
    {
        showPhoto(listPhoto[0]->path);
        currentPhotoNumber = 0;
    }

    delete dir;

}

void PhotoWidget::showPhoto(QString name)
{
    QImage img;
    img.load(name);
    if(ui->checkShowBeginEnd->isChecked())
    {
        double begin, end;
        begin = (double)ui->sliderLaserBegin->value()/1000;
        end = (double)ui->sliderLaserEnd->value()/1000;
        begin = img.width()*begin;
        end = img.width()*end;
        for(int y=0; y<img.height(); y++)
        {
            img.setPixel((int)begin, y, qRgb(200,0,0));
            img.setPixel((int)end, y, qRgb(200,0,0));
        }
    }
    currentPhotoPath = name;
    currentPhoto->clear();
    currentPhoto->setPixmap(QPixmap::fromImage(img).scaled(currentPhoto->width()-3, currentPhoto->height()-3,
                                                           Qt::KeepAspectRatio, Qt::FastTransformation));
    int x;
    for(int i=0; i<name.length(); i++)
    {
        if(name[i] == '/')
            x = i+1;
    }
    x = name.length()-x;
    name = name.right(x);
    ui->progressBar->setFormat(name);
}

void PhotoWidget::listPhotoClicked()
{
    int i = 0;
    while(i < listPhoto.count())
    {
        if(listPhoto[i]->isClicked)
        {
            listPhoto[i]->isClicked = false;
            QString path = listPhoto[i]->path;
            showPhoto(path);
            currentPhotoPath = path;
            currentPhotoNumber = i;
            return;
        }
        i++;
    }
}












void PhotoWidget::skeletonization()
{
    QString dirSkeletPath = ui->buttonOpenDir->toolTip() + "/" + ui->comboListDir->currentText() + "/Skelet";
    QDir d;
    d.mkdir(dirSkeletPath);

    QImage *img;
    QImage *imgl;
    QImage *tmp;

    QColor *color;
    QColor *c1;
    QColor *c2;

    ui->checkShowBeginEnd->setChecked(false);


    for(int k=0; k<listPhoto.count(); k+=2)
    {
        showPhoto(listPhoto.at(k)->path);
        img = new QImage(currentPhotoPath);

        showPhoto(listPhoto.at(k+1)->path);
        imgl = new QImage(currentPhotoPath);

        tmp = new QImage(img->width(), img->height(), QImage::Format_RGB888);


        color = new QColor;
        c1 = new QColor;
        c2 = new QColor;
        int value;
        int maxValue = 0;
        int minValue = 40000;
        int r,g,b;
        ui->progressBar->setMaximum(img->width()/10);
        ui->progressBar->setValue(0);
        ui->progressBar->setFormat(QString::number(k/2+1) + "/" + QString::number(listPhoto.count()/2) + ":  %p%");

        for(int i=0; i<img->width(); i++)
        {
            for(int j=0; j<img->height(); j++)
            {
                c1->setRgb(imgl->pixel(i,j));
                c2->setRgb(img->pixel(i,j));

//                value = c1->red()-c2->red();
//                if(value < 0)
//                    value = 0;

                if(i > img->width()*((double)ui->sliderLaserEnd->value()/1000))
                    value = 0;
                else
                {
                    if(i < img->width()*((double)ui->sliderLaserBegin->value()/1000))
                        value = 0;
                    else
                    {
                        value = c1->red()-c2->red();
                        if(value < 0)
                            value = 0;

                        if(value > maxValue)
                            maxValue = value;
                        if(value < minValue)
                            minValue = value;
                    }
                }


                color->setRgb(value, value, value);
                r = color->red();
                g = color->green();
                b = color->blue();

                tmp->setPixel(i, j, qRgb(r,g,b));
            }
            if(i%10 == 0)
                ui->progressBar->setValue(ui->progressBar->value()+1);
        }
//        tmp->save(ui->buttonOpenDir->toolTip() + "/" + ui->comboListDir->currentText() + "/Skelet/" + QString::number(k/2+1) + "-1.png", "PNG");

        ui->progressBar->setValue(0);
        ui->progressBar->setFormat("Obraz binarny: " + QString::number(k/2+1) + "/" + QString::number(listPhoto.count()/2) + ":  %p%");
        int avr = (minValue+maxValue)/ui->labelSliderSkieletValue->text().toDouble();
        for(int i=0; i<img->width(); i++)
        {
            for(int j=0; j<img->height(); j++)
            {
                c1->setRgb(tmp->pixel(i,j));

                value = c1->red();

                if(value > avr)
                    value = 255;
                else
                    value = 0;

                color->setRgb(value, value, value);
                r = color->red();
                g = color->green();
                b = color->blue();

                tmp->setPixel(i, j, qRgb(r,g,b));
            }
            if(i%10 == 0)
                ui->progressBar->setValue(ui->progressBar->value()+1);
        }
//        tmp->save(ui->buttonOpenDir->toolTip() + "/" + ui->comboListDir->currentText() + "/Skelet/" + QString::number(k/2+1) + "-2.png", "PNG");


        ui->progressBar->setValue(0);
        ui->progressBar->setMaximum(img->height()/10);
        ui->progressBar->setFormat("Optymalizacja: " + QString::number(k/2+1) + "/" + QString::number(listPhoto.count()/2) + ":  %p%");
        bool min;
        for(int i=0; i<img->height(); i++)
        {
            min = false;
            minValue = maxValue = 0;
            for(int j=0; j<img->width(); j++)
            {
                c1->setRgb(tmp->pixel(j,i));
                if(!min)
                {
                    if(c1->red()==255)
                    {
                        min = true;
                        minValue = j;
                        maxValue = j;
                    }
                }
                else
                {
                    if(c1->red()==255)
                    {
                        if(j-maxValue < ui->spinMaxSpace->value())
                            maxValue = j;
                    }
                }
                tmp->setPixel(j,i,qRgb(0,0,0));
            }

            avr = (minValue+maxValue)/2;
            if(avr!=0)
                tmp->setPixel(avr, i, qRgb(255,255,255));
            if(!i%10)
                ui->progressBar->setValue(ui->progressBar->value()+1);
        }


        tmp->save(ui->buttonOpenDir->toolTip() + "/" + ui->comboListDir->currentText() + "/Skelet/" + QString::number(k/2+1) + ".png", "PNG");
        delete img;
        delete imgl;
        delete tmp;
        delete color;
        delete c1;
        delete c2;
    }
    ui->progressBar->setFormat("Gotowe!  %p%");
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(100);


}

void PhotoWidget::improvePics()
{
    QString dirSkeletPath = ui->buttonOpenDir->toolTip() + "/" + ui->comboListDir->currentText() + "/Skelet";
    QDir d;
    d.setPath(dirSkeletPath);
    if(!d.exists())
    {
        QMessageBox::warning(this, "Poprawa obrazów", "Musisz najpierw użyć szkieletyzacji na obrazach!");
        return;
    }
}




void PhotoWidget::generatePoints()
{
    QString dirPath = ui->buttonOpenDir->toolTip() + "/" + ui->comboListDir->currentText();
    QString dirSkeletPath = ui->buttonOpenDir->toolTip() + "/" + ui->comboListDir->currentText() + "/Skelet";
    QDir d;
    d.setPath(dirSkeletPath);
    if(!d.exists())
    {
        QMessageBox::warning(this, "Generowanie punktów", "Musisz najpierw użyć szkieletyzacji na obrazach!");
        return;
    }

    idList.clear();
    xList.clear();
    yList.clear();
    zList.clear();
    rList.clear();
    gList.clear();
    bList.clear();
//    double srodekZ;
    double x,y,z, r,g,b, tmpx; // RGB = [0.0, 1.0]
    double a,c, alfa,beta,gama;
    int fov, fovh;
    int stepCount;
    int angleStep;
    QStringList info;
    info.clear();



    QFile *f = new QFile(dirPath + "/info.txt");
    if(!f->open(QIODevice::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Generowanie punktów", "Nie udało się wczytać pliku z ustawieniami!");
        return;
    }
    QTextStream in(f);
    while(!in.atEnd())
    {
        info.append(in.readLine());
    }
    f->close();
    delete f;

    StopButton *sb = new StopButton();
    sb->start();


    beta = info.at(0).toDouble();
    fov = info.at(1).toInt();
    c = info.at(2).toDouble();
    stepCount = info.at(3).toInt();
    if(info.count() == 4)
        info.append("Brak opisu");


    QImage *imgColor, *imgPoints;
    emit newTable(ui->comboListDir->currentText(), info.at(3));


    for(int i=0; i<=stepCount; i++)
    {
        if(sb->isChecked())
            break;

        ui->progressBar->setFormat("Obraz " + QString::number(i+1) + "/" + QString::number(stepCount) + ": %p%");
        ui->progressBar->setValue(0);

        if(i+1<10)
            imgColor = new QImage(dirPath + "/00" + QString::number(i+1) + ".png");
        else
        {
            if(i+1<100)
                imgColor = new QImage(dirPath + "/0" + QString::number(i+1) + ".png");
            else
                imgColor = new QImage(dirPath + "/" + QString::number(i+1) + ".png");
        }
        imgPoints = new QImage(dirSkeletPath + "/" + QString::number(i+1) + ".png");


        int px;
        int k;
        QColor color;
        int maximum = imgPoints->height();
        ui->progressBar->setMaximum(maximum);
        for(int py=0; py<imgPoints->height(); py++)
        {

            k=0;
            px = -1;
            while(px==-1 && k<imgPoints->width())
            {
                color = imgPoints->pixel(k, py);
                if(color.red() == 255)
                    px = k;
                k++;
            }

            if(px != -1)
            {
                color = imgColor->pixel(px, py);
                r = (double)color.red()/(double)255;
                g = (double)color.green()/(double)255;
                b = (double)color.blue()/(double)255;


                if(px<=imgPoints->width()/2)
                    alfa = (((double)imgPoints->width()/(double)2-(double)px)*(double)fov/(double)2)/(double)imgPoints->width()/(double)2;
                else
                    alfa = (((double)px-(double)imgPoints->width()/(double)2)*(double)fov/(double)2)/(double)imgPoints->width()/(double)2;
                alfa = 90 - alfa;

                gama = 180 - alfa-beta;

//                srodekZ = c * tan(beta*3.14159265359/180);
                a = c * sin(alfa*M_PI/180) / sin(gama*M_PI/180);

                x = a * cos(alfa*M_PI/180);
                if(px >= imgPoints->width()/2)
                    x = -x;
//                z = a * sin(alfa*3.14159265359/180);
//                z = srodekZ-z;
                z = x / tan((90-beta)*M_PI/180);
//                if(px <= imgPoints->width()/2)
//                    z = -z;

                if(z >= -2)
                {
                    fovh = (double)fov * ((double)imgPoints->height()/(double)imgPoints->width());
                    if(py <= imgPoints->height()/2)
                        alfa = (((double)imgPoints->height()/(double)2-(double)py)*(double)fovh/(double)2)/(double)imgPoints->height()/(double)2;
                    else
                        alfa = (((double)py-(double)imgPoints->height()/(double)2)*(double)fovh/(double)2)/(double)imgPoints->height()/(double)2;

                    y = tan(alfa*M_PI/180) * a;
                    if(py > imgPoints->height()/2)
                        y = -y;

                    angleStep = 360/stepCount * -i;
                    tmpx = x*cos(angleStep*M_PI/180) - z*sin(angleStep*M_PI/180);
                    z = x*sin(angleStep*M_PI/180) + z*cos(angleStep*M_PI/180);
                    x = tmpx;

    //                x = x*cos(angleStep*3.14159265359/180) + z*sin(angleStep*3.14159265359/180);
    //                z = -x*sin(angleStep*3.14159265359/180) + z*cos(angleStep*3.14159265359/180);

    //                przerabiac = false;
    //                emit newPoint(ui->comboListDir->currentText(), i+1, x,y,z, r,g,b);
    //                while(!przerabiac)
    //                {
    //                    QTime dieTime = QTime::currentTime().addMSecs(10);
    //                    while( QTime::currentTime() < dieTime )
    //                        QCoreApplication::processEvents(QEventLoop::AllEvents);
    //                }

                    idList.append(i+1);
                    xList.append(x);
                    yList.append(y);
                    zList.append(z);
                    rList.append(r);
                    gList.append(g);
                    bList.append(b);
                }
            }
//            if(py%5 == 0)
                ui->progressBar->setValue(ui->progressBar->value()+1);
        }

        delete imgColor;
        delete imgPoints;
    }

    ui->progressBar->setFormat("Zapisuję do bazy danych... Proszę czekać...");
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(100);
    przerabiac = false;
    emit newPoint(ui->comboListDir->currentText(), idList, xList,yList,zList, rList,gList,bList);
    while(!przerabiac)
    {
        QTime dieTime = QTime::currentTime().addMSecs(10);
        while( QTime::currentTime() < dieTime )
            QCoreApplication::processEvents(QEventLoop::AllEvents);
    }

    ui->progressBar->setFormat("Gotowe!  %p%");
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(100);

    delete sb;
}



void PhotoWidget::showSkieletDetail()
{
    if(ui->buttonShowSkieletDetal->arrowType() == Qt::DownArrow)
    {
        ui->buttonShowSkieletDetal->setArrowType(Qt::UpArrow);
        ui->frameSkieletDetail->setVisible(true);
    }
    else
    {
        ui->buttonShowSkieletDetal->setArrowType(Qt::DownArrow);
        ui->frameSkieletDetail->setVisible(false);
    }
}
void PhotoWidget::changeSliderSkieletValue()
{
    ui->labelSliderSkieletValue->setText(QString::number((double)ui->sliderSkieletSegm->value()/10));
}
void PhotoWidget::showSegmentation()
{
    QImage *img;
    QImage *imgl;
    QImage *tmp;

    QColor *color;
    QColor *c1;
    QColor *c2;

    if(currentPhotoNumber%2 == 1)
        currentPhotoNumber--;

    if(listPhoto.isEmpty())
        return;

    showPhoto(listPhoto.at(currentPhotoNumber)->path);
    img = new QImage(currentPhotoPath);

    showPhoto(listPhoto.at(currentPhotoNumber+1)->path);
    imgl = new QImage(currentPhotoPath);

    tmp = new QImage(img->width(), img->height(), QImage::Format_RGB888);


    color = new QColor;
    c1 = new QColor;
    c2 = new QColor;
    int value;
    int maxValue = 0;
    int minValue = 40000;
    int r,g,b;

    ui->progressBar->setMaximum(img->width()/10);
    ui->progressBar->setValue(0);

    ui->progressBar->setFormat("Różnica obrazów:  %p%");
    for(int i=0; i<img->width(); i++)
    {
        for(int j=0; j<img->height(); j++)
        {
            c1->setRgb(imgl->pixel(i,j));
            c2->setRgb(img->pixel(i,j));

            if(i > img->width()*((double)ui->sliderLaserEnd->value()/1000))
                value = 0;
            else
            {
                if(i < img->width()*((double)ui->sliderLaserBegin->value()/1000))
                    value = 0;
                else
                {
                    value = c1->red()-c2->red();
                    if(value < 0)
                        value = 0;

                    if(value > maxValue)
                        maxValue = value;
                    if(value < minValue)
                        minValue = value;
                }
            }


            color->setRgb(value, value, value);
            r = color->red();
            g = color->green();
            b = color->blue();

            tmp->setPixel(i, j, qRgb(r,g,b));
        }
        if(i%10 == 0)
            ui->progressBar->setValue(ui->progressBar->value()+1);
    }
    currentPhoto->setPixmap(QPixmap::fromImage(*tmp).scaled(currentPhoto->width()-3, currentPhoto->height()-3,
                                                           Qt::KeepAspectRatio, Qt::FastTransformation));
    if(ui->radioSubtraction->isChecked())
        return;

    ui->progressBar->setValue(0);
    ui->progressBar->setFormat("Obraz binarny:  %p%");
    int avr = (minValue+maxValue)/ui->labelSliderSkieletValue->text().toDouble();
    for(int i=0; i<img->width(); i++)
    {
        for(int j=0; j<img->height(); j++)
        {
            c1->setRgb(tmp->pixel(i,j));

            value = c1->red();

            if(value > avr)
                value = 255;
            else
                value = 0;


            color->setRgb(value, value, value);
            r = color->red();
            g = color->green();
            b = color->blue();

            tmp->setPixel(i, j, qRgb(r,g,b));
        }
        if(i%10 == 0)
            ui->progressBar->setValue(ui->progressBar->value()+1);
    }
    currentPhoto->setPixmap(QPixmap::fromImage(*tmp).scaled(currentPhoto->width()-3, currentPhoto->height()-3,
                                                           Qt::KeepAspectRatio, Qt::FastTransformation));
    if(ui->radioBinary->isChecked())
        return;

    ui->progressBar->setMaximum(img->height());
    ui->progressBar->setValue(0);
    ui->progressBar->setFormat("Optymalizacja:  %p%");
    bool min;
    for(int i=0; i<img->height(); i++)
    {
        min = false;
        minValue = maxValue = 0;
        for(int j=0; j<img->width(); j++)
        {
            c1->setRgb(tmp->pixel(j,i));
            if(!min)
            {
                if(c1->red()==255)
                {
                    min = true;
                    minValue = j;
                    maxValue = j;
                }
            }
            else
            {
//                if(c1->red()==255)
//                    maxValue = j;
                if(c1->red()==255)
                {
                    if(j-maxValue < ui->spinMaxSpace->value())
                        maxValue = j;
                }
            }
            tmp->setPixel(j,i,qRgb(0,0,0));
        }

        avr = (minValue+maxValue)/2;
        if(avr!=0)
            tmp->setPixel(avr, i, qRgb(255,255,255));
        ui->progressBar->setValue(ui->progressBar->value()+1);
    }

    currentPhoto->setPixmap(QPixmap::fromImage(*tmp).scaled(currentPhoto->width()-3, currentPhoto->height()-3,
                                                           Qt::KeepAspectRatio, Qt::FastTransformation));


    delete img;
    delete imgl;
    delete tmp;
    delete color;
    delete c1;
    delete c2;
}

void PhotoWidget::showLinesBegin()
{
    if(ui->sliderLaserBegin->value() > ui->sliderLaserEnd->value())
        ui->sliderLaserEnd->setValue(ui->sliderLaserBegin->value());

    showPhoto(currentPhotoPath);
}
void PhotoWidget::showLinesEnd()
{
    if(ui->sliderLaserEnd->value() < ui->sliderLaserBegin->value())
        ui->sliderLaserBegin->setValue(ui->sliderLaserEnd->value());

    showPhoto(currentPhotoPath);
}









Label::Label(const QString &text, QWidget *parent):QLabel(parent)
{
    this->setText(text);
    path = "";
    isClicked = false;
}
void Label::mousePressEvent ( QMouseEvent * event )
{
    event->ignore();
    isClicked = true;
    emit clicked();
}
