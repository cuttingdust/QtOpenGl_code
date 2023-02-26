#ifndef HELLOOPENGLWIDGET_H
#define HELLOOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include <QWidget>
#include <QTimer>
#include <QTime>
#include <QElapsedTimer>
#include <QImage>
#include "camera.h"

class Camera;

class HelloOpenGLWidget : public QOpenGLWidget, QOpenGLFunctions
{
    Q_OBJECT
public:
    enum Shape {
        None,
        Rect,
        Circle,
        Triangle
    };

    explicit HelloOpenGLWidget(QWidget *parent = nullptr);
    
    ~HelloOpenGLWidget();
    
    void drawShape(Shape shape);
    
    void setWireframe(bool wireframe);
    
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

protected slots:
    void slotUpdateTimer();
    
signals:
    
private:
    Shape shape_;
    QOpenGLShaderProgram shaderProgram_;
    QOpenGLTexture* textureWall_{};
    QOpenGLTexture* textureSmile_{};
    QOpenGLTexture* textureSmall_{};
    QTimer updateTimer_;
    QElapsedTimer elapsedTimer_;

    Camera camera_;
};

#endif // HELLOOPENGLWIDGET_H
