#include "scanner.h"
#include "ui_scanner.h"

Scanner::Scanner(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Scanner)
{
    ui->setupUi(this);

//  KAMERA *******************************************************************
    view = new CameraView;
    view->camera = new QLabel(ui->scrollCameraArea);
    view->camera->setFrameStyle(1);
    view->camera->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    view->camera->setWordWrap(true);
    ui->scrollCameraArea->setWidget(view->camera);
    ui->scrollCameraArea->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    ui->inProgress->setVisible(false);

    connect(ui->buttonCameraRefresh, SIGNAL(clicked()), this, SLOT(refreshCam()));
    refreshCam();

    connect(ui->comboCamera, SIGNAL(currentIndexChanged(QString)), view, SLOT(setCameraNumber(QString)));
    connect(ui->comboCameraDefinition, SIGNAL(currentIndexChanged(int)), view, SLOT(setCameraDefinition(int)));

    connect(ui->buttonCameraStart, SIGNAL(clicked()), view, SLOT(cameraStart()));
    connect(ui->buttonCameraStop, SIGNAL(clicked()), view, SLOT(cameraStop()));
    connect(view, SIGNAL(newImage()), this, SLOT(setImage()));
    connect(ui->spinCameraFps, SIGNAL(valueChanged(int)), view, SLOT(setFps(int)));
    connect(view, SIGNAL(isActiveChanged(bool)), ui->inProgress, SLOT(setVisible(bool)));

    QMovie *movie = new QMovie(":/Icons/load.gif");
    ui->inProgress->setMovie(movie);
    movie->start();

    ui->comboCameraDefinition->setCurrentIndex(1);
    ui->comboCameraDefinition->setCurrentIndex(0);
//  *************************************************************************

//  MCU *********************************************************************
    mcuConnect = new MCUConnect();
    connect(mcuConnect, SIGNAL(connectChanged(bool)), ui->buttonConnected, SLOT(setChecked(bool)));
    connect(ui->buttonConnected, SIGNAL(toggled(bool)), ui->frame, SLOT(setEnabled(bool)));
    connect(mcuConnect, SIGNAL(newMessage()), this, SLOT(onNewMessage()));
    mcuConnect->start();

    connect(ui->sliderScanAccurancy, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));

    connect(mcuConnect->reader, SIGNAL(readyRead()), this, SLOT(onNewMessage()));
    connect(mcuConnect->reader, SIGNAL(readyReadStandardError()), this, SLOT(onNewMessage()));
    connect(mcuConnect->reader, SIGNAL(readyReadStandardOutput()), this, SLOT(onNewMessage()));

    connect(ui->buttonLaser, SIGNAL(clicked(bool)), this, SLOT(setLaserState(bool)));
    connect(ui->buttonLaserZeroAngle, SIGNAL(clicked()), this, SLOT(setLaserAngleToZero()));
    connect(ui->buttonLaserLeft, SIGNAL(clicked()), this, SLOT(turnLaserLeft()));
    connect(ui->buttonLaserRight, SIGNAL(clicked()), this, SLOT(turnLaserRight()));
    //  *************************************************************************

    connect(ui->buttonScan, SIGNAL(clicked()), this, SLOT(Scan()));
}

Scanner::~Scanner()
{
//    QString s = ui->labelLaserAngle->text();
//    s = s.left(s.length()-1);
//    ui->spinLaserStep->setValue(s.toDouble());
//    turnLaserRight();

    delete view;
    delete mcuConnect;
    delete ui;
}

void Scanner::sleep(int ms)
{
    QTime dieTime = QTime::currentTime().addMSecs(ms);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents);
}

