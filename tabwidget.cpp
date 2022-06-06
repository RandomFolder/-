#include "tabwidget.h"

TabWidget::TabWidget(QWidget * parent)
    : QTabWidget(parent)
{
    this->setTabPosition(QTabWidget::North);
    this->setTabsClosable(true);
    this->setMovable(true);
}

TabWidget::~TabWidget()
{
    //  -\_(-.-)_/-
}
