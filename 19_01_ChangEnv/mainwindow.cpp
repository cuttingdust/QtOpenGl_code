#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);
    setCentralWidget(ui_->openGLWidget);
    connect(ui_->actionWireFrameView, SIGNAL(triggered()), this, SLOT(slotWireframeView()));
    connect(ui_->actionEnvSetting, SIGNAL(triggered()), this, SLOT(slotEnvSetting()));
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