//  KAMERA *******************************************************************
bool Scanner::saveImage(QString path, QString name)
{
    QImage img = view->getImage();
    QString text, textmini;

//    QColor c;
    img = img.rgbSwapped();
//    for(int i=0; i<img.width(); i++)
//    {
//        for(int j=0; j<img.height(); j++)
//        {
//             c.setRgb(img.pixel(i,j));
//             img.setPixel(i,j, qRgb(c.blue(), c.green(), c.red()));
//        }
//    }

    bool l;
    if(name[name.count()-1] == 'l')
    {
        l = true;
        name = name.left(name.length()-1);
    }
    else
        l = false;

    if(name.toInt() < 10)
        name = "00" + name;
    else
    {
        if(name.toInt() < 100)
            name = "0" + name;
    }

    if(l)
        name.append("l");

    if(path.right(1) == "/")
    {
        text = path + name + ".png";
        textmini = path + "mini/" + name + ".png";
    }
    else
    {
        text = path + "/" + name + ".png";
        textmini = path + "/mini/" + name + ".png";
    }

    if(img.save(text, "PNG"))
    {
        img = img.scaled(140,80, Qt::KeepAspectRatio, Qt::FastTransformation);
        if(img.save(textmini, "PNG"))
            return true;
    }

    return false;
}

void Scanner::refreshCam()
{
    QFile file;
    ui->comboCamera->clear();

    for(int i=0; i<10; i++)
    {
        file.setFileName("/dev/video" + QString::number(i));
        if(file.exists())
        {
            QString text = file.fileName();
            text.remove(0,5);
            ui->comboCamera->addItem(text);
        }
    }
}

void Scanner::setImage()
{
    QImage qimg = view->getImage();
    QColor c;

    qimg = qimg.rgbSwapped();
    if(ui->checkShowCentre->isChecked())
    {
        for(int j=0; j<qimg.height(); j++)
        {
            qimg.setPixel(qimg.width()/2,j, qRgb(255,0,0));
        }
    }

    switch(ui->comboCameraSize->currentIndex())
    {
    case 0:
        view->camera->setMinimumSize(view->width, view->height);
        view->camera->setMaximumSize(view->width, view->height);
        view->camera->setPixmap(QPixmap::fromImage(qimg));
        break;
    case 1:
        view->camera->setMinimumSize(view->width/2, view->height/2);
        view->camera->setMaximumSize(view->width/2, view->height/2);
        view->camera->setPixmap(QPixmap::fromImage(qimg).scaled(view->camera->width(), view->camera->height(),
                                                                  Qt::KeepAspectRatio, Qt::FastTransformation));
        break;
    case 2:
        view->camera->setMinimumSize(view->width/4, view->height/4);
        view->camera->setMaximumSize(view->width/4, view->height/4);
        view->camera->setPixmap(QPixmap::fromImage(qimg).scaled(view->camera->width(), view->camera->height(),
                                                                  Qt::KeepAspectRatio, Qt::FastTransformation));
        break;
    case 3:
        view->camera->setMinimumSize(view->width/10, view->height/10);
        view->camera->setMaximumSize(view->width/10, view->height/10);
        view->camera->setPixmap(QPixmap::fromImage(qimg).scaled(view->camera->width(), view->camera->height(),
                                                                  Qt::KeepAspectRatio, Qt::FastTransformation));
        break;
    }
}
//  *************************************************************************


//  MCU *********************************************************************
void Scanner::sliderValueChanged(int x)
{
    switch(x)
    {
    case 0:
        ui->labelSliderPos->setText("8");
        break;
    case 1:
        ui->labelSliderPos->setText("10");
        break;
    case 2:
//        ui->labelSliderPos->setText("15");
        ui->labelSliderPos->setText("16");
        break;
    case 3:
        ui->labelSliderPos->setText("20");
        break;
    case 4:
        ui->labelSliderPos->setText("30");
        break;
    case 5:
        ui->labelSliderPos->setText("36");
        break;
    case 6:
        ui->labelSliderPos->setText("40");
        break;
    case 7:
        ui->labelSliderPos->setText("45");
        break;
    case 8:
        ui->labelSliderPos->setText("60");
        break;
    case 9:
        ui->labelSliderPos->setText("72");
        break;
    case 10:
        ui->labelSliderPos->setText("90");
        break;
    case 11:
        ui->labelSliderPos->setText("120");
        break;
    case 12:
        ui->labelSliderPos->setText("180");
        break;
    case 13:
        ui->labelSliderPos->setText("360");
        break;
    }
}

