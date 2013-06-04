#ifndef SCANNER_H
#define SCANNER_H

#include <QWidget>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/mat.hpp>
#include <QThread>
#include <QLabel>
#include <QScrollArea>
#include <QMovie>
#include <QFile>
#include <QTimer>
#include <QTime>
#include <QComboBox>

#include <QProcess>
#include <QMessageBox>
#include <QListWidget>
#include <QDoubleSpinBox>

#include <QInputDialog>
#include <QToolButton>
#include <QPushButton>
#include <QProgressBar>
#include <QDir>
#include <QTextStream>
#include <QColor>
#include <QRgb>
#include <QThread>
namespace Ui {
class Scanner;
}

class CameraView : public QThread
{
    Q_OBJECT

public:
    CameraView();
    ~CameraView();

    QLabel *camera;
    void sleep(int);
    void run();                         // wątek kamery
    QImage getImage();

    bool isCameraVisible;

    int width;                          // szerokość obrazu
    int height;                         // wysokość pobranego obrazu


private:
    int cameraNumber;                   // która kamera
    int fps;
    bool isActive;                      // czy kamera jest wlaczona

    CvCapture *capture;
    IplImage *frame;
    cv::Mat image;                      // surowy obraz
    QImage qimg;                        // obraz jako QImage

public slots:
    void setCameraNumber(QString camPath);
    void setCameraDefinition(int i);

    void cameraStart();
    void cameraStop();
    void setFps(int x);
    void setCameraVisible(bool x){isCameraVisible = x;}

signals:
    void newImage();
    void isActiveChanged(bool);
};





class MCUConnect : public QThread
{
    Q_OBJECT

public:
    MCUConnect();
    ~MCUConnect();
    void run();

    void sendMessage(QString message);
    bool searchDevice();
    QProcess *reader;


private:
    bool connected;
    QString device;
    bool isMessageToSend;
    QString messageToSend;
    bool isReaderMessage;

private slots:
    void onReaderMessage();

signals:
    void connectChanged(bool);
    void newMessage();
};









class Scanner : public QWidget
{
    Q_OBJECT

public:
    explicit Scanner(QWidget *parent = 0);
    ~Scanner();
    void sleep(int);

//CAMERA:
    CameraView *view;
    bool saveImage(QString path, QString name);
//**********

//MCU:
    MCUConnect *mcuConnect;
//**********

private:
    Ui::Scanner *ui;

private slots:
//CAMERA:
    void refreshCam();
    void setImage();
//**********

//MCU:
    void sliderValueChanged(int);
    void onNewMessage();

    void setLaserState(bool);
    void setLaserAngleToZero();
    void turnLaserLeft();
    void turnLaserRight();
//**********

    void Scan();
};










#endif // SCANNER_H
