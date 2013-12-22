#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QDir>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void readSettings();
    void writeSettings();
    void setWorkingDirectory(QString dir);
    void openWorkingDirectoryPrompt();

signals:
    void workingDirectoryChanged(QString dir);

protected:
     void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;

    QDir workingDirectory;
};

#endif // MAINWINDOW_HPP
