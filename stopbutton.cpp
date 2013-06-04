#include "stopbutton.h"

StopButton::StopButton() : QThread()
{
    button = new QPushButton;
    button->setText("Przerwij");
    button->setCheckable(true);
    button->setChecked(false);
}

void StopButton::run()
{
    button->show();
}

bool StopButton::isChecked()
{
    return button->isChecked();
}