void Scanner::onNewMessage()
{
    QString std, err, all;
    std = err = all = "";

    QByteArray byteArray = mcuConnect->reader->readAllStandardOutput();
    QStringList strLines = QString(byteArray).split("\n");

    foreach (QString line, strLines)
    {
        if(line != "" && line != "\n")
        {
            std = line;// + "\n");
        }
    }

    byteArray = mcuConnect->reader->readAllStandardError();
    strLines = QString(byteArray).split("\n");

    foreach (QString line, strLines)
    {
        if(line != "" && line != "\n")
        {
            err = line;// + "\n");
        }
    }
    byteArray = mcuConnect->reader->readAll();
    strLines = QString(byteArray).split("\n");
    foreach (QString line, strLines)
    {
        if(line != "" && line != "\n")
        {
            all = line;// + "\n");
        }
    }

    QString readerMessage = "";
    if(std != "")
        readerMessage += std;//"Std:\n" + std + "\n";
    if(err != "")
        readerMessage += "ERROR:\n" + err + "\n";
    if(all != "")
        readerMessage += "All:\n" + all + "\n";
    if(readerMessage.isEmpty())
        return;

//    QMessageBox::information(this, "MCU message", readerMessage);
    if(readerMessage.length() >= 2)
    {
        if(readerMessage[0] == 'l' && readerMessage[1] == '0')
            readerMessage = "Wyłączono laser";
        if(readerMessage[0] == 'l' && readerMessage[1] == '1')
            readerMessage = "Włączono laser";
    }
    if(readerMessage[0] == '0')
    {
        readerMessage = "Wyzerowano pozycje lasera";
    }

    ui->listLog->addItem("MCU Message:\n" + readerMessage);
    ui->listLog->setCurrentRow(ui->listLog->count()-1);
}

void Scanner::setLaserState(bool x)
{
    if(x)
        mcuConnect->sendMessage("l1");
    else
        mcuConnect->sendMessage("l0");
}

void Scanner::setLaserAngleToZero()
{
    ui->labelLaserAngle->setText("0°");
    mcuConnect->sendMessage("s1r");
}


void Scanner::turnLaserLeft()
{
    int steps = ui->spinLaserStep->value() / 0.1875;
    QString message = "s11" + QString::number(steps) + ".";
    mcuConnect->sendMessage(message);

    QString text = ui->labelLaserAngle->text();
    text = text.left(text.length()-1);
    double x = text.toDouble();
    x += ui->spinLaserStep->value();
    ui->labelLaserAngle->setText(QString::number(x) + "°");
}
void Scanner::turnLaserRight()
{
    int steps = ui->spinLaserStep->value() / 0.1875;
    QString message = "s10" + QString::number(steps) + ".";
    mcuConnect->sendMessage(message);

    QString text = ui->labelLaserAngle->text();
    text = text.left(text.length()-1);
    double x = text.toDouble();
    x -= ui->spinLaserStep->value();
    ui->labelLaserAngle->setText(QString::number(x) + "°");
}

//  *************************************************************************


















