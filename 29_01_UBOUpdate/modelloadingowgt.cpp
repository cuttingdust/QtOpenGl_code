#include "modelloadingowgt.h"
#include "verticesData.h"

#include <QKeyEvent>
#include <QOpenGLVersionFunctionsFactory>

#define TIMEOUTSEC 50
unsigned int VBO, VAO, EBO;
unsigned int lightVAO;
float gNear = 0.1f, gFar = 100.0f;
QMatrix4x4 model;
QMatrix4x4 view;
QMatrix4x4 projection;
QPoint lastPos;

QVector3D gLightPos(1.2f, 1.0f, 2.0f);
QVector3D gViewInitPos(0.0, 5.0, 20.0);

QVector3D gLightColor(1.0f, 1.0f, 1.0f);
QVector3D gObjectColor(1.0f, 0.5f, 0.31f);

std::vector<QVector3D> gPointLightPositions = {
        QVector3D(0.7f, 0.2f, 2.0f),
        QVector3D(2.3f, -3.3f, -4.0f),
        QVector3D(-4.0f, 2.0f, -12.0f),
        QVector3D(0.0f, 0.0f, -3.0f)
};

std::vector<QVector3D> gPointLightColors;
std::vector<QVector3D> gPointLightColorsDesert = {
        QVector3D(1.0f, 0.6f, 0.0f),
        QVector3D(1.0f, 0.0f, 0.0f),
        QVector3D(1.0f, 1.0, 0.0),
        QVector3D(0.2f, 0.2f, 1.0f)
};
std::vector<QVector3D> gPointLightColorsFactory = {
        QVector3D(0.2f, 0.2f, 0.6f),
        QVector3D(0.3f, 0.3f, 0.7f),
        QVector3D(0.0f, 0.0f, 0.3f),
        QVector3D(0.4f, 0.4f, 0.4f)
};
std::vector<QVector3D> gPointLightColorsHorror = {
        QVector3D(0.1f, 0.1f, 0.1f),
        QVector3D(0.1f, 0.1f, 0.1f),
        QVector3D(0.1f, 0.1f, 0.1f),
        QVector3D(0.3f, 0.1f, 0.1f)
};

int gFrambuffer = 0;


std::vector<QVector3D> gPointLightColorsBiochemicalLab = {
        QVector3D(1.0f, 0.6f, 0.0f),
        QVector3D(1.0f, 0.0f, 0.0f),
        QVector3D(1.0f, 1.0, 0.0),
        QVector3D(0.2f, 0.2f, 1.0f)
};

unsigned int gUboMatrices;
#define QMatrix4x4DataSzie 64

ModelLoadingOWgt::ModelLoadingOWgt(QWidget *parent) :
        QOpenGLWidget{parent}
        , bShowLights_(false)
        , bDrawCube_(false)
        , bDrawPlane_(false)
        , bDepthTestShader_(false)
        , bStencil_(false)
        , bDrawVegetation_(false)
        , bDrawWindows_(false)
        , bDrawFramBuffer_(false)
        , bOpenSkyBox_(false)
        , bReflectionSkyBox_(false)
        , bRefractionSkyBox_(false)
        , bReflectTexturteSkyBox_(false)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    connect(&updateTimer_, SIGNAL(timeout()), this, SLOT(slotUpdateTimer()));
    updateTimer_.start(TIMEOUTSEC);
    elapsedTimer_.start();

    setEnvSettingType(ET_DESERT);
    setFbtSettingType(FT_DEFAULT);
    setTestFuncType(DepthFuncType::DFT_LESS);
    camera_.setPosition(gViewInitPos);
}

