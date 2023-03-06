#ifndef __SIMPLELIGHTINGOWGT_H__
#define __SIMPLELIGHTINGOWGT_H__

#include "environmentsettingdialog.h"
#include "camera.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include <QWidget>
#include <QTimer>
#include <QTime>
#include <QElapsedTimer>
#include <QImage>

class SimpleLightingOWgt : public QOpenGLWidget, QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit SimpleLightingOWgt(QWidget *parent = nullptr);
    ~SimpleLightingOWgt();

public:
    void setWireframe(bool wireframe);
    void setEnvSettingType(EnvironmentType type);
public:
    EnvironmentType getViewEnvType() const;
    void setViewEnvType(EnvironmentType viewEnvType);
    
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

    QVector3D clearColor_;
    QVector3D dirlightAmbient_;
    QVector3D dirlightDiffuse_;
    QVector3D dirlightSpecular_;
    EnvironmentType viewEnvType_;

};

#endif