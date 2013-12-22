#include "toollaunchbutton.hpp"

ToolLaunchButton::ToolLaunchButton(QWidget * parent) :
    QPushButton(parent)
{
}


void ToolLaunchButton::enterEvent(QEvent *e)
{
    emit mouseEntered(description);
}

void ToolLaunchButton::leaveEvent(QEvent *e)
{
    emit mouseLeft();
}

void ToolLaunchButton::setToolDescription(const QString& description)
{
    this->description = description;
}
