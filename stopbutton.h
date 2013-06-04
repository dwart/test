#ifndef STOPBUTTON_H
#define STOPBUTTON_H

#include <QObject>
#include <QThread>
#include <QPushButton>

class StopButton : public QThread
{
    Q_OBJECT
public:
    StopButton();
    void run();

    bool isChecked();
private:
    QPushButton *button;
    bool visible;

signals:
    

public slots:
};

#endif // STOPBUTTON_H
