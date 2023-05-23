#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "depthtestdialog.h"
#include "frambufferdialog.h"
#include "shownormaldg.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);
    setCentralWidget(ui_->openGLWidget);
    connect(ui_->actionWireFrameView, SIGNAL(triggered()), this, SLOT(slotWireframeView()));
    connect(ui_->actionEnvSetting, SIGNAL(triggered()), this, SLOT(slotEnvSetting()));
    connect(ui_->actionLoadModel, SIGNAL(triggered()), this, SLOT(slotLoadModel()));
    connect(ui_->actionShowLights, SIGNAL(triggered()), this, SLOT(slotShowLights()));
    connect(ui_->actionDrawCube, SIGNAL(triggered()), this, SLOT(slotDrawCube()));
    connect(ui_->actionDrawPlane, SIGNAL(triggered()), this, SLOT(slotDrawPlane()));
    connect(ui_->actionDepthTest, SIGNAL(triggered()), this, SLOT(slotDepthTestSetting()));
    connect(ui_->actionDepthTestShader, SIGNAL(triggered()), this, SLOT(slotZbufferShader()));
    connect(ui_->actionStencil, SIGNAL(triggered()), this, SLOT(slotStenCil()));
    connect(ui_->actionDrawVegetation, SIGNAL(triggered()), this, SLOT(slotDrawVegetation()));
    connect(ui_->actionDrawWindows, SIGNAL(triggered()), this, SLOT(slotDrawWindows()));
    connect(ui_->actionDrawFramBuffer, SIGNAL(triggered()), this, SLOT(slotDrawFramBuffer()));
    connect(ui_->actionSelectFrambufferType, SIGNAL(triggered()), this, SLOT(slotSelectFramBufferType()));
    connect(ui_->openGLWidget,&ModelLoadingOWgt::signalMousePickingPos,
            this, &MainWindow::slotMousePickingPos);
    connect(ui_->actionOpenSkyBox, SIGNAL(triggered()), this, SLOT(slotOpenSkyBox()));
    connect(ui_->actionReflectIonSkyBox, SIGNAL(triggered()), this, SLOT(slotReflectionSkyBox()));
    connect(ui_->actionReflractionSkyBox, SIGNAL(triggered()), this, SLOT(slotRefractionSkyBox()));
    connect(ui_->actionReflectTexSkyBox, SIGNAL(triggered()), this, SLOT(slotReflectTextureSkyBox()));
    connect(ui_->actionPoints, SIGNAL(triggered()), this, SLOT(slotPointsView()));
    connect(ui_->actionDrawHouse, SIGNAL(triggered()), this, SLOT(slotDrawHouse()));
    connect(ui_->actionDrawExplode, SIGNAL(triggered()), this, SLOT(slotDrawExplode()));
    connect(ui_->actionshowNormal, SIGNAL(triggered()), this, SLOT(slotShowNormal()));
    connect(ui_->actionDrawInstancing, SIGNAL(triggered()), this, SLOT(slotDrawInstancing()));
    connect(ui_->actionDrawPlanet, SIGNAL(triggered()), this, SLOT(slotDrawPlanets()));
    connect(ui_->actionDrawTriangle, SIGNAL(triggered()), this, SLOT(slotDrawTriangle()));
    connect(ui_->actionactionPointLight, SIGNAL(triggered()), this, SLOT(slotPointLight()));
    connect(ui_->actionactionDirLight, SIGNAL(triggered()), this, SLOT(slotDirLight()));
    connect(ui_->actionactionSPointLight, SIGNAL(triggered()), this, SLOT(slotSPointLight()));
    connect(ui_->actionTestGamma, SIGNAL(triggered()), this, SLOT(slotTestGamma()));
    connect(ui_->actionDepthMap, SIGNAL(triggered()), this, SLOT(slotDepthMap()));
    connect(ui_->actionNormalMap, SIGNAL(triggered()), this, SLOT(slotNormalMap()));


    connect(ui_->checkBox, SIGNAL(stateChanged(int)), ui_->openGLWidget, SLOT(slotHDR()));
    connect(ui_->doubleSpinBox, SIGNAL(valueChanged(double )), ui_->openGLWidget, SLOT(slotchangExpose(double )));
}

void MainWindow::slotWireframeView()
{
    ui_->openGLWidget->setWireframe(ui_->actionWireFrameView->isChecked());
}

MainWindow::~MainWindow()
{
    delete ui_;
}

void MainWindow::slotEnvSetting() {
    auto dlg = new EnvironmentSettingDialog(this);
    dlg->setEnvType(ui_->openGLWidget->getViewEnvType());
    if(dlg->exec()==QDialog::Accepted){
        ui_->openGLWidget->setEnvSettingType(dlg->getEnvType());
    }
    delete dlg;
}

void MainWindow::slotLoadModel() {
    QString str = QFileDialog::getOpenFileName(this,"选择模型文件","",
                                             "OBJ (*.obj);;PLY (*.ply);;FBX(*.fbx);;STL (*.stl);;ALL FILES( *.* ) ");
    ui_->openGLWidget->loadModel(str.toStdString());
}

void MainWindow::slotShowLights() {

    auto bShowLight = ui_->openGLWidget->isBShowLights();
    ui_->openGLWidget->setBShowLights(!bShowLight);
}

void MainWindow::slotDrawCube() {
    auto bDrawCube = ui_->openGLWidget->isBDrawCube();
    ui_->openGLWidget->setBDrawCube(!bDrawCube);
}

void MainWindow::slotDrawPlane() {
    auto bDrawPlane = ui_->openGLWidget->isBDrawPlane();
    ui_->openGLWidget->setBDrawPlane(!bDrawPlane);
}

