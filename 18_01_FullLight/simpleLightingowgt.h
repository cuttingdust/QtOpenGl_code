#ifndef __SIMPLELIGHTINGOWGT_H__
#define __SIMPLELIGHTINGOWGT_H__

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

class SimpleLightingOWgt : public QOpenGLWidget, QOpenGLFunctions
{
    Q_OBJECT
public:

    explicit SimpleLightingOWgt(QWidget *parent = nullptr);
    
    ~SimpleLightingOWgt();
    
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
    QOpenGLShaderProgram shaderProgram_;
    QOpenGLShaderProgram lightShaderProgram_;
    QTimer updateTimer_;
    QElapsedTimer elapsedTimer_;
    QOpenGLTexture* diffuseTexture_;
    QOpenGLTexture* specularTexture_;
    QOpenGLTexture* emissionTexture_;

    Camera camera_;
};

#endif