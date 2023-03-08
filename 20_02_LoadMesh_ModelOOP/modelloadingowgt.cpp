#include "modelloadingowgt.h"

#include <QKeyEvent>

/// TODO QtOpengl  glsl2.0  和 系统不兼容 导致的 assimp等一系列操作

#define TIMEOUTSEC 50
unsigned int VBO, VAO, EBO;
unsigned  int lightVAO;

QVector3D gLightPos(1.2f, 1.0f, 2.0f);
QVector3D gViewInitPos(0.0,0.0,5.0);

QVector3D gLightColor(1.0f, 1.0f, 1.0f);
QVector3D gObjectColor(1.0f, 0.5f, 0.31f);


float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f
};

QVector3D gCubePositions[] = {
        QVector3D( 0.0f, 0.0f, 0.0f),
        QVector3D( 2.0f, 5.0f, -15.0f),
        QVector3D(-1.5f, -2.2f, -2.5f),
        QVector3D(-3.8f, -2.0f, -12.3f),
        QVector3D( 2.4f, -0.4f, -3.5f),
        QVector3D(-1.7f, 3.0f, -7.5f),
        QVector3D( 1.3f, -2.0f, -2.5f),
        QVector3D( 1.5f, 2.0f, -2.5f),
        QVector3D( 1.5f, 0.2f, -1.5f),
        QVector3D(-1.3f, 1.0f, -1.5f)
};

QVector3D gPointLightPositions[] = {
        QVector3D( 0.7f, 0.2f, 2.0f),
        QVector3D( 2.3f, -3.3f, -4.0f),
        QVector3D(-4.0f, 2.0f, -12.0f),
        QVector3D( 0.0f, 0.0f, -3.0f)
};

QVector3D gPointLightColors[4];
QVector3D gPointLightColorsDesert[] = {
        QVector3D(1.0f, 0.6f, 0.0f),
        QVector3D(1.0f, 0.0f, 0.0f),
        QVector3D(1.0f, 1.0, 0.0),
        QVector3D(0.2f, 0.2f, 1.0f)
};
QVector3D gPointLightColorsFactory[] = {
        QVector3D(0.2f, 0.2f, 0.6f),
        QVector3D(0.3f, 0.3f, 0.7f),
        QVector3D(0.0f, 0.0f, 0.3f),
        QVector3D(0.4f, 0.4f, 0.4f)
};
QVector3D gPointLightColorsHorror[] = {
        QVector3D(0.1f, 0.1f, 0.1f),
        QVector3D(0.1f, 0.1f, 0.1f),
        QVector3D(0.1f, 0.1f, 0.1f),
        QVector3D(0.3f, 0.1f, 0.1f)
};

QVector3D gPointLightColorsBiochemicalLab[] = {
        QVector3D(1.0f, 0.6f, 0.0f),
        QVector3D(1.0f, 0.0f, 0.0f),
        QVector3D(1.0f, 1.0, 0.0),
        QVector3D(0.2f, 0.2f, 1.0f)
};

unsigned int indices[] = { // note that we start from 0!
                           0, 1, 3,  // first triangle
                           1, 2, 3   // second triangle
                         };

ModelLoadingOWgt::ModelLoadingOWgt(QWidget *parent):
        QOpenGLWidget{parent}
{
    setFocusPolicy(Qt::StrongFocus);
//    setMouseTracking(true);
    connect(&updateTimer_, SIGNAL(timeout()), this, SLOT(slotUpdateTimer()));
    updateTimer_.start(TIMEOUTSEC);
    elapsedTimer_.start();

    camera_.setPosition(gViewInitPos);
    setEnvSettingType(ET_DESERT);
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
}

void ModelLoadingOWgt::resizeGL(int w, int h)
{
    Q_UNUSED(w);
    Q_UNUSED(h);
    // glViewport(0, 0, w, h);
}

void ModelLoadingOWgt::paintGL()
{
    if (!backModel_)
        return;

    glClearColor(clearColor_.x(), clearColor_.y(), clearColor_.z(), 1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 model;
    QMatrix4x4 view;
    QMatrix4x4 projection;
    view = camera_.getViewMatrix();
    projection.perspective(camera_.getZoom(),(float)width() / height(),0.1,100);

    shaderProgram_.bind();
    shaderProgram_.setUniformValue("projection", projection);
    shaderProgram_.setUniformValue("view", view);
    shaderProgram_.setUniformValue("model", model);
    shaderProgram_.setUniformValue("viewPos",camera_.getPosition());

    shaderProgram_.setUniformValue("light.ambient", 0.4f, 0.4f, 0.4f);
    shaderProgram_.setUniformValue("light.diffuse", 0.9f, 0.9f, 0.9f);
    shaderProgram_.setUniformValue("light.specular", 1.0f, 1.0f, 1.0f);
    // material properties
    shaderProgram_.setUniformValue("material.shininess", 32.0f);
    shaderProgram_.setUniformValue("light.direction", -0.2f, -1.0f, -0.3f);
    backModel_->Draw(shaderProgram_);
}

ModelLoadingOWgt::~ModelLoadingOWgt() noexcept {
    
    if (!isValid()) {
        return;
    }
    
    makeCurrent();
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArraysAPPLE(1, &VAO);
    doneCurrent();
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
            for(int i=0; i<4; i++)
                gPointLightColors[i] = gPointLightColorsDesert[i];
            break;
        case EnvironmentType::ET_FACTTORY:
            clearColor_ = QVector3D(0.0f, 0.0f, 0.0f);
            for(int i=0; i<4; i++)
                gPointLightColors[i] = gPointLightColorsFactory[i];
            break;
        case EnvironmentType::ET_HORROR:
            clearColor_ = QVector3D(0.0f, 0.0f, 0.0f);
            for(int i=0; i<4; i++)
                gPointLightColors[i] = gPointLightColorsHorror[i];
            break;
        case EnvironmentType::ET_BIOCHEMICALLAB:
            clearColor_ = QVector3D(0.9f, 0.9f, 0.9f);
            for(int i=0; i<4; i++)
                gPointLightColors[i] = gPointLightColorsBiochemicalLab[i];
            break;
        default:
            break;
    }
}


void ModelLoadingOWgt::loadModel(const std::string &path) {
    if(backModel_ != nullptr)
        delete backModel_;

    backModel_ = nullptr;
    makeCurrent();
    backModel_ = new Model(QOpenGLContext::currentContext()->functions()
            ,path.c_str());

    camera_.setPosition( cameraPosInit(backModel_->maxY_,backModel_->minY_) );
    doneCurrent();
}

QVector3D ModelLoadingOWgt::cameraPosInit(float maxY, float minY) {

    QVector3D temp = {0.0f,0.0f,0.0f};
    float height=maxY-minY;
    temp.setZ(1.5 * height);
    if(minY >= 0)
        temp.setY(height / 2.0f);
    gViewInitPos = temp;
    return temp;
}






