#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->openGLWidget);
    connect(ui->actionDrawRect, SIGNAL(triggered()), this, SLOT(slotDrawRect()));
    connect(ui->actionClear, SIGNAL(triggered()), this, SLOT(slotClearShape()));
    connect(ui->actionWireFrameView, SIGNAL(triggered()), this, SLOT(slotWireframeView()));
}

void MainWindow::slotDrawRect()
{
    ui->openGLWidget->drawShape(HelloOpenGLWidget::Rect);
}

void MainWindow::slotClearShape()
{
    ui->openGLWidget->drawShape(HelloOpenGLWidget::None);
}

void MainWindow::slotWireframeView()
{
    ui->openGLWidget->setWireframe(ui->actionWireFrameView->isChecked());
}

MainWindow::~MainWindow()
{
    delete ui;
}

