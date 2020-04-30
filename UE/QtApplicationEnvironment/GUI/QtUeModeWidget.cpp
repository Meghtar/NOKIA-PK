#include "QtUeModeWidget.hpp"

namespace ue
{

QtUeModeWidget::QtUeModeWidget(QtPhoneNumberEdit &phoneNumberEdit, QtStackedWidget &stackedWidget)
    : phoneNumberEdit(phoneNumberEdit),
      stackedWidget(stackedWidget)
{
    setLayout(&mainLayout);
    connect(this,SIGNAL(activateSignal()),this,SLOT(activateSlot()));
}

void QtUeModeWidget::addChildWidget(QWidget *childWidget)
{
    mainLayout.addWidget(childWidget);
}

void QtUeModeWidget::activateWithPhoneNumberEditEnabled()
{
    phoneNumberEdit.enable();
    activateThisWidget();
}

void QtUeModeWidget::activateWithPhoneNumberEditDisabled()
{
    clearPhoneNumber(); // TODO: i'm not sure if we should clear phone Number
    phoneNumberEdit.disable();
    activateThisWidget();
}

PhoneNumber QtUeModeWidget::getPhoneNumber() const
{
    return phoneNumberEdit.getPhoneNumber();
}

void QtUeModeWidget::clearPhoneNumber()
{
    phoneNumberEdit.setText("");
}

void QtUeModeWidget::activateThisWidget()
{
    stackedWidget.setCurrentIndex(stackedWidgetIndex);
}

void QtUeModeWidget::init()
{
    stackedWidgetIndex = stackedWidget.addModeWidget(this);
}

void QtUeModeWidget::activate()
{
    emit activateSignal();
}

}
