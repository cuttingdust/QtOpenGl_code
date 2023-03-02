#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->openGLWidget);
    connect(ui->actionWireFrameView, SIGNAL(triggered()), this, SLOT(slotWireframeView()));
}

void MainWindow::slotWireframeView()
{
    ui->openGLWidget->setWireframe(ui->actionWireFrameView->isChecked());
}

MainWindow::~MainWindow()
{
    delete ui;
}