void Scanner::Scan()
{
    if(!ui->buttonConnected->isChecked())
    {
        QMessageBox::warning(this, "Połączenie skanera", "Skaner jest odłączony!\nPodepnij skaner i spróbuj ponownie.");
        return;
    }
    if(!ui->buttonLaser->isChecked())
    {
        QMessageBox::warning(this, "Laser", "Laser jest wyłączony!\nWłącz laser i spróbuj ponownie.");
        return;
    }
    if(ui->inProgress->isHidden())
    {
        QMessageBox::warning(this, "Kamera", "Kamera jest wyłączona!\nWłącz kamerę i spróbuj ponownie.");
        return;
    }

    QString name;
    QString description;

    name = QInputDialog::getText(this, "Nazwa", "Podaj nazwę obiektu:");
    description = QInputDialog::getText(this, "Opis", "Podaj opis obiektu:");


    QString dirName = "/home/dwart/" + name;
    QDir dir, dirmini;
    dir.setPath(dirName);
    if(dir.exists())
    {
        QMessageBox::information(this, "Katalog", "Katalog " + name + " już istnieje!");
        return;
    }
    else
    {
        dir.mkdir(dirName);
        dir.mkdir(dirName + "/mini");
    }


    int x = QMessageBox::question(this, "Kontynuować?", "Skanowanie obiektu " + name + ":\n(" + description + ")\nDokładność skanowania: " +
                                  ui->labelSliderPos->text() + "\nRozdzielczość: " + ui->comboCameraDefinition->currentText() +
                                  "\n\nKontynuować skanowanie?", QMessageBox::Ok, QMessageBox::Cancel);
    if(x == QMessageBox::Cancel)
        return;

    ui->progressBar->setMaximum(ui->labelSliderPos->text().toInt()*2);
    ui->progressBar->setValue(0);
    bool save = false;
    int count = 0;

    ui->checkShowCentre->setChecked(false);
    for(int i=0; i<ui->labelSliderPos->text().toInt(); i++)
    {
        ui->listLog->addItem("Skanowanie obrazów " + QString::number(i+1) + "/" + ui->labelSliderPos->text() + ":");
        setLaserState(true);
        sleep(400);
        while(!save)
        {
            save = saveImage(dirName, QString::number(i+1) + "l");
            count++;
            if(count >= 10)
            {
                QMessageBox::warning(this, "Zapis", "Problem z zapisem obrazów!\nPrzerywam skanowanie!");
                return;
            }
        }
        count = 0;
        save = false;
        ui->progressBar->setValue(ui->progressBar->value()+1);

        setLaserState(false);
        sleep(400);
        while(!save)
        {
            save = saveImage(dirName, QString::number(i+1));
            count++;
            if(count >= 10)
            {
                QMessageBox::warning(this, "Zapis", "Problem z zapisem obrazów!\nPrzerywam skanowanie!");
                return;
            }
        }
        count = 0;
        save = false;
        ui->progressBar->setValue(ui->progressBar->value()+1);
        sleep(400);

        if(ui->checkHandRotate->isChecked())
        {
            setLaserState(true);
            QMessageBox::information(this, "Obrót", "Obróć obiekt o " + QString::number(360/ui->labelSliderPos->text().toInt()) + "°");
        }
    }
    ui->listLog->addItem("Zapisuje ustawienia pliku...");
    QString text = dirName + "/info.txt";
    QFile plik(text);
    if(plik.open(QFile::WriteOnly | QFile::Text))
    {
         QTextStream out(&plik);
         text = ui->labelLaserAngle->text();
         text = text.left(text.length()-1);
         text += "\n" + QString::number(ui->spinCameraFov->value());
         text += "\n" + QString::number(ui->spinCameraDistance->value());
         text += "\n" + ui->labelSliderPos->text();
         text += "\n" + description;
         out << text;
    }
    plik.close();
    ui->listLog->addItem("OK");
    QMessageBox::information(this, "Skanowanie", "Skanowanie zakończone sukcesem!");
}






















// ************************************************************************************
// ************************************** MCUCONNECT **********************************
// ************************************************************************************


MCUConnect::MCUConnect() : QThread()
{
    connected = false;
    isMessageToSend = false;
    messageToSend = "";
    isReaderMessage = false;
    reader = new QProcess(this);
    searchDevice();
    connect(reader, SIGNAL(readyRead()), this, SLOT(onReaderMessage()));
    connect(reader, SIGNAL(readyReadStandardError()), this, SLOT(onReaderMessage()));
    connect(reader, SIGNAL(readyReadStandardOutput()), this, SLOT(onReaderMessage()));
}
MCUConnect::~MCUConnect()
{
    delete reader;
}

void MCUConnect::run()
{
    QFile file;
    int i = 0;
    while(true)
    {
        if(isMessageToSend)
        {
            system(messageToSend.toLatin1());

            messageToSend = "";
            isMessageToSend = false;
        }
        if(i==0)
        {
            if(connected)
            {
                file.setFileName(device);
                if(file.exists())
                {
                    connected = true;
                }
                else
                {
                    connected = false;
                    emit connectChanged(false);
                }
            }
            else
            {
                if(searchDevice())
                {
                    connected = true;
                    emit connectChanged(true);
                }
                else
                    connected = false;
            }
        }

        if(isReaderMessage)
        {
            isReaderMessage = false;
            emit newMessage();
        }
        if(i==50) i=0;

        i++;
        QThread::msleep(100);
    }
}
bool MCUConnect::searchDevice()
{
    QFile file;
    device = "/dev/ttyUSB";
    for(int i=0; i<10; i++)
    {
        file.setFileName("/dev/ttyUSB" + QString::number(i));
        if(file.exists())
        {
            device += QString::number(i);
            QString polecenie = "stty -F " + device + " 9600 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts";
            system(polecenie.toLatin1());
            polecenie = "cat " + device;
            reader->start(polecenie);
            return true;
        }
    }
    return false;
}

