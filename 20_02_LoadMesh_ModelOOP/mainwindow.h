#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public:

private:
    Ui::MainWindow *ui_;
    
protected slots:
    void slotWireframeView();
    void slotEnvSetting();
    void slotLoadModel();
    void slotShowLights();

};
#endif // MAINWINDOW_H
