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
    void slotPointsView();
    void slotEnvSetting();
    void slotDepthTestSetting();
    void slotLoadModel();
    void slotShowLights();
    void slotDrawCube();
    void slotDrawPlane();
    void slotDrawVegetation();
    void slotDrawWindows();
    void slotDrawFramBuffer();
    void slotSelectFramBufferType();
    void slotZbufferShader();
    void slotMousePickingPos(QVector3D pos);
    void slotStenCil();
    void slotOpenSkyBox();
    void slotReflectionSkyBox();
    void slotRefractionSkyBox();
    void slotReflectTextureSkyBox();
    void slotDrawHouse();
    void slotDrawExplode();
    void slotShowNormal();
    void slotDrawInstancing();
    void slotDrawPlanets();
    void slotDrawTriangle();
    void slotPointLight();
    void slotSPointLight();
    void slotDirLight();
    void slotTestGamma();
    void slotDepthMap();
    void slotNormalMap();
    void slotDrawBloom();
    void slotDeferedRender();
    void slotDrawSSAO();

private:
    Ui::MainWindow *ui_;

};
#endif // MAINWINDOW_H
