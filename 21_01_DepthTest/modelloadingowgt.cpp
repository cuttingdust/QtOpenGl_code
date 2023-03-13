#include "modelloadingowgt.h"

#include <QKeyEvent>
#include <QOpenGLVersionFunctionsFactory>

#define TIMEOUTSEC 50
unsigned int VBO, VAO, EBO;
unsigned  int lightVAO;

QVector3D gLightPos(1.2f, 1.0f, 2.0f);
QVector3D gViewInitPos(0.0,0.0,5.0);

QVector3D gLightColor(1.0f, 1.0f, 1.0f);
QVector3D gObjectColor(1.0f, 0.5f, 0.31f);

std::vector<QVector3D> gPointLightPositions = {
        QVector3D( 0.7f, 0.2f, 2.0f),
        QVector3D( 2.3f, -3.3f, -4.0f),
        QVector3D(-4.0f, 2.0f, -12.0f),
        QVector3D( 0.0f, 0.0f, -3.0f)
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

std::vector<QVector3D> gPointLightColorsBiochemicalLab = {
        QVector3D(1.0f, 0.6f, 0.0f),
        QVector3D(1.0f, 0.0f, 0.0f),
        QVector3D(1.0f, 1.0, 0.0),
        QVector3D(0.2f, 0.2f, 1.0f)
};

ModelLoadingOWgt::ModelLoadingOWgt(QWidget *parent):
        QOpenGLWidget{parent}
        , bShowLights_(false)
        , bDrawCube_(false)
        , bDrawPlane_(false)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    connect(&updateTimer_, SIGNAL(timeout()), this, SLOT(slotUpdateTimer()));
    updateTimer_.start(TIMEOUTSEC);
    elapsedTimer_.start();

    camera_.setPosition(QVector3D(0.0f, 0.0f, 5.0f));
    setEnvSettingType(ET_DESERT);
    setTestFuncType(DepthFuncType::DFT_LESS);
}

void ModelLoadingOWgt::initializeGL()
{
    initializeOpenGLFunctions();

    shaderProgram_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/shapes.vert");
    shaderProgram_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/shapes.frag");
    auto bSucess = shaderProgram_.link();
    if (!bSucess) {
        qDebug() << __FUNCTION__ << " " << shaderProgram_.log();
    }

    lightShaderProgram_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/lighting/lighting.vert");
    lightShaderProgram_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/lighting/lighting.frag");
    bSucess = lightShaderProgram_.link();
    if (!bSucess)
        qWarning() << __FUNCTION__ << " " << lightShaderProgram_.log();

//    loadModel("/Users/handabao/MyFile/code/QtOpengl/20_02_LoadMesh_ModelOOP/nanosuit/nanosuit.obj");
    auto funcs = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_4_1_Core>(QOpenGLContext::currentContext());
    if (!funcs) {
        qFatal("Could not obtain required OpenGL context version");
    }
    aLight_ = new LightModel(funcs, gPointLightPositions, gPointLightColors);
    aCube_ = new CubeModel(funcs);
    aPlane_ = new PlaneModel(funcs);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArrayAPPLE(0);
}

void ModelLoadingOWgt::resizeGL(int w, int h)
{
    Q_UNUSED(w);
    Q_UNUSED(h);
    // glViewport(0, 0, w, h);
}

