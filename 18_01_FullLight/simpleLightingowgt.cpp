#include "simpleLightingowgt.h"

#include <QKeyEvent>

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

QVector3D gPointLightColors[] = {
        QVector3D(1.0f, 1.0f, 0.0f),
        QVector3D(0.3f, 0.3f, 0.7f),
        QVector3D(0.0f, 0.0f, 0.3f),
        QVector3D(0.4f, 0.4f, 0.4f)
};

unsigned int indices[] = { // note that we start from 0!
                           0, 1, 3,  // first triangle
                           1, 2, 3   // second triangle
                         };

SimpleLightingOWgt::SimpleLightingOWgt(QWidget *parent):
        QOpenGLWidget{parent}
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    connect(&updateTimer_, SIGNAL(timeout()), this, SLOT(slotUpdateTimer()));
    updateTimer_.start(TIMEOUTSEC);
    elapsedTimer_.start();

    camera_.setPosition(QVector3D(0.0f, 0.0f, 5.0f));
}

void SimpleLightingOWgt::initializeGL()
{
    initializeOpenGLFunctions();

    /// 创建VBO与VAO对象 ，并赋予ID
    glGenVertexArraysAPPLE(1, &VAO);
    glGenBuffers(1, &VBO);

    /// 绑定VBO与VAO对象
    glBindVertexArrayAPPLE(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    /// 为当前绑定到target的缓冲区对象创建一个新的数据存储
    /// 如果data不是NULL， 则使用来自此指针的数据化存储
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /// 告知显卡如何解析缓冲里的属性值
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    /// 开启VAO管理的第一个属性值
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    /// 开启VAO管理的第一个属性值
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    /// 开启VAO管理的第一个属性值
    glEnableVertexAttribArray(2);

    shaderProgram_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/shapes.vert");
    shaderProgram_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/shapes.frag");
    auto bSucess = shaderProgram_.link();
    if (!bSucess)
        qWarning() << __FUNCTION__ << " " << shaderProgram_.log();

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    diffuseTexture_ = new QOpenGLTexture(QImage(":/image/texMap/container2.png").mirrored());
    specularTexture_ = new QOpenGLTexture(QImage(":/image/texMap/lighting_maps_specular_color.png").mirrored());
    emissionTexture_ = new QOpenGLTexture(QImage(":/image/texMap/matrix.jpg").mirrored());

    shaderProgram_.bind();
    shaderProgram_.setUniformValue("material.diffuse", 0);
    shaderProgram_.setUniformValue("material.specular", 1);
    shaderProgram_.setUniformValue("material.emission", 2);

    glGenVertexArraysAPPLE(1, &lightVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArrayAPPLE(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    lightShaderProgram_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/lighting/lighting.vert");
    lightShaderProgram_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/lighting/lighting.frag");
    bSucess = lightShaderProgram_.link();
    if (!bSucess)
        qWarning() << __FUNCTION__ << " " << lightShaderProgram_.log();

    lightShaderProgram_.bind();
    lightShaderProgram_.setUniformValue("lightColor",gLightColor);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArrayAPPLE(0);
}

void SimpleLightingOWgt::resizeGL(int w, int h)
{
    Q_UNUSED(w);
    Q_UNUSED(h);
    // glViewport(0, 0, w, h);
}

void SimpleLightingOWgt::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 model;
    QMatrix4x4 view;
    QMatrix4x4 projection;
    float time = elapsedTimer_.elapsed() / 50.0;
    view = camera_.getViewMatrix();
    projection.perspective(camera_.getZoom(),(float)width()/height(),0.1,100);
    diffuseTexture_->bind(0);
    specularTexture_->bind(1);
    emissionTexture_->bind(2);

    glBindVertexArrayAPPLE(VAO);
    /// positions all containers
    for(unsigned int i = 0; i < 10; i++) {
        QMatrix4x4 model;
        model.translate(gCubePositions[i]);
        float angle = 20.0f * i;
        model.rotate(angle, QVector3D(1.0f, 0.3f, 0.5f));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        shaderProgram_.bind();
        shaderProgram_.setUniformValue("model", model);
    }

    shaderProgram_.bind();
    shaderProgram_.setUniformValue("projection", projection);
    shaderProgram_.setUniformValue("view", view);
    shaderProgram_.setUniformValue("lightPos",gLightPos);
    shaderProgram_.setUniformValue("viewPos",camera_.getPosition());

    shaderProgram_.setUniformValue("material.shininess", 32.0f);
    shaderProgram_.setUniformValue("spotLight.position", camera_.getPosition());
    shaderProgram_.setUniformValue("spotLight.direction", camera_.getFront());
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
    shaderProgram_.setUniformValue("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    shaderProgram_.setUniformValue("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    shaderProgram_.setUniformValue("dirLight.specular", 0.5f, 0.5f, 0.5f);

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

    model.setToIdentity();
    model.translate(gLightPos);
    model.rotate(1.0, 1.0f, 1.0f, 0.5f);
    model.scale(0.2);
    glBindVertexArray(lightVAO);
    for(int i=0;i<4;i++){
        model.setToIdentity();
        model.translate(gPointLightPositions[i]);
        model.scale(0.2);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawArrays(GL_TRIANGLES,0,36);

        lightShaderProgram_.bind();
        lightShaderProgram_.setUniformValue("model", model);
        lightShaderProgram_.setUniformValue("lightColor",gPointLightColors[i]);
    }

    lightShaderProgram_.bind();
    lightShaderProgram_.setUniformValue("projection", projection);
    lightShaderProgram_.setUniformValue("view", view);
}

SimpleLightingOWgt::~SimpleLightingOWgt() noexcept {
    
    if (!isValid()) {
        return;
    }
    
    makeCurrent();
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArraysAPPLE(1, &VAO);
    doneCurrent();
}

void SimpleLightingOWgt::setWireframe(bool wireframe) {
    makeCurrent();
    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    doneCurrent();
    update();
}

void SimpleLightingOWgt::keyPressEvent(QKeyEvent *event) {
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

void SimpleLightingOWgt::slotUpdateTimer() {
    update();
}

void SimpleLightingOWgt::mouseMoveEvent(QMouseEvent *event) {
    if(event->buttons() & Qt::RightButton){
        static QPoint lastPos(width()/2,height()/2);
        auto currentPos=event->pos();
        QPoint deltaPos=currentPos-lastPos;
        lastPos=currentPos;

        camera_.processMouseMovement(deltaPos.x(),-deltaPos.y());
    }

    QWidget::mouseMoveEvent(event);
}

void SimpleLightingOWgt::wheelEvent(QWheelEvent *event) {
    camera_.processMouseScroll(event->angleDelta().y()/120);
    QWidget::wheelEvent(event);
}






