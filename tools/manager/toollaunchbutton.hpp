#ifndef TOOLLAUNCHBUTTON_HPP
#define TOOLLAUNCHBUTTON_HPP

#include <QPushButton>

class ToolLaunchButton : public QPushButton
{
    Q_OBJECT

public:
    explicit ToolLaunchButton(QWidget * parent = 0);

protected:
    virtual void enterEvent( QEvent* e );
    virtual void leaveEvent( QEvent* e );

signals:
    void mouseEntered(QString);
    void mouseLeft();

public slots:
    void setToolDescription(const QString& description);

private:
    QString description;
};

#endif // TOOLLAUNCHBUTTON_HPP
