#include "helloopenglwidget.h"

#include <QKeyEvent>

#define TIMEOUTSEC 50
unsigned int VBO, VAO, EBO;

float vertices[] = {
-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
 0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
 0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

 0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
 0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
 0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
 0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
 0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
 0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
 0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
 0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
 0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
 0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
};

QVector<QVector3D> cubePositions= {
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

unsigned int indices[] = { // note that we start from 0!
                           0, 1, 3,  // first triangle
                           1, 2, 3   // second triangle
                         };

auto gRatio = 0.5f;
QPoint gDeltaPos;

// Custom implementation of the LookAt function
QMatrix4x4 calculate_lookAt_matrix(QVector3D position, QVector3D target, QVector3D worldUp);

HelloOpenGLWidget::HelloOpenGLWidget(QWidget *parent):
        QOpenGLWidget{parent}
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    connect(&updateTimer_, SIGNAL(timeout()), this, SLOT(slotUpdateTimer()));
    updateTimer_.start(TIMEOUTSEC);
    elapsedTimer_.start();

    camera_.setPosition(QVector3D(0.0f, 0.0f, 3.0f));
}

void HelloOpenGLWidget::initializeGL()
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    /// 开启VAO管理的第一个属性值
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    shaderProgram_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/shapes.vs");
    shaderProgram_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/shapes.fs");
    auto bSucess = shaderProgram_.link();
    if (bSucess) {}
    else {
        qWarning() << __FUNCTION__ << " " << shaderProgram_.log();
    }

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    textureWall_ = new QOpenGLTexture(QImage(":image/images/wall.jpg").mirrored());
    textureSmile_ = new QOpenGLTexture(QImage(":image/images/awesomeface.png").mirrored());
    textureSmall_ = new QOpenGLTexture(QImage(":image/images/small.png").mirrored());

    shaderProgram_.bind();
    shaderProgram_.setUniformValue("textureWall", 0);
    shaderProgram_.setUniformValue("textureSmile", 1);
    shaderProgram_.setUniformValue("textureSmall", 2);
    textureSmall_->generateMipMaps();

    textureSmile_->bind(1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArrayAPPLE(0);
}

void HelloOpenGLWidget::resizeGL(int w, int h)
{
    Q_UNUSED(w);
    Q_UNUSED(h);
    // glViewport(0, 0, w, h);
}

void HelloOpenGLWidget::paintGL()
{
    QMatrix4x4 model;
    QMatrix4x4 view;
    QMatrix4x4 projection;
    float time = elapsedTimer_.elapsed()/1000.0;
//    view = camera_.getViewMatrix();
    view = calculate_lookAt_matrix(camera_.getPosition(), camera_.getPosition() + camera_.getFront(), camera_.getUp());

    projection.perspective(camera_.getZoom(),(float)width()/height(),0.1,100);
    shaderProgram_.setUniformValue("projection", projection);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    shaderProgram_.bind();
    shaderProgram_.setUniformValue("ratio", gRatio);
    glBindVertexArrayAPPLE(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    switch (shape_) {
        case Rect:
        {
            // glActiveTexture(GL_TEXTURE0);
             textureWall_->bind(0);
             //glActiveTexture(GL_TEXTURE1);
             textureSmile_->bind(1);
             textureSmall_->bind(2);
            
            shaderProgram_.setUniformValue("view", view);
            
            foreach (auto item, cubePositions) {
                model.setToIdentity();
                model.translate(item);
                model.rotate(time, 1.0f, 5.0f, 0.5f);
                shaderProgram_.setUniformValue("model", model);
                glDrawArrays(GL_TRIANGLES,0,36);
            }
            

            break;
        }
        case Circle:
            
            break;
            
        case Triangle:
            
            break;
            
        default:
            break;
    }
    
}

HelloOpenGLWidget::~HelloOpenGLWidget() noexcept {
    
    if (!isValid()) {
        return;
    }
    
    makeCurrent();
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArraysAPPLE(1, &VAO);
    doneCurrent();
}

void HelloOpenGLWidget::drawShape(HelloOpenGLWidget::Shape shape) {
    shape_ = shape;
    update();
}

void HelloOpenGLWidget::setWireframe(bool wireframe) {
    makeCurrent();
    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    doneCurrent();
    update();
}

void HelloOpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    float deltaTime= TIMEOUTSEC/ 1000.0;
    switch (event->key())
    {
        case Qt::Key_Up: gRatio += 0.1f; break;
        case Qt::Key_Down: gRatio -= 0.1f; break;
        case Qt::Key_W:
            camera_.processKeyboard(FORWARD, deltaTime);break;
        case Qt::Key_S:
            camera_.processKeyboard(BACKWARD, deltaTime);break;
        case Qt::Key_D:
            camera_.processKeyboard(RIGHT, deltaTime);break;
        case Qt::Key_A:
            camera_.processKeyboard(LEFT, deltaTime);break;
        default:
            break;
    };

    if (gRatio > 1) gRatio = 1;
    if (gRatio < 0) gRatio = 0;

    shaderProgram_.bind();
    shaderProgram_.setUniformValue("ratio", gRatio);
    update();
    
}

void HelloOpenGLWidget::slotUpdateTimer() { 
    update();
}

void HelloOpenGLWidget::mouseMoveEvent(QMouseEvent *event) {

    static  QPoint lastPos(width()/ 2, height()/ 2);
    auto currentPos = event->pos();
    gDeltaPos = currentPos - lastPos;
    lastPos = currentPos;

    camera_.processMouseMovement(gDeltaPos.x(), -gDeltaPos.y());
    update();

    QWidget::mouseMoveEvent(event);
}

void HelloOpenGLWidget::wheelEvent(QWheelEvent *event) {
    camera_.processMouseScroll(event->angleDelta().y()/120);
    update();

    QWidget::wheelEvent(event);
}


QMatrix4x4 calculate_lookAt_matrix(QVector3D position, QVector3D target, QVector3D worldUp) {
// 1. Position = known
// 2. Calculate cameraDirection
    QVector3D zaxis = QVector3D(position - target);
    zaxis.normalize();
// 3. Get positive right axis vector
    worldUp.normalize();
    QVector3D xaxis = QVector3D::crossProduct(worldUp, zaxis);
    xaxis.normalize();
// 4. Calculate camera up vector
    QVector3D yaxis = QVector3D::crossProduct(zaxis, xaxis); // Create translation and rotation matrix
    QMatrix4x4 translation; // Identity matrix by default
    QVector4D theVector4D;
    theVector4D.setX(-position.x()); // Third column, first row
    theVector4D.setY(-position.y());
    theVector4D.setZ(-position.z());
    theVector4D.setW(1.0);
    translation.setColumn(3,theVector4D);
    QMatrix4x4 rotation ;
    theVector4D.setX(xaxis.x()); // First column, first row
    theVector4D.setY(xaxis.y());
    theVector4D.setZ(xaxis.z());
    theVector4D.setW(0.0);
    rotation.setColumn(0,theVector4D);
    theVector4D.setX(yaxis.x()); // Secon column, second row
    theVector4D.setY(yaxis.y());
    theVector4D.setZ(yaxis.z());
    theVector4D.setW(0.0);
    rotation.setColumn(1,theVector4D);
    theVector4D.setX(zaxis.x()); // First column, third row
    theVector4D.setY(zaxis.y());
    theVector4D.setZ(zaxis.z());
    theVector4D.setW(0.0);
    rotation.setColumn(2,theVector4D); // Return lookAt matrix as combination of translation and rotation matrix
    return rotation * translation;
}



