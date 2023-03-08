#ifndef __MODELLOADINGOWGT_H__
#define __MODELLOADINGOWGT_H__

#include "environmentsettingdialog.h"
#include "camera.h"
#include "mesh.h"
#include "model.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include <QWidget>
#include <QTimer>
#include <QTime>
#include <QElapsedTimer>
#include <QImage>

class ModelLoadingOWgt : public QOpenGLWidget, QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit ModelLoadingOWgt(QWidget *parent = nullptr);
    ~ModelLoadingOWgt() noexcept;

public:
    void setWireframe(bool wireframe);
    void setEnvSettingType(EnvironmentType type);
public:
    EnvironmentType getViewEnvType() const;
    void setViewEnvType(EnvironmentType viewEnvType);

    void loadModel(const std::string& path);
    
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
    QVector3D cameraPosInit(float maxY,float minY);
private:
    QOpenGLShaderProgram shaderProgram_;
    QTimer updateTimer_;
    QElapsedTimer elapsedTimer_;
    Camera camera_;

    QVector3D clearColor_;
    EnvironmentType viewEnvType_;

    Model* backModel_ = nullptr;
};

#endif