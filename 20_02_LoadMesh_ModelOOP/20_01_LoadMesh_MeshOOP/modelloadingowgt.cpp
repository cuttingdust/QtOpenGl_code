#include "modelloadingowgt.h"

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
    setMouseTracking(true);
    connect(&updateTimer_, SIGNAL(timeout()), this, SLOT(slotUpdateTimer()));
    updateTimer_.start(TIMEOUTSEC);
    elapsedTimer_.start();

    camera_.setPosition(QVector3D(0.0f, 0.0f, 5.0f));
    setEnvSettingType(ET_DESERT);
}

void ModelLoadingOWgt::initializeGL()
{
    initializeOpenGLFunctions();

    /// 图片加载
    diffuseTexture_ = new QOpenGLTexture(QImage(":/image/texMap/container2.png").mirrored());
    specularTexture_ = new QOpenGLTexture(QImage(":/image/texMap/lighting_maps_specular_color.png").mirrored());
    emissionTexture_ = new QOpenGLTexture(QImage(":/image/texMap/matrix.jpg").mirrored());

    mesh_ = processMesh();

    shaderProgram_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/shapes.vert");
    shaderProgram_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/shapes.frag");
    auto bSucess = shaderProgram_.link();
    if (!bSucess) {
        qDebug() << __FUNCTION__ << " " << shaderProgram_.log();
    }

    lightMesh_ = processMesh();

    lightShaderProgram_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/lighting/lighting.vert");
    lightShaderProgram_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/lighting/lighting.frag");
    bSucess = lightShaderProgram_.link();
    if (!bSucess)
        qWarning() << __FUNCTION__ << " " << lightShaderProgram_.log();

    lightShaderProgram_.bind();
    lightShaderProgram_.setUniformValue("lightColor",gLightColor);


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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 model;
    QMatrix4x4 view;
    QMatrix4x4 projection;
    view = camera_.getViewMatrix();
    projection.perspective(camera_.getZoom(),(float)width() / height(),0.1,100);

    /// positions all containers
    for(unsigned int i = 0; i < 10; i++) {
        QMatrix4x4 model;
        model.translate(gCubePositions[i]);
        float angle = 20.0f * i;
        model.rotate(angle, QVector3D(1.0f, 0.3f, 0.5f));
        mesh_->Draw(shaderProgram_);
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

    model.setToIdentity();
    model.translate(gLightPos);
    model.rotate(1.0, 1.0f, 1.0f, 0.5f);
    model.scale(0.2);
    for(int i=0;i<4;i++){
        model.setToIdentity();
        model.translate(gPointLightPositions[i]);
        model.scale(0.2);
        lightMesh_->Draw(lightShaderProgram_);

        lightShaderProgram_.bind();
        lightShaderProgram_.setUniformValue("model", model);
        lightShaderProgram_.setUniformValue("lightColor",gPointLightColors[i]);
    }

    lightShaderProgram_.bind();
    lightShaderProgram_.setUniformValue("projection", projection);
    lightShaderProgram_.setUniformValue("view", view);
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
            for(int i=0; i<4; i++)
                gPointLightColors[i] = gPointLightColorsDesert[i];
            break;
        case EnvironmentType::ET_FACTTORY:
            clearColor_ = QVector3D(0.0f, 0.0f, 0.0f);
            dirlightAmbient_ = QVector3D(0.05f, 0.05f, 0.1f);
            dirlightDiffuse_ = QVector3D(0.2f, 0.2f, 0.7);
            dirlightSpecular_ = QVector3D(0.7f, 0.7f, 0.7f);
            for(int i=0; i<4; i++)
                gPointLightColors[i] = gPointLightColorsFactory[i];
            break;
        case EnvironmentType::ET_HORROR:
            clearColor_ = QVector3D(0.0f, 0.0f, 0.0f);
            dirlightAmbient_ = QVector3D(0.0f, 0.0f, 0.0f);
            dirlightDiffuse_ = QVector3D(0.05f, 0.05f, 0.05);
            dirlightSpecular_ = QVector3D(0.2f, 0.2f, 0.2f);
            for(int i=0; i<4; i++)
                gPointLightColors[i] = gPointLightColorsHorror[i];
            break;
        case EnvironmentType::ET_BIOCHEMICALLAB:
            clearColor_ = QVector3D(0.9f, 0.9f, 0.9f);
            dirlightAmbient_ = QVector3D(0.5f, 0.5f, 0.5f);
            dirlightDiffuse_ = QVector3D(1.0f, 1.0f, 1.0f);
            dirlightSpecular_ = QVector3D(1.0f, 1.0f, 1.0f);
            for(int i=0; i<4; i++)
                gPointLightColors[i] = gPointLightColorsBiochemicalLab[i];
            break;
        default:
            break;
    }
}

Mesh *ModelLoadingOWgt::processMesh() {
    std::vector<Vertex> _vertices(36);
    std::vector<unsigned int> _indices;
    std::vector<Texture> _textures;
    // _vertices.reserve(36);
    // _vertices.resize(36);
    memcpy(&_vertices[0],vertices,sizeof(vertices));
    for(int i=0;i<36;i++){
        // Vertex vert;
        // vert.Position[0]=vertices[i*8+0];
        // vert.Position[1]=vertices[i*8+1];
        // vert.Position[2]=vertices[i*8+2];
        // vert.Normal[0]=vertices[i*8+3];
        // vert.Normal[1]=vertices[i*8+4];
        // vert.Normal[2]=vertices[i*8+5];
        // vert.TexCoords[0]=vertices[i*8+6];
        // vert.TexCoords[1]=vertices[i*8+7];
        // _vertices.push_back(vert);
        _indices.push_back(i);
    }

    Texture tex;
    tex.id_= diffuseTexture_->textureId();
    tex.type_ = "texture_diffuse";
    _textures.push_back(tex);
    tex.id_ = specularTexture_->textureId();
    tex.type_ = "texture_specular";
    _textures.push_back(tex);
    return new Mesh(QOpenGLContext::currentContext()->functions()
            ,_vertices,_indices,_textures);

}






