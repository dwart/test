#ifndef PHOTOWIDGET_H
#define PHOTOWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QDir>
#include <QToolButton>
#include <QStringList>
#include <QComboBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QFrame>
#include <QProgressBar>
#include <QImage>
#include <QPixmap>
#include <QRgb>
#include <QColor>
#include <QMessageBox>
#include <cmath>
#include <QFile>
#include <QTextStream>
#include <QTime>
#include <QRadioButton>
#include <QCheckBox>
#include <QPushButton>

#include <stopbutton.h>

class Label : public QLabel
{
    Q_OBJECT
public:
    Label( const QString & text, QWidget * parent = 0 );
    ~Label(){}

    bool isClicked;
    QString path;

signals:
    void clicked();

protected:
    void mousePressEvent ( QMouseEvent * event ) ;
};





namespace Ui {
class PhotoWidget;
}

class PhotoWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PhotoWidget(QWidget *parent = 0);
    ~PhotoWidget();

    QDir *directory;
    QString currentPhotoPath;
    int currentPhotoNumber;

    Label *currentPhoto;
    QList<Label*> listPhoto;

    QWidget *frameList;
    QHBoxLayout *layoutList;

    bool przerabiac;

    QList<int> idList;
    QList<double> xList;
    QList<double> yList;
    QList<double> zList;
    QList<double> rList;
    QList<double> gList;
    QList<double> bList;

    bool makeMiniPhoto(QDir *dir, QStringList list);


private:
    Ui::PhotoWidget *ui;
    QPushButton *buttonStop;

public slots:
    void loadPhotos();

private slots:
    void initialize();

    void openDir();
    void loadDirs();
    void showPhoto(QString name);
    void listPhotoClicked();

    void skeletonization();
    void improvePics();
    void generatePoints();

    void showSkieletDetail();
    void changeSliderSkieletValue();
    void showSegmentation();
    void showLinesBegin();
    void showLinesEnd();

signals:
    void newTable(QString nazwa, QString opis);
    void newPoint(QString nazwa, QList<int> id, QList<double> x, QList<double> y, QList<double> z, QList<double> r, QList<double> g, QList<double> b);
};

#endif // PHOTOWIDGET_H
