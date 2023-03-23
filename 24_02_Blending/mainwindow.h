#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector3D>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected slots:
    void slotWireframeView();
    void slotEnvSetting();
    void slotDepthTestSetting();
    void slotLoadModel();
    void slotShowLights();
    void slotDrawCube();
    void slotDrawPlane();
    void slotDrawVegetation();
    void slotZbufferShader();
    void slotMousePickingPos(QVector3D pos);
    void slotStenCil();

private:
    Ui::MainWindow *ui_;

};
#endif // MAINWINDOW_H
