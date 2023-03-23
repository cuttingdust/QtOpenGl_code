#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "depthtestdialog.h"

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
    connect(ui_->openGLWidget,&ModelLoadingOWgt::signalMousePickingPos,
            this, &MainWindow::slotMousePickingPos);


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

