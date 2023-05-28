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
#include "housemodel.h"
#include "shownormaldg.h"
#include "instancingModel.h"
#include "rockmodel.h"
#include "trianglemodel.h"
#include "depthmapmodel.h"
#include "normalmapplanemodel.h"
#include "hdrcubemodel.h"
#include "bloomcubemodel.h"
#include "deferredshadingmodel.h"
#include "ssaomodel.h"

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
    Model* loadModel(const std::string path);
    void setPointRender(bool bPoint);

    void  setShowNormalDgVisible(bool bVis);
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

    [[maybe_unused]] bool isBReflectionSkyBox() const;
    void setBReflectionSkyBox(bool bReflectionSkyBox);

    [[maybe_unused]] bool isBRefractionSkyBox() const;
    void setBRefractionSkyBox(bool bRefractionSkyBox);

    [[maybe_unused]] bool isBReflectTexturteSkyBox() const;
    void setBReflectTexturteSkyBox(bool bReflectTexturteSkyBox);

    bool isBPoints() const;
    void setBPoints(bool bPoints);

    bool isBDrawHouse() const;
    void setBDrawHouse(bool bDrawHouse);

    bool isBDrawExplode() const;
    void setBDrawExplode(bool bDrawExplode);

    bool isBDrawInstancing() const;
    void setBDrawInstancing(bool bDrawInstancing);

    bool isBDrawPlanets() const;
    void setBDrawPlanets(bool bDrawPlanets);

    bool isBDrawTriangle() const;
    void setBDrawTriangle(bool bDrawTriangle);

    bool isBTestGamma() const;
    void setBTestGamma(bool bTestGamma);

    bool isBDrawDepthMap() const;
    void setBDrawDepthMap(bool bDrawDepthMap);

    bool isBDrawNormalMapTest() const;
    void setBDrawNormalMapTest(bool bDrawNormalMapTest);

    bool isBDrawBloom() const;
    void setBDrawBloom(bool bDrawBloom);

    bool isBDeferedRender() const;
    void setBDeferedRender(bool bDeferedRender);

    bool isBDrawSsao() const;
    void setBDrawSsao(bool bDrawSsao);

    void setPointLight(bool bPointLight);
    void setSSPointLight(bool bSSPointLightEnabled);
    void setDirectionalLight(bool bDirection);

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
    void slotShowNormalLength(int vau);
    void slotHDR();
    void slotchangExpose(double vaule);
    void slotBloom();
    void slotchangBloomExpose(double vaule);


signals:
    void signalMousePickingPos(QVector3D worldPos);

private:
    QVector3D cameraPosInit();

    void enableGLFun();
    void createShaderProgram();
    void createSkyBox(QOpenGLFunctions_4_1_Core *funcs);
    void createModel(QOpenGLFunctions_4_1_Core *funcs);
    void createUBOBuffer();
    void setMVPMatIdentity();
    void bindUBOBuffer();
    void bindFramBuffers();
    void enableGl();


    void drawShaderProgram();
    void drawFrambufferShaderProgram();
    void fillUBOBufferData();
    void drawVegetationShaderProgram();
    void drawWindowsShaderProgram();
    void drawrReflectionShaderProgram(Model* model);
    void drawrRefractionShaderProgram(Model* model);
    void drawShadowShaderProgram();
    void drawHDRLightShaderProgram();
    void drawNormalMapShaderProgram();
    void drawBloomLightShaderProgram();
    void drawDeferredLightShaderProgram();
    void drawSSAOModel();

    QVector4D worldPosFromViewPort(int posX,int posY);
    void drawObject();
private:
    QOpenGLShaderProgram shaderProgram_;
    QOpenGLShaderProgram lightShaderProgram_;
    QOpenGLShaderProgram outlineShaderProgram_;
    QOpenGLShaderProgram vegetationShaderProgram_;
    QOpenGLShaderProgram windowsShaderProgram_;
    QOpenGLShaderProgram frambufferShaderProgram_;
    QOpenGLShaderProgram skyBoxShaderProgram_;
    QOpenGLShaderProgram reflectionShaderProgram_;
    QOpenGLShaderProgram refractionShaderProgram_;
    QOpenGLShaderProgram houseShaderProgram_;
    QOpenGLShaderProgram normalShaderProgram_;
    QOpenGLShaderProgram instancingShaderProgram_;
    QOpenGLShaderProgram depthMapShaderProgram_;
    QOpenGLShaderProgram quadShaderProgram_;
    QOpenGLShaderProgram shadowShaderProgram_;
    QOpenGLShaderProgram normalMapShaderProgram_;
    QOpenGLShaderProgram hdrLightingShaderProgram_;
    QOpenGLShaderProgram hdrCubeShaderProgram_;
    ////////////////////////////////////////////////////////////////
    QOpenGLShaderProgram bloomLightingShaderProgram_;
    QOpenGLShaderProgram bloomLightShaderProgram_;
    QOpenGLShaderProgram bloomBlurShaderProgram_;
    QOpenGLShaderProgram bloomShaderProgram_;
    ////////////////////////////////////////////////////////////////
    QOpenGLShaderProgram gBufferShaderProgram_;
    QOpenGLShaderProgram deferredLightBoxShaderProgram_;
    QOpenGLShaderProgram deferredShadingShaderProgram_;
    ////////////////////////////////////////////////////////////////
    QOpenGLShaderProgram geometryPassShaderProgram_;
    QOpenGLShaderProgram lightingPassShaderProgram_;
    QOpenGLShaderProgram SSAOShaderProgram_;
    QOpenGLShaderProgram blurrShaderProgram_;



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
    HouseModel* aHouse_;
    InstancingModel* aInstance_;
    RockModel* aRockModel_;
    TriangleModel* aTriangle_;
    DepthMapModel* aDepthMap_;
    NormalMapPlaneModel* aNormalMapPlane_;
    HDRCubeModel* aHDRCube_;
    BloomCubeModel * aBloomCube_;
    DeferredShadingModel * aDeferredSh_;
    SSAOModel* aSSAOModel_;

    bool bShowLights_;
    bool bDrawCube_;
    bool bDrawPlane_;
    bool bDrawVegetation_;
    bool bDrawWindows_;
    bool bDrawFramBuffer_;
    bool bDrawHouse_;
    bool bDrawExplode_;
    bool bDrawPlanets_;
    bool bDrawTriangle_;
    bool bDrawNormalMapTest_;


private:
    bool bOpenSkyBox_;
    bool bReflectionSkyBox_;
    bool bRefractionSkyBox_;
    bool bReflectTexturteSkyBox_;
    bool bDrawInstancing_;
    bool bDrawDepthMap_;

private:
    bool bDepthTestShader_;
    bool bStencil_;

private:
    bool bModelMoving_;
    bool bPoints_;

private:
    bool bBlinn_;
    bool bDirLight_;
    bool bSPointLight_;
    bool bPointLight_;

    bool bTestGamma_;
    bool bGamma_;
    bool bDrawBloom_;

    bool bDeferedRender_;



private:
    ShowNormalDg* pShownormalDG_;


private:
    bool bDrawSSAO_;


};

#endif