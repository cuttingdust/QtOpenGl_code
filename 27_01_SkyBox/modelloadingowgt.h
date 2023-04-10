#ifndef __MODELLOADINGOWGT_H__
#define __MODELLOADINGOWGT_H__

#include "environmentsettingdialog.h"
#include "depthtestdialog.h"
#include "camera.h"
#include "mesh.h"
#include "model.h"
#include "lightmodel.h"
#include "cubemodel.h"
#include "planemodel.h"
#include "vegetationModel.h"
#include "windowsModel.h"
#include "glassModel.h"
#include "frambufferdialog.h"
#include "skybox.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include <QWidget>
#include <QTimer>
#include <QTime>
#include <QElapsedTimer>
#include <QImage>
#include <QVector3D>

class ModelLoadingOWgt : public QOpenGLWidget, protected  QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    explicit ModelLoadingOWgt(QWidget *parent = nullptr);
    ~ModelLoadingOWgt() noexcept;

    struct ModelInfo{
        Model *model;
        QVector3D worldPos;
        float pitch;
        float yaw;
        float roll;
        bool isSelected;
        QString name;
    };

public:
    void setWireframe(bool wireframe);
    void setEnvSettingType(EnvironmentType type);
    void setFbtSettingType(FrambufferType type);
    void loadModel(const std::string path);
public:
    EnvironmentType getViewEnvType() const;
    void setViewEnvType(EnvironmentType viewEnvType);

    DepthFuncType getTestFuncType() const;
    void setTestFuncType(DepthFuncType testFuncType);

    bool isBShowLights() const;
    void setBShowLights(bool bShowLights);

    bool isBDrawCube() const;
    void setBDrawCube(bool bDrawCube);

    bool isBDrawPlane() const;
    void setBDrawPlane(bool bDrawPlane);

    bool isBDepthTestShader() const;
    void setBDepthTestShader(bool bDepthTestShader);

    [[nodiscard]] bool isBStencil() const;
    void setBStencil(bool bStencil);

    bool isBDrawVegetation() const;
    void setBDrawVegetation(bool bDrawVegetation);

    bool isBDrawWindows() const;
    void setBDrawWindows(bool bDrawWindows);

    bool isBDrawFramBuffer() const;
    void setBDrawFramBuffer(bool bDrawFramBuffer);

    [[nodiscard]] FrambufferType getFbType() const;
    void setFbType(FrambufferType fbType);

    [[maybe_unused]] bool isBOpenSkyBox() const;
    void setBOpenSkyBox(bool bOpenSkyBox);

    bool isBReflextionSkyBox() const;
    void setBReflextionSkyBox(bool bReflextionSkyBox);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

protected slots:
    void slotUpdateTimer();
    
signals:
    void signalMousePickingPos(QVector3D worldPos);

private:
    QVector3D cameraPosInit();
    void drawShaderProgram(QMatrix4x4 projection, QMatrix4x4 view);
    void drawVegetationShaderProgram(QMatrix4x4 projection, QMatrix4x4 view);
    void drawWindowsShaderProgram(QMatrix4x4 projection, QMatrix4x4 view);
    void drawFrambufferShaderProgram(QMatrix4x4 projection, QMatrix4x4 view);
    void drawOutlineShaderProgram(QMatrix4x4 projection, QMatrix4x4 view);
    void drawrReflectionShaderProgram(QMatrix4x4 projection, QMatrix4x4 view);
    QVector4D worldPosFromViewPort(int posX,int posY);
private:
    QOpenGLShaderProgram shaderProgram_;
    QOpenGLShaderProgram lightShaderProgram_;
    QOpenGLShaderProgram outlineShaderProgram_;
    QOpenGLShaderProgram vegetationShaderProgram_;
    QOpenGLShaderProgram windowsShaderProgram_;
    QOpenGLShaderProgram frambufferShaderProgram_;
    QOpenGLShaderProgram skyBoxShaderProgram_;
    QOpenGLShaderProgram reflectionShaderProgram_;
    QTimer updateTimer_;
    QElapsedTimer elapsedTimer_;
    Camera camera_;

    QVector3D clearColor_;
    QVector3D dirlightAmbient_;
    QVector3D dirlightDiffuse_;
    QVector3D dirlightSpecular_;
    EnvironmentType viewEnvType_;
    FrambufferType fbType_;

private:
    DepthFuncType testFuncType_;
    GLenum testFunc_;

private:
    QMap<QString, ModelInfo> modelsMap_;

    SkyBox* skybox_;

    LightModel* aLight_;
    CubeModel* aCube_;
    PlaneModel* aPlane_;
    VegetationModel* aVegetation_;
    GlassModel* aGlass_;
    WindowsModel* aWindows_;

    bool bShowLights_;
    bool bDrawCube_;
    bool bDrawPlane_;
    bool bDrawVegetation_;
    bool bDrawWindows_;
    bool bDrawFramBuffer_;

    bool bOpenSkyBox_;
    bool bReflextionSkyBox_;




private:
    bool bDepthTestShader_;
    bool bStencil_;

private:
    bool bModelMoving_;
    void drawObject();
};

#endif