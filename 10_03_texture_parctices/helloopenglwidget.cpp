#include "helloopenglwidget.h"
#include <QTime>
#include <QKeyEvent>

unsigned int VBO, VAO, EBO;

float vertices[] = {
    0.9f, 0.9f, 0.0f,  1.0f, 1.0f, // top right
    0.9f, -0.9f, 0.0f, 1.0f, 0.0f, // bottom right
    -0.9f, -0.9f, 0.0f, 0.0f, 0.0f, // bottom left
    -0.9f, 0.9f, 0.0f, 0.0f, 1.0f // top left
};

unsigned int indices[] = {
    0, 1, 3, /// first triangle
    1, 2, 3 /// second triangle
};

float gRatio = 0.5f;

HelloOpenGLWidget::HelloOpenGLWidget(QWidget *parent)
: QOpenGLWidget{parent}
{
    setFocusPolicy(Qt::StrongFocus);
}

void HelloOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    
    shaderProgram_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/shapes.vs");
    shaderProgram_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/shapes.fs");
    bool bSucess = shaderProgram_.link();
    if (!bSucess) {
        qWarning() << __FUNCTION__ << " " << shaderProgram_.log();
    }
    
    
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
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArrayAPPLE(0);
    
    textureSmile_->bind(1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
}

void HelloOpenGLWidget::resizeGL(int w, int h)
{
    Q_UNUSED(w);
    Q_UNUSED(h);
    glViewport(0, 0, w, h);
}

void HelloOpenGLWidget::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    shaderProgram_.bind();
    shaderProgram_.setUniformValue("ratio", gRatio);
    glBindVertexArrayAPPLE(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    switch (shape_) {
        case Rect:
        {
            textureSmall_->bind(2);
            
            // glActiveTexture(GL_TEXTURE0);
             textureWall_->bind(0);
             //glActiveTexture(GL_TEXTURE1);
             textureSmile_->bind(1);
             textureSmall_->bind(2);
            
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0/* &indices */);
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
    switch (event->key())
    {
        case Qt::Key_Up:
            gRatio += 0.1f;
            break;
        case Qt::Key_Down:
            gRatio -= 0.1f;
            break;
            
        default:
            break;
    };
    
    if (gRatio > 1) {
        gRatio = 1;
    }
    
    if (gRatio < 0) {
        gRatio = 0;
    }
    
    shaderProgram_.bind();
    shaderProgram_.setUniformValue("ratio", gRatio);
    update();
    
}