void ModelLoadingOWgt::paintGL()
{
    glClearColor(clearColor_.x(), clearColor_.y(), clearColor_.z(), 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(testFunc_);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 view;
    QMatrix4x4 projection;
    view = camera_.getViewMatrix();
    projection.perspective(camera_.getZoom(),(float)width() / height(),0.1,100);

    if (aLight_ && bShowLights_)
    {
        lightShaderProgram_.bind();
        lightShaderProgram_.setUniformValue("projection", projection);
        lightShaderProgram_.setUniformValue("view", view);
        aLight_->Draw(lightShaderProgram_);
    }

    DrawShaderProgram(projection, view);
    if (aCube_ && bDrawCube_)
    {
        aCube_->Draw(shaderProgram_);
    }

    if (aPlane_ && bDrawPlane_)
    {
        aPlane_->Draw(shaderProgram_);
    }

    if(aModel_)
    {
        aModel_->Draw(shaderProgram_);
    }
}

ModelLoadingOWgt::~ModelLoadingOWgt() noexcept {

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
    if(event->buttons() & Qt::RightButton){
        static QPoint lastPos(width()/2,height()/2);
        auto currentPos=event->pos();
        QPoint deltaPos=currentPos-lastPos;
        lastPos=currentPos;

        camera_.processMouseMovement(deltaPos.x(),-deltaPos.y());
    }

    QWidget::mouseMoveEvent(event);
}

void ModelLoadingOWgt::wheelEvent(QWheelEvent *event) {
    camera_.processMouseScroll(event->angleDelta().y()/120);
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


void ModelLoadingOWgt::loadModel( const std::string path)
{
    if(aModel_ !=NULL)
        delete aModel_;

    aModel_=NULL;
//    makeCurrent();

    auto funcs = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_4_1_Core>(QOpenGLContext::currentContext());
    if (!funcs) {
        qFatal("Could not obtain required OpenGL context version");
    }


    aModel_ = new Model(funcs, path.c_str());
    camera_.setPosition(cameraPosInit(aModel_->maxY_,aModel_->minY_));

//    doneCurrent();
}

QVector3D ModelLoadingOWgt::cameraPosInit(float maxY, float minY)
{
    QVector3D temp = {0,0,0};
    float height = maxY-minY;
    temp.setZ(1.5 * height);
    if(minY >= 0)
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

void ModelLoadingOWgt::DrawShaderProgram(QMatrix4x4 projection, QMatrix4x4 view) {
    shaderProgram_.bind();
    shaderProgram_.setUniformValue("projection", projection);
    shaderProgram_.setUniformValue("view", view);
    shaderProgram_.setUniformValue("lightPos",gLightPos);
    shaderProgram_.setUniformValue("viewPos",camera_.getPosition());

    shaderProgram_.setUniformValue("material.shininess", 32.0f);
    shaderProgram_.setUniformValue("spotLight.position", camera_.getPosition());
    shaderProgram_.setUniformValue("spotL˜ight.direction", camera_.getFront());
    shaderProgram_.setUniformValue("spotLight.cutOff", static_cast<float>(cos(12.5f * M_PI / 180)));
    shaderProgram_.setUniformValue("spotLight.outerCutOff", (float)cos(17.5f * M_PI / 180));
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

    // point lights
    for(int i=0; i<4; i++){
        QString iStr="pointLights["+QString::number(i)+"].position";
        shaderProgram_.setUniformValue(iStr.toStdString().c_str(), gPointLightPositions[i].x(), gPointLightPositions[i].y(), gPointLightPositions[i].z());
        iStr="pointLights["+QString::number(i)+"].ambient";
        shaderProgram_.setUniformValue(iStr.toStdString().c_str(), gPointLightColors[i].x() * 0.1, gPointLightColors[i].y() * 0.1, gPointLightColors[i].z() * 0.1);
        iStr="pointLights["+QString::number(i)+"].diffuse";
        shaderProgram_.setUniformValue(iStr.toStdString().c_str(), gPointLightColors[i].x(), gPointLightColors[i].y(), gPointLightColors[i].z());
        iStr="pointLights["+QString::number(i)+"].specular";
        shaderProgram_.setUniformValue(iStr.toStdString().c_str(), gPointLightColors[i].x(), gPointLightColors[i].y(), gPointLightColors[i].z());
        iStr="pointLights["+QString::number(i)+"].constant";
        shaderProgram_.setUniformValue(iStr.toStdString().c_str(), 1.0f);
        iStr="pointLights["+QString::number(i)+"].linear";
        shaderProgram_.setUniformValue(iStr.toStdString().c_str(), 0.09f);
        iStr="pointLights["+QString::number(i)+"].quadratic";
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




