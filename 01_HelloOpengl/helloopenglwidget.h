#ifndef HELLOOPENGLWIDGET_H
#define HELLOOPENGLWIDGET_H

#include <QWidget>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class HelloOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit HelloOpenGLWidget(QWidget *parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

signals:

};

#endif // HELLOOPENGLWIDGET_H