void ModelLoadingOWgt::initializeGL() {
    initializeOpenGLFunctions();

    //    loadModel("/Users/handabao/MyFile/code/QtOpengl/20_02_LoadMesh_ModelOOP/nanosuit/nanosuit.obj");
    auto funcs = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_4_1_Core>(QOpenGLContext::currentContext());
    if (!funcs) {
        qFatal("Could not obtain required OpenGL context version");
    }

    /// 创建shader
    createShaderProgram();

    /// 绑定UBO
    bindUBOBuffer();

    /// 使能 gl通用功能
    enableGLFun();

    /// 创建天空盒子
    createSkyBox(funcs);

    /// 创建模型
    createModel(funcs);

    /// 创建UBOBuffer
    createUBOBuffer();


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ModelLoadingOWgt::resizeGL(int w, int h) {
    Q_UNUSED(w);
    Q_UNUSED(h);
    // glViewport(0, 0, w, h);
}

void ModelLoadingOWgt::paintGL() {
    setMVPMatIdentity();

    glClearColor(clearColor_.x(), clearColor_.y(), clearColor_.z(), 1.0f);
    if (aGlass_ && bDrawFramBuffer_)
    {
        aGlass_->bindFramer();
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(testFunc_);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glStencilMask(0x00);

    view = camera_.getViewMatrix();
    projection.perspective(camera_.getZoom(), (float) width() / height(), gNear, gFar);

    /// 填充数据
    fillUBOBufferData();

    drawObject();

    if (aGlass_ && bDrawFramBuffer_)
    {
        drawFrambufferShaderProgram();
    }
}

void ModelLoadingOWgt::drawObject() {
    if (aLight_ && bShowLights_) {
        aLight_->Draw(lightShaderProgram_);
    }

    drawShaderProgram();
    {
        if (aCube_ && bDrawCube_) {
            aCube_->Draw(shaderProgram_);
        }

        if (aPlane_ && bDrawPlane_) {
            aPlane_->Draw(shaderProgram_);
        }


        if (!modelsMap_.empty())
        {
                foreach(auto modelInfo, modelsMap_) {
                model.setToIdentity();
                model.translate(modelInfo.worldPos);

                model.rotate(modelInfo.pitch, QVector3D(1.0, 0.0, 0.0));
                model.rotate(modelInfo.yaw, QVector3D(0.0, 1.0, 0.0));
                model.rotate(modelInfo.roll, QVector3D(0.0, 0.0, 1.0));

                glStencilFunc(GL_ALWAYS, 1, 0xFF);
                glStencilMask(0xFF);

                if (bReflectionSkyBox_)
                {
                    drawrReflectionShaderProgram(modelInfo.model);
                }
                else if (bRefractionSkyBox_)
                {
                    drawrRefractionShaderProgram(modelInfo.model);
                }
                else
                {
                    shaderProgram_.bind();
                    shaderProgram_.setUniformValue("model", model);
                    modelInfo.model->Draw(shaderProgram_);
                    shaderProgram_.release();
                }

                if (!modelInfo.isSelected || !bStencil_)
                    continue;
                glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
                glStencilMask(0x00);

                float height = modelInfo.model->maxY_ - modelInfo.model->minY_;
                float width = modelInfo.model->maxX_ - modelInfo.model->minX_;
                if (modelInfo.model->minY_ >= 0)
                    model.translate(0.0f, height / 2, 0.0f);

                model.scale(1.1f, 1.0 + 0.1 * (width / height));

                if (modelInfo.model->minY_ >= 0)
                    model.translate(0.0f, -height / 2, 0.0f);

                outlineShaderProgram_.bind();
                outlineShaderProgram_.setUniformValue("model", model);

                modelInfo.model->Draw(outlineShaderProgram_);
                outlineShaderProgram_.release();
                glStencilMask(0xFF);
                glStencilFunc(GL_ALWAYS, 1, 0xFF);
            }
        }
    }

    if (aVegetation_ && bDrawVegetation_)
    {
        drawVegetationShaderProgram();
    }


    if (aWindows_ && bDrawWindows_)
    {
        drawWindowsShaderProgram();
    }

    if (skybox_ && bOpenSkyBox_)
    {
        skyBoxShaderProgram_.bind();
        QMatrix4x4 skyboxView = view;// remove translation from the view matrix
        skyboxView.setColumn(3,QVector4D(0.0f,0.0f,0.0f,1.0f));
        skyBoxShaderProgram_.setUniformValue("skyView", skyboxView);
        skyBoxShaderProgram_.setUniformValue("skybox", 15);
        skybox_->Draw(skyBoxShaderProgram_);

        shaderProgram_.bind();
        shaderProgram_.setUniformValue("skybox", 15);
        skyBoxShaderProgram_.release();
    }
}

ModelLoadingOWgt::~ModelLoadingOWgt() noexcept {
    for (auto iter = modelsMap_.begin(); iter != modelsMap_.end(); iter++) {
        ModelInfo *modelInfo = &iter.value();
        delete modelInfo->model;
    }
}

void ModelLoadingOWgt::setWireframe(bool wireframe) {
    makeCurrent();
    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    doneCurrent();
    update();
}

void ModelLoadingOWgt::keyPressEvent(QKeyEvent *event) {
    float deltaTime = TIMEOUTSEC / 1000.0;
    switch (event->key()) {
        case Qt::Key_W:
            camera_.processKeyboard(FORWARD, deltaTime);
            break;
        case Qt::Key_S:
            camera_.processKeyboard(BACKWARD, deltaTime);
            break;
        case Qt::Key_D:
            camera_.processKeyboard(RIGHT, deltaTime);
            break;
        case Qt::Key_A:
            camera_.processKeyboard(LEFT, deltaTime);
            break;
        case Qt::Key_Q:
            camera_.processKeyboard(DOWN, deltaTime);
            break;
        case Qt::Key_E:
            camera_.processKeyboard(UP, deltaTime);
            break;
        case Qt::Key_Space:
            camera_.setPosition(gViewInitPos);
            break;
        default:
            break;
    };

    update();
}

void ModelLoadingOWgt::slotUpdateTimer() {
    update();
}

void ModelLoadingOWgt::mouseMoveEvent(QMouseEvent *event) {

    makeCurrent();

    if (bModelMoving_) {
        for (auto iter = modelsMap_.begin(); iter != modelsMap_.end(); ++iter) {
            ModelInfo *modelInfo = &iter.value();
            if (!modelInfo->isSelected) continue;
            modelInfo->worldPos = QVector3D(worldPosFromViewPort(event->pos().x(), event->pos().y()));
        }
    } else {
        if (event->buttons() & Qt::RightButton
            || event->buttons() & Qt::LeftButton
            || event->buttons() & Qt::MiddleButton) {

            auto currentPos = event->pos();
            QPoint deltaPos = currentPos - lastPos;
            lastPos = currentPos;

            if (event->buttons() & Qt::RightButton)
                camera_.processMouseMovement(deltaPos.x(), -deltaPos.y());
            else {
                for (auto iter = modelsMap_.begin(); iter != modelsMap_.end(); iter++) {
                    ModelInfo *modelInfo = &iter.value();
                    if (!modelInfo->isSelected) continue;

                    if (event->buttons() & Qt::MiddleButton) {
                        modelInfo->roll += deltaPos.x();
                    } else if (event->buttons() & Qt::LeftButton) {
                        modelInfo->yaw += deltaPos.x();
                        modelInfo->pitch += deltaPos.y();
                    }
                }
            }
        }
    }


    doneCurrent();
    QWidget::mouseMoveEvent(event);
}

void ModelLoadingOWgt::wheelEvent(QWheelEvent *event) {
    camera_.processMouseScroll(event->angleDelta().y() / 120);
    QWidget::wheelEvent(event);
}

EnvironmentType ModelLoadingOWgt::getViewEnvType() const {
    return viewEnvType_;
}

void ModelLoadingOWgt::setViewEnvType(EnvironmentType viewEnvType) {
    viewEnvType_ = viewEnvType;
}

void ModelLoadingOWgt::setEnvSettingType(EnvironmentType type) {
    setViewEnvType(type);
    switch (type) {
        case EnvironmentType::ET_DESERT:
            clearColor_ = QVector3D(0.75f, 0.52f, 0.3f);
            dirlightAmbient_ = QVector3D(0.3f, 0.24f, 0.14f);
            dirlightDiffuse_ = QVector3D(0.7f, 0.42f, 0.26f);
            dirlightSpecular_ = QVector3D(0.5f, 0.5f, 0.5f);
            gPointLightColors = gPointLightColorsDesert;
            break;
        case EnvironmentType::ET_FACTTORY:
            clearColor_ = QVector3D(0.0f, 0.0f, 0.0f);
            dirlightAmbient_ = QVector3D(0.05f, 0.05f, 0.1f);
            dirlightDiffuse_ = QVector3D(0.2f, 0.2f, 0.7);
            dirlightSpecular_ = QVector3D(0.7f, 0.7f, 0.7f);
            gPointLightColors = gPointLightColorsFactory;
            break;
        case EnvironmentType::ET_HORROR:
            clearColor_ = QVector3D(0.0f, 0.0f, 0.0f);
            dirlightAmbient_ = QVector3D(0.0f, 0.0f, 0.0f);
            dirlightDiffuse_ = QVector3D(0.05f, 0.05f, 0.05);
            dirlightSpecular_ = QVector3D(0.2f, 0.2f, 0.2f);
            gPointLightColors = gPointLightColorsHorror;
            break;
        case EnvironmentType::ET_BIOCHEMICALLAB:
            clearColor_ = QVector3D(0.9f, 0.9f, 0.9f);
            dirlightAmbient_ = QVector3D(0.5f, 0.5f, 0.5f);
            dirlightDiffuse_ = QVector3D(1.0f, 1.0f, 1.0f);
            dirlightSpecular_ = QVector3D(1.0f, 1.0f, 1.0f);
            gPointLightColors = gPointLightColorsBiochemicalLab;
            break;
        default:
            break;
    }
}


void ModelLoadingOWgt::loadModel(const std::string path) {
    auto funcs = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_4_1_Core>(QOpenGLContext::currentContext());
    if (!funcs) {
        qFatal("Could not obtain required OpenGL context version");
    }
    static int i = 0;
    Model *model = new Model(funcs, path.c_str());

    modelsMap_["Julian" + QString::number(i++)] =
            ModelInfo{model, QVector3D(0.0f, 0.0f - model->minY_, 0.0f), 0.0, 0.0, 0.0, false, "张三"};
}

QVector3D ModelLoadingOWgt::cameraPosInit() {

    float maxY, minY;

            foreach(auto modelInfo, modelsMap_) {
            if (maxY < modelInfo.model->maxY_) maxY = modelInfo.model->maxY_;
            if (minY > modelInfo.model->minY_) minY = modelInfo.model->minY_;
        }

    QVector3D temp = {0, 0, 0};
    float height = maxY - minY;
    temp.setZ(1.5 * height);
    if (minY >= 0)
        temp.setY(height / 2.0);
    gViewInitPos = temp;
    return temp;
}

bool ModelLoadingOWgt::isBShowLights() const {
    return bShowLights_;
}

void ModelLoadingOWgt::setBShowLights(bool bShowLights) {
    bShowLights_ = bShowLights;
}

void ModelLoadingOWgt::drawShaderProgram() {
    shaderProgram_.bind();
    shaderProgram_.setUniformValue("lightPos", gLightPos);
    shaderProgram_.setUniformValue("viewPos", camera_.getPosition());
    shaderProgram_.setUniformValue("skybox", 15);

    shaderProgram_.setUniformValue("material.shininess", 32.0f);
    shaderProgram_.setUniformValue("spotLight.position", camera_.getPosition());
    shaderProgram_.setUniformValue("spotL˜ight.direction", camera_.getFront());
    shaderProgram_.setUniformValue("spotLight.cutOff", static_cast<float>(cos(12.5f * M_PI / 180)));
    shaderProgram_.setUniformValue("spotLight.outerCutOff", (float) cos(17.5f * M_PI / 180));
    shaderProgram_.setUniformValue("spotLight.ambient", 0.4f, 0.4f, 0.4f);
    shaderProgram_.setUniformValue("spotLight.diffuse", 0.9f, 0.9f, 0.9f);
    shaderProgram_.setUniformValue("spotLight.specular", 1.0f, 1.0f, 1.0f);
    shaderProgram_.setUniformValue("spotLight.constant", 1.0f);
    shaderProgram_.setUniformValue("spotLight.linear", 0.09f);
    shaderProgram_.setUniformValue("spotLight.quadratic", 0.032f);

    // directional light
    shaderProgram_.setUniformValue("dirLight.direction", -0.2f, -1.0f, -0.3f);
    shaderProgram_.setUniformValue("dirLight.ambient", dirlightAmbient_);
    shaderProgram_.setUniformValue("dirLight.diffuse", dirlightDiffuse_);
    shaderProgram_.setUniformValue("dirLight.specular", dirlightSpecular_);

    /// Depth Z Buffer Test
    shaderProgram_.setUniformValue("bSimpleZBudder", bDepthTestShader_);

    // point lights
    for (int i = 0; i < 4; i++) {
        QString iStr = "pointLights[" + QString::number(i) + "].position";
        shaderProgram_.setUniformValue(iStr.toStdString().c_str(), gPointLightPositions[i].x(),
                                       gPointLightPositions[i].y(), gPointLightPositions[i].z());
        iStr = "pointLights[" + QString::number(i) + "].ambient";
        shaderProgram_.setUniformValue(iStr.toStdString().c_str(), gPointLightColors[i].x() * 0.1,
                                       gPointLightColors[i].y() * 0.1, gPointLightColors[i].z() * 0.1);
        iStr = "pointLights[" + QString::number(i) + "].diffuse";
        shaderProgram_.setUniformValue(iStr.toStdString().c_str(), gPointLightColors[i].x(), gPointLightColors[i].y(),
                                       gPointLightColors[i].z());
        iStr = "pointLights[" + QString::number(i) + "].specular";
        shaderProgram_.setUniformValue(iStr.toStdString().c_str(), gPointLightColors[i].x(), gPointLightColors[i].y(),
                                       gPointLightColors[i].z());
        iStr = "pointLights[" + QString::number(i) + "].constant";
        shaderProgram_.setUniformValue(iStr.toStdString().c_str(), 1.0f);
        iStr = "pointLights[" + QString::number(i) + "].linear";
        shaderProgram_.setUniformValue(iStr.toStdString().c_str(), 0.09f);
        iStr = "pointLights[" + QString::number(i) + "].quadratic";
        shaderProgram_.setUniformValue(iStr.toStdString().c_str(), 0.032f);
    }

}

bool ModelLoadingOWgt::isBDrawCube() const {
    return bDrawCube_;
}

void ModelLoadingOWgt::setBDrawCube(bool bDrawCube) {
    bDrawCube_ = bDrawCube;
}

bool ModelLoadingOWgt::isBDrawPlane() const {
    return bDrawPlane_;
}

void ModelLoadingOWgt::setBDrawPlane(bool bDrawPlane) {
    bDrawPlane_ = bDrawPlane;
}

DepthFuncType ModelLoadingOWgt::getTestFuncType() const {
    return testFuncType_;
}

void ModelLoadingOWgt::setTestFuncType(DepthFuncType testFuncType) {
    testFuncType_ = testFuncType;
    switch (testFuncType_) {
        case DepthFuncType::DFT_LESS:
            testFunc_ = GL_LESS;
            break;
        case DepthFuncType::DFT_ALWAYS:
            testFunc_ = GL_ALWAYS;
            break;
        case DepthFuncType::DFT_GREATER:
            testFunc_ = GL_GREATER;
            break;
        case DepthFuncType::DFT_EQUAL:
            testFunc_ = GL_EQUAL;
            break;
    }
}

bool ModelLoadingOWgt::isBDepthTestShader() const {
    return bDepthTestShader_;
}

void ModelLoadingOWgt::setBDepthTestShader(bool bDepthTestShader) {
    bDepthTestShader_ = bDepthTestShader;
}

void ModelLoadingOWgt::mousePressEvent(QMouseEvent *event) {
    bool bHasSelected = false;

    makeCurrent();
    lastPos = event->pos();
    if (event->buttons() & Qt::LeftButton) {

        QVector4D wolrdPostion = worldPosFromViewPort(event->pos().x(),
                                                      event->pos().y());

        signalMousePickingPos(QVector3D(wolrdPostion));

        for (auto iter = modelsMap_.begin(); iter != modelsMap_.end(); ++iter) {
            ModelInfo *modelInfo = &iter.value();
            float r = (modelInfo->model->maxY_ - modelInfo->model->minY_) / 2;
            if (modelInfo->worldPos.distanceToPoint(QVector3D(wolrdPostion)) < r && !bHasSelected) {
                modelInfo->isSelected = true;
                bHasSelected = true;
            } else {
                modelInfo->isSelected = false;
            }

            qDebug() << modelInfo->worldPos.distanceToPoint(QVector3D(wolrdPostion))
                     << "<" << r << "=" << modelInfo->isSelected;
        }

    }

    doneCurrent();
}

QVector4D ModelLoadingOWgt::worldPosFromViewPort(int posX, int posY) {
    float winZ;
    glReadPixels(
            posX,
            this->height() - posY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    float x = (2.0f * posX) / this->width() - 1.0f;
    float y = 1.0f - (2.0f * posY) / this->height();
    float z = winZ * 2.0 - 1.0f;

    float w = (2.0 * gNear * gFar) / (gFar + gNear - z * (gFar - gNear));
    //float w= gNear*gFar/(gNear*winZ-gFar*winZ+gFar);
    QVector4D wolrdPostion(x, y, z, 1);
    wolrdPostion = w * wolrdPostion;
    return view.inverted() * projection.inverted() * wolrdPostion;
}

void ModelLoadingOWgt::mouseDoubleClickEvent(QMouseEvent *event) {
    Q_UNUSED(event);

    if (bModelMoving_) {
        //再次双击取消移动
        bModelMoving_ = false;
    } else {
                foreach(auto modelInfo, modelsMap_) {
                //双击启动移动
                if (modelInfo.isSelected == true)
                    bModelMoving_ = true;
                qDebug() << modelInfo.name << modelInfo.isSelected;
            }
    }

    QWidget::mouseDoubleClickEvent(event);
}

bool ModelLoadingOWgt::isBStencil() const {
    return bStencil_;
}

void ModelLoadingOWgt::setBStencil(bool bStencil) {
    bStencil_ = bStencil;
}

void ModelLoadingOWgt::drawVegetationShaderProgram() {
    vegetationShaderProgram_.bind();
    vegetationShaderProgram_.setUniformValue("lightPos", gLightPos);
    vegetationShaderProgram_.setUniformValue("viewPos", camera_.getPosition());

    vegetationShaderProgram_.setUniformValue("material.shininess", 32.0f);
    vegetationShaderProgram_.setUniformValue("spotLight.position", camera_.getPosition());
    vegetationShaderProgram_.setUniformValue("spotL˜ight.direction", camera_.getFront());
    vegetationShaderProgram_.setUniformValue("spotLight.cutOff", static_cast<float>(cos(12.5f * M_PI / 180)));
    vegetationShaderProgram_.setUniformValue("spotLight.outerCutOff", (float) cos(17.5f * M_PI / 180));
    vegetationShaderProgram_.setUniformValue("spotLight.ambient", 0.4f, 0.4f, 0.4f);
    vegetationShaderProgram_.setUniformValue("spotLight.diffuse", 0.9f, 0.9f, 0.9f);
    vegetationShaderProgram_.setUniformValue("spotLight.specular", 1.0f, 1.0f, 1.0f);
    vegetationShaderProgram_.setUniformValue("spotLight.constant", 1.0f);
    vegetationShaderProgram_.setUniformValue("spotLight.linear", 0.09f);
    vegetationShaderProgram_.setUniformValue("spotLight.quadratic", 0.032f);

    // directional light
    vegetationShaderProgram_.setUniformValue("dirLight.direction", -0.2f, -1.0f, -0.3f);
    vegetationShaderProgram_.setUniformValue("dirLight.ambient", dirlightAmbient_);
    vegetationShaderProgram_.setUniformValue("dirLight.diffuse", dirlightDiffuse_);
    vegetationShaderProgram_.setUniformValue("dirLight.specular", dirlightSpecular_);

    /// Depth Z Buffer Test
    vegetationShaderProgram_.setUniformValue("bSimpleZBudder", bDepthTestShader_);

    // point lights
    for (int i = 0; i < 4; i++) {
        QString iStr = "pointLights[" + QString::number(i) + "].position";
        vegetationShaderProgram_.setUniformValue(iStr.toStdString().c_str(), gPointLightPositions[i].x(),
                                       gPointLightPositions[i].y(), gPointLightPositions[i].z());
        iStr = "pointLights[" + QString::number(i) + "].ambient";
        vegetationShaderProgram_.setUniformValue(iStr.toStdString().c_str(), gPointLightColors[i].x() * 0.1,
                                       gPointLightColors[i].y() * 0.1, gPointLightColors[i].z() * 0.1);
        iStr = "pointLights[" + QString::number(i) + "].diffuse";
        vegetationShaderProgram_.setUniformValue(iStr.toStdString().c_str(), gPointLightColors[i].x(), gPointLightColors[i].y(),
                                       gPointLightColors[i].z());
        iStr = "pointLights[" + QString::number(i) + "].specular";
        vegetationShaderProgram_.setUniformValue(iStr.toStdString().c_str(), gPointLightColors[i].x(), gPointLightColors[i].y(),
                                       gPointLightColors[i].z());
        iStr = "pointLights[" + QString::number(i) + "].constant";
        vegetationShaderProgram_.setUniformValue(iStr.toStdString().c_str(), 1.0f);
        iStr = "pointLights[" + QString::number(i) + "].linear";
        vegetationShaderProgram_.setUniformValue(iStr.toStdString().c_str(), 0.09f);
        iStr = "pointLights[" + QString::number(i) + "].quadratic";
        vegetationShaderProgram_.setUniformValue(iStr.toStdString().c_str(), 0.032f);
    }

    aVegetation_->Draw(vegetationShaderProgram_);

    vegetationShaderProgram_.release();
}

bool ModelLoadingOWgt::isBDrawVegetation() const {
    return bDrawVegetation_;
}

void ModelLoadingOWgt::setBDrawVegetation(bool bDrawVegetation) {
    bDrawVegetation_ = bDrawVegetation;
}

void ModelLoadingOWgt::drawWindowsShaderProgram() {
    windowsShaderProgram_.bind();
    windowsShaderProgram_.setUniformValue("lightPos", gLightPos);
    windowsShaderProgram_.setUniformValue("viewPos", camera_.getPosition());

    windowsShaderProgram_.setUniformValue("material.shininess", 32.0f);
    windowsShaderProgram_.setUniformValue("spotLight.position", camera_.getPosition());
    windowsShaderProgram_.setUniformValue("spotL˜ight.direction", camera_.getFront());
    windowsShaderProgram_.setUniformValue("spotLight.cutOff", static_cast<float>(cos(12.5f * M_PI / 180)));
    windowsShaderProgram_.setUniformValue("spotLight.outerCutOff", (float) cos(17.5f * M_PI / 180));
    windowsShaderProgram_.setUniformValue("spotLight.ambient", 0.4f, 0.4f, 0.4f);
    windowsShaderProgram_.setUniformValue("spotLight.diffuse", 0.9f, 0.9f, 0.9f);
    windowsShaderProgram_.setUniformValue("spotLight.specular", 1.0f, 1.0f, 1.0f);
    windowsShaderProgram_.setUniformValue("spotLight.constant", 1.0f);
    windowsShaderProgram_.setUniformValue("spotLight.linear", 0.09f);
    windowsShaderProgram_.setUniformValue("spotLight.quadratic", 0.032f);

    // directional light
    windowsShaderProgram_.setUniformValue("dirLight.direction", -0.2f, -1.0f, -0.3f);
    windowsShaderProgram_.setUniformValue("dirLight.ambient", dirlightAmbient_);
    windowsShaderProgram_.setUniformValue("dirLight.diffuse", dirlightDiffuse_);
    windowsShaderProgram_.setUniformValue("dirLight.specular", dirlightSpecular_);

    /// Depth Z Buffer Test
    windowsShaderProgram_.setUniformValue("bSimpleZBudder", bDepthTestShader_);

    // point lights
    for (int i = 0; i < 4; i++) {
        QString iStr = "pointLights[" + QString::number(i) + "].position";
        windowsShaderProgram_.setUniformValue(iStr.toStdString().c_str(), gPointLightPositions[i].x(),
                                                 gPointLightPositions[i].y(), gPointLightPositions[i].z());
        iStr = "pointLights[" + QString::number(i) + "].ambient";
        windowsShaderProgram_.setUniformValue(iStr.toStdString().c_str(), gPointLightColors[i].x() * 0.1,
                                                 gPointLightColors[i].y() * 0.1, gPointLightColors[i].z() * 0.1);
        iStr = "pointLights[" + QString::number(i) + "].diffuse";
        windowsShaderProgram_.setUniformValue(iStr.toStdString().c_str(), gPointLightColors[i].x(), gPointLightColors[i].y(),
                                                 gPointLightColors[i].z());
        iStr = "pointLights[" + QString::number(i) + "].specular";
        windowsShaderProgram_.setUniformValue(iStr.toStdString().c_str(), gPointLightColors[i].x(), gPointLightColors[i].y(),
                                                 gPointLightColors[i].z());
        iStr = "pointLights[" + QString::number(i) + "].constant";
        windowsShaderProgram_.setUniformValue(iStr.toStdString().c_str(), 1.0f);
        iStr = "pointLights[" + QString::number(i) + "].linear";
        windowsShaderProgram_.setUniformValue(iStr.toStdString().c_str(), 0.09f);
        iStr = "pointLights[" + QString::number(i) + "].quadratic";
        windowsShaderProgram_.setUniformValue(iStr.toStdString().c_str(), 0.032f);
    }

    aWindows_->Draw(windowsShaderProgram_);
}

bool ModelLoadingOWgt::isBDrawWindows() const {
    return bDrawWindows_;
}

void ModelLoadingOWgt::setBDrawWindows(bool bDrawWindows) {
    bDrawWindows_ = bDrawWindows;
}

bool ModelLoadingOWgt::isBDrawFramBuffer() const {
    return bDrawFramBuffer_;
}

void ModelLoadingOWgt::setBDrawFramBuffer(bool bDrawFramBuffer) {
    bDrawFramBuffer_ = bDrawFramBuffer;
}

void ModelLoadingOWgt::drawFrambufferShaderProgram() {
    frambufferShaderProgram_.bind();
    frambufferShaderProgram_.setUniformValue("lightPos", gLightPos);
    frambufferShaderProgram_.setUniformValue("viewPos", camera_.getPosition());

    frambufferShaderProgram_.setUniformValue("material.shininess", 32.0f);
    frambufferShaderProgram_.setUniformValue("spotLight.position", camera_.getPosition());
    frambufferShaderProgram_.setUniformValue("spotL˜ight.direction", camera_.getFront());
    frambufferShaderProgram_.setUniformValue("spotLight.cutOff", static_cast<float>(cos(12.5f * M_PI / 180)));
    frambufferShaderProgram_.setUniformValue("spotLight.outerCutOff", (float) cos(17.5f * M_PI / 180));
    frambufferShaderProgram_.setUniformValue("spotLight.ambient", 0.4f, 0.4f, 0.4f);
    frambufferShaderProgram_.setUniformValue("spotLight.diffuse", 0.9f, 0.9f, 0.9f);
    frambufferShaderProgram_.setUniformValue("spotLight.specular", 1.0f, 1.0f, 1.0f);
    frambufferShaderProgram_.setUniformValue("spotLight.constant", 1.0f);
    frambufferShaderProgram_.setUniformValue("spotLight.linear", 0.09f);
    frambufferShaderProgram_.setUniformValue("spotLight.quadratic", 0.032f);

    // directional light
    frambufferShaderProgram_.setUniformValue("dirLight.direction", -0.2f, -1.0f, -0.3f);
    frambufferShaderProgram_.setUniformValue("dirLight.ambient", dirlightAmbient_);
    frambufferShaderProgram_.setUniformValue("dirLight.diffuse", dirlightDiffuse_);
    frambufferShaderProgram_.setUniformValue("dirLight.specular", dirlightSpecular_);

    /// Depth Z Buffer Test
    frambufferShaderProgram_.setUniformValue("bSimpleZBudder", bDepthTestShader_);

    // point lights
    for (int i = 0; i < 4; i++) {
        QString iStr = "pointLights[" + QString::number(i) + "].position";
        frambufferShaderProgram_.setUniformValue(iStr.toStdString().c_str(), gPointLightPositions[i].x(),
                                              gPointLightPositions[i].y(), gPointLightPositions[i].z());
        iStr = "pointLights[" + QString::number(i) + "].ambient";
        frambufferShaderProgram_.setUniformValue(iStr.toStdString().c_str(), gPointLightColors[i].x() * 0.1,
                                              gPointLightColors[i].y() * 0.1, gPointLightColors[i].z() * 0.1);
        iStr = "pointLights[" + QString::number(i) + "].diffuse";
        frambufferShaderProgram_.setUniformValue(iStr.toStdString().c_str(), gPointLightColors[i].x(), gPointLightColors[i].y(),
                                              gPointLightColors[i].z());
        iStr = "pointLights[" + QString::number(i) + "].specular";
        frambufferShaderProgram_.setUniformValue(iStr.toStdString().c_str(), gPointLightColors[i].x(), gPointLightColors[i].y(),
                                              gPointLightColors[i].z());
        iStr = "pointLights[" + QString::number(i) + "].constant";
        frambufferShaderProgram_.setUniformValue(iStr.toStdString().c_str(), 1.0f);
        iStr = "pointLights[" + QString::number(i) + "].linear";
        frambufferShaderProgram_.setUniformValue(iStr.toStdString().c_str(), 0.09f);
        iStr = "pointLights[" + QString::number(i) + "].quadratic";
        frambufferShaderProgram_.setUniformValue(iStr.toStdString().c_str(), 0.032f);
    }

    aGlass_->Draw(frambufferShaderProgram_);
    drawObject();
}

void ModelLoadingOWgt::setFbtSettingType(FrambufferType type) {
    setFbType(type);
    gFrambuffer = static_cast<int> (fbType_);
    frambufferShaderProgram_.bind();
    frambufferShaderProgram_.setUniformValue("frambufferType", gFrambuffer);
}

FrambufferType ModelLoadingOWgt::getFbType() const {
    return fbType_;
}

void ModelLoadingOWgt::setFbType(FrambufferType fbType) {
    fbType_ = fbType;
}

[[maybe_unused]] bool ModelLoadingOWgt::isBOpenSkyBox() const {
    return bOpenSkyBox_;
}

void ModelLoadingOWgt::setBOpenSkyBox(bool bOpenSkyBox) {
    bOpenSkyBox_ = bOpenSkyBox;
}

void ModelLoadingOWgt::drawrReflectionShaderProgram(Model* m) {
    reflectionShaderProgram_.bind();
    reflectionShaderProgram_.setUniformValue("model", model);
    reflectionShaderProgram_.setUniformValue("viewPos",camera_.getPosition());
    reflectionShaderProgram_.setUniformValue("skybox", 15);
    m->Draw(reflectionShaderProgram_);
    reflectionShaderProgram_.release();
}

[[maybe_unused]] bool ModelLoadingOWgt::isBReflectionSkyBox() const {
    return bReflectionSkyBox_;
}

void ModelLoadingOWgt::setBReflectionSkyBox(bool bReflectionSkyBox) {
    bReflectionSkyBox_ = bReflectionSkyBox;
}

void ModelLoadingOWgt::drawrRefractionShaderProgram(Model* m) {
    refractionShaderProgram_.bind();
    refractionShaderProgram_.setUniformValue("model", model);
    refractionShaderProgram_.setUniformValue("viewPos",camera_.getPosition());
    refractionShaderProgram_.setUniformValue("skybox", 15);
    m->Draw(refractionShaderProgram_);
    refractionShaderProgram_.release();
}

[[maybe_unused]] bool ModelLoadingOWgt::isBRefractionSkyBox() const {
    return bRefractionSkyBox_;
}

void ModelLoadingOWgt::setBRefractionSkyBox(bool bRefractionSkyBox) {
    bRefractionSkyBox_ = bRefractionSkyBox;
}

[[maybe_unused]] bool ModelLoadingOWgt::isBReflectTexturteSkyBox() const {
    return bReflectTexturteSkyBox_;
}

void ModelLoadingOWgt::setBReflectTexturteSkyBox(bool bReflectTexturteSkyBox) {
    bReflectTexturteSkyBox_ = bReflectTexturteSkyBox;
    shaderProgram_.bind();
    shaderProgram_.setUniformValue("bReflectTexturteSkyBox", bReflectTexturteSkyBox);
}

void ModelLoadingOWgt::setPointRender(bool bPoint) {
    for (auto& modelInfo: modelsMap_)
    {
        setBPoints(bPoint);
        modelInfo.model->setPoints(bPoints_? GL_POINTS: GL_TRIANGLES);
    }
}

bool ModelLoadingOWgt::isBPoints() const {
    return bPoints_;
}

void ModelLoadingOWgt::setBPoints(bool bPoints) {
    bPoints_ = bPoints;
}

void ModelLoadingOWgt::createShaderProgram() {
    shaderProgram_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/shapes.vert");
    shaderProgram_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/shapes.frag");
    auto bSucess = shaderProgram_.link();
    if (!bSucess) {
        qWarning() << __FUNCTION__ << " " << shaderProgram_.log();
    }

    lightShaderProgram_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/lighting/lighting.vert");
    lightShaderProgram_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/lighting/lighting.frag");
    bSucess = lightShaderProgram_.link();
    if (!bSucess) {
        qWarning() << __FUNCTION__ << " " << lightShaderProgram_.log();
    }

    outlineShaderProgram_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/outline/outline.vert");
    outlineShaderProgram_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/outline/outline.frag");
    bSucess = outlineShaderProgram_.link();
    if (!bSucess) {
        qWarning() << __FUNCTION__ << " " << outlineShaderProgram_.log();
    }

    vegetationShaderProgram_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vegetation/vegetation.vert");
    vegetationShaderProgram_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/vegetation/vegetation.frag");
    bSucess = vegetationShaderProgram_.link();
    if (!bSucess) {
        qWarning() << __FUNCTION__ << " " << vegetationShaderProgram_.log();
    }

    windowsShaderProgram_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/windows/windows.vert");
    windowsShaderProgram_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/windows/windows.frag");
    bSucess = windowsShaderProgram_.link();
    if (!bSucess) {
        qWarning() << __FUNCTION__ << " " << windowsShaderProgram_.log();
    }

    frambufferShaderProgram_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/frambuffer/frambuffer.vert");
    frambufferShaderProgram_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/frambuffer/frambuffer.frag");
    bSucess = frambufferShaderProgram_.link();
    if (!bSucess) {
        qWarning() << __FUNCTION__ << " " << frambufferShaderProgram_.log();
    }

    skyBoxShaderProgram_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/skybox/skybox.vert");
    skyBoxShaderProgram_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/skybox/skybox.frag");
    bSucess = skyBoxShaderProgram_.link();
    if (!bSucess) {
        qWarning() << __FUNCTION__ << " " << skyBoxShaderProgram_.log();
    }

    reflectionShaderProgram_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/reflection/reflection.vert");
    reflectionShaderProgram_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/reflection/reflection.frag");
    bSucess = reflectionShaderProgram_.link();
    if (!bSucess) {
        qWarning() << __FUNCTION__ << " " << reflectionShaderProgram_.log();
    }

    refractionShaderProgram_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/refraction/refraction.vert");
    refractionShaderProgram_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/refraction/refraction.frag");
    bSucess = refractionShaderProgram_.link();
    if (!bSucess) {
        qWarning() << __FUNCTION__ << " " << refractionShaderProgram_.log();
    }
}

void ModelLoadingOWgt::bindUBOBuffer() {
    int uniformBlockIndexShader =
            glGetUniformBlockIndex(shaderProgram_.programId(),"Matrices");
    glUniformBlockBinding(shaderProgram_.programId(),
                          uniformBlockIndexShader, 0);

    int uniformBlockIndexSkyBoxShader =
            glGetUniformBlockIndex(skyBoxShaderProgram_.programId(),"Matrices");
    glUniformBlockBinding(skyBoxShaderProgram_.programId(),
                          uniformBlockIndexSkyBoxShader, 0);

    int uniformBlockIndexFramBufferShader =
            glGetUniformBlockIndex(frambufferShaderProgram_.programId(),"Matrices");
    glUniformBlockBinding(frambufferShaderProgram_.programId(),
                          uniformBlockIndexFramBufferShader, 0);

    int uniformBlockIndexLightMapShader =
            glGetUniformBlockIndex(lightShaderProgram_.programId(),"Matrices");
    glUniformBlockBinding(lightShaderProgram_.programId(),
                          uniformBlockIndexLightMapShader, 0);

    int uniformBlockIndexOutlineShader =
            glGetUniformBlockIndex(outlineShaderProgram_.programId(),"Matrices");
    glUniformBlockBinding(outlineShaderProgram_.programId(),
                          uniformBlockIndexOutlineShader, 0);

    int uniformBlockIndexReflectionShader =
            glGetUniformBlockIndex(reflectionShaderProgram_.programId(),"Matrices");
    glUniformBlockBinding(reflectionShaderProgram_.programId(),
                          uniformBlockIndexReflectionShader, 0);

    int uniformBlockIndexRefractionShader =
            glGetUniformBlockIndex(refractionShaderProgram_.programId(),"Matrices");
    glUniformBlockBinding(refractionShaderProgram_.programId(),
                          uniformBlockIndexRefractionShader, 0);

    int uniformBlockIndexVegationShader =
            glGetUniformBlockIndex(vegetationShaderProgram_.programId(),"Matrices");
    glUniformBlockBinding(vegetationShaderProgram_.programId(),
                          uniformBlockIndexVegationShader, 0);

    int uniformBlockIndexWindowShader =
            glGetUniformBlockIndex(windowsShaderProgram_.programId(),"Matrices");
    glUniformBlockBinding(windowsShaderProgram_.programId(),
                          uniformBlockIndexWindowShader, 0);
}

void ModelLoadingOWgt::enableGLFun() {
    //    ::glEnable(GL_CULL_FACE);
    //    ::glFrontFace(GL_CCW);
    //    ::glCullFace(GL_FRONT);
    ::glEnable(GL_PROGRAM_POINT_SIZE);
}

void ModelLoadingOWgt::createSkyBox(QOpenGLFunctions_4_1_Core* funcs) {
    skybox_ = new SkyBox(funcs);
}

void ModelLoadingOWgt::createModel(QOpenGLFunctions_4_1_Core *funcs) {
    aLight_ = new LightModel(funcs, gPointLightPositions, gPointLightColors);
    aCube_ = new CubeModel(funcs);
    aPlane_ = new PlaneModel(funcs);
    aVegetation_ = new VegetationModel(funcs);
    aWindows_ = new WindowsModel(funcs, camera_);
    aGlass_ = new GlassModel(funcs, QWidget::width(), QWidget::height());
}

void ModelLoadingOWgt::createUBOBuffer() {
    glGenBuffers(1, &gUboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, gUboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * QMatrix4x4DataSzie, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, gUboMatrices, 0, 2 * QMatrix4x4DataSzie);
}

void ModelLoadingOWgt::fillUBOBufferData() {
    /// 填充UBO数据
    glBindBuffer(GL_UNIFORM_BUFFER, gUboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, QMatrix4x4DataSzie, projection.constData());
//    glBindBuffer(GL_UNIFORM_BUFFER, 0);
//    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, QMatrix4x4DataSzie, QMatrix4x4DataSzie, view.constData());
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ModelLoadingOWgt::setMVPMatIdentity() {
    model.setToIdentity();
    view.setToIdentity();
    projection.setToIdentity();
}