void MCUConnect::sendMessage(QString message)
{
    messageToSend = "echo -n '" + message + "' > " + device;
    isMessageToSend = true;
}

void MCUConnect::onReaderMessage()
{
    isReaderMessage = true;
}





// ************************************************************************************
// ************************************** CAMERAVIEW **********************************
// ************************************************************************************

CameraView::CameraView() : QThread()
{
    camera = new QLabel;
    camera->setText("Kamera");
    camera->setAlignment(Qt::AlignCenter);
    camera->setFrameStyle(1);

    cameraNumber = 0;
    width = 640;
    height = 480;
    fps = 30;
    isActive = false;
    capture = NULL;
}
CameraView::~CameraView()
{
    if(isActive)
        cameraStop();
    delete camera;
}

void CameraView::sleep(int ms)
{
    QTime dieTime = QTime::currentTime().addMSecs(ms);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents);
}

void CameraView::run()
{
    int ms = 1000 / fps;
    if(capture != NULL)
        cvReleaseCapture(&capture);
    capture = cvCreateCameraCapture(cameraNumber);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, (double)width);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, (double)height);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FPS, (double)fps);

    while(isActive)
    {
        frame = cvQueryFrame(capture);
        if(!frame)
        {
            camera->setText("Błąd pobrania obrazu");
            cameraStop();
        }
        image = cv::Mat(frame,false);
//        cv::cvtColor(image, image, CV_BGR2RGB);

        qimg = QImage((const uchar*) image.data, image.cols, image.rows, QImage::Format_RGB888);

        emit newImage();
        sleep(ms);
    }
}


void CameraView::setCameraNumber(QString camPath)
{
    bool isA = isActive;

    if(isActive)
        cameraStop();

    camPath = camPath.right(1);
    cameraNumber = camPath.toInt();

    if(isA)
        cameraStart();
}

void CameraView::setCameraDefinition(int i)
{
    bool isA = isActive;
    if(isActive)
        cameraStop();

    switch(i)
    {
    case 0:
        width = 640;
        height = 480;
        camera->setMinimumSize(640,480);
        camera->setMaximumSize(640,480);
        break;
    case 1:
        width = 854;
        height = 480;
        camera->setMinimumSize(854,480);
        camera->setMaximumSize(854,480);
        break;
    case 2:
        width = 960;
        height = 720;
        camera->setMinimumSize(960,720);
        camera->setMaximumSize(960,720);
        break;
    case 3:
        width = 1280;
        height = 720;
        camera->setMinimumSize(1280,720);
        camera->setMaximumSize(1280,720);
        break;
    }

//    switch(ui->comboCameraSize->currentIndex())
//    {
//    case 0:
//        break;
//    case 1:
//        camera->setMinimumSize(width/2, height/2);
//        camera->setMaximumSize(width/2, height/2);
//        break;
//    case 2:
//        camera->setMinimumSize(width/4, height/4);
//        camera->setMaximumSize(width/4, height/4);
//        break;
//    case 3:
//        camera->setMinimumSize(width/10, height/10);
//        camera->setMaximumSize(width/10, height/10);
//        break;
//    }

    if(isA)
        cameraStart();
}

void CameraView::cameraStart()
{
    if(isActive)
        return;

    camera->setText("Czekam na obraz");

    isActive = true;
    this->start();
    emit isActiveChanged(true);
}
void CameraView::cameraStop()
{
    if(!isActive)
        return;

    isActive = false;
    sleep(200);
    cvReleaseCapture(&capture);
//    camera->setPixmap(NULL);
    camera->setText("Kamera");
    emit isActiveChanged(false);
}

QImage CameraView::getImage()
{
    return qimg;
}

void CameraView::setFps(int x)
{
    fps = x;
}

// ************************************************************************************