void MainWindow::slotDepthTestSetting() {
    auto dlg = new DepthTestDialog(this);
    dlg->setDepthTestFunc(ui_->openGLWidget->getTestFuncType());
    if(dlg->exec()==QDialog::Accepted){
        ui_->openGLWidget->setTestFuncType(dlg->getDepthTestFunc());
    }
    delete dlg;
}

void MainWindow::slotZbufferShader() {
    auto bZBufferTest = ui_->openGLWidget->isBDepthTestShader();
    ui_->openGLWidget->setBDepthTestShader(!bZBufferTest);
}

void MainWindow::slotMousePickingPos(QVector3D pos) {
    ui_->statusbar->setStyleSheet("font: 14pt ");
    ui_->statusbar->showMessage(" 世界坐标:    X:"
                               +QString::number(pos.x(),'f', 2)
                               +" Y:"+QString::number(pos.y(),'f', 2)
                               +" Z:"+QString::number(pos.z(),'f', 2)
    );
}

void MainWindow::slotStenCil() {
    auto bStencil = ui_->actionStencil->isChecked();
    ui_->openGLWidget->setBStencil(bStencil);
}

void MainWindow::slotDrawVegetation() {
    auto bDrawVegetation = ui_->openGLWidget->isBDrawVegetation();
    ui_->openGLWidget->setBDrawVegetation(!bDrawVegetation);
}

void MainWindow::slotDrawWindows() {
    auto bDrawWindows = ui_->openGLWidget->isBDrawWindows();
    ui_->openGLWidget->setBDrawWindows(!bDrawWindows);
}

void MainWindow::slotDrawFramBuffer() {
    auto bDrawFramBuffer = ui_->openGLWidget->isBDrawFramBuffer();
    ui_->openGLWidget->setBDrawFramBuffer(!bDrawFramBuffer);
}

void MainWindow::slotSelectFramBufferType() {
    auto dlg = new FrambufferDialog(this);
    dlg->setFbType(ui_->openGLWidget->getFbType());
    if(dlg->exec()==QDialog::Accepted){
        ui_->openGLWidget->setFbtSettingType(dlg->getFbType());
    }
    delete dlg;
}

void MainWindow::slotOpenSkyBox() {
    ui_->openGLWidget->setBOpenSkyBox(ui_->actionOpenSkyBox->isChecked());
}

void MainWindow::slotReflectionSkyBox() {
    ui_->actionReflractionSkyBox->setChecked(false);
    ui_->actionReflectTexSkyBox->setChecked(false);
    ui_->openGLWidget->setBReflectionSkyBox(ui_->actionReflectIonSkyBox->isChecked());
}

void MainWindow::slotRefractionSkyBox() {
    ui_->actionReflectIonSkyBox->setChecked(false);
    ui_->actionReflectTexSkyBox->setChecked(false);
    ui_->openGLWidget->setBRefractionSkyBox(ui_->actionReflractionSkyBox->isChecked());
}

void MainWindow::slotReflectTextureSkyBox() {
    ui_->actionReflectIonSkyBox->setChecked(false);
    ui_->actionReflractionSkyBox->setChecked(false);
    ui_->openGLWidget->setBReflectTexturteSkyBox(ui_->actionReflectTexSkyBox->isChecked());
}

void MainWindow::slotPointsView() {
    ui_->openGLWidget->setPointRender(!ui_->openGLWidget->isBPoints());
}

void MainWindow::slotDrawHouse() {
    auto bDrawHouse = ui_->openGLWidget->isBDrawHouse();
    ui_->openGLWidget->setBDrawHouse(!bDrawHouse);
}

void MainWindow::slotDrawExplode() {
    auto bDrawExplode = ui_->openGLWidget->isBDrawExplode();
    ui_->openGLWidget->setBDrawExplode(!bDrawExplode);
}

void MainWindow::slotShowNormal() {
    ui_->openGLWidget->setShowNormalDgVisible(ui_->actionshowNormal->isChecked());
}

void MainWindow::slotDrawInstancing() {
    bool bDrawInstancing = ui_->openGLWidget->isBDrawInstancing();
    ui_->openGLWidget->setBDrawInstancing(!bDrawInstancing);
}

void MainWindow::slotDrawPlanets() {
    bool bDrawPlanets = ui_->openGLWidget->isBDrawPlanets();
    ui_->openGLWidget->setBDrawPlanets(!bDrawPlanets);
}

void MainWindow::slotDrawTriangle() {
    bool bDrawTriangle = ui_->openGLWidget->isBDrawTriangle();
    ui_->openGLWidget->setBDrawTriangle(!bDrawTriangle);
}

void MainWindow::slotPointLight() {
    ui_->openGLWidget->setPointLight(ui_->actionactionPointLight->isChecked());
}

void MainWindow::slotSPointLight() {
    ui_->openGLWidget->setSSPointLight(ui_->actionactionSPointLight->isChecked());
}

void MainWindow::slotDirLight() {
    ui_->openGLWidget->setDirectionalLight(ui_->actionactionDirLight->isChecked());
}

void MainWindow::slotTestGamma() {
    auto gamma = ui_->openGLWidget->isBTestGamma();
    ui_->openGLWidget->setBTestGamma(!gamma);
}

void MainWindow::slotDepthMap() {
    auto bDepthMap = ui_->openGLWidget->isBDrawDepthMap();
    ui_->openGLWidget->setBDrawDepthMap(!bDepthMap);
}

void MainWindow::slotNormalMap() {
    auto bNormalMap = ui_->openGLWidget->isBDrawNormalMapTest();
    ui_->openGLWidget->setBDrawNormalMapTest(!bNormalMap);

}
