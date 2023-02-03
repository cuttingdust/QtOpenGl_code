#include "helloopenglwidget.h"

unsigned int VBO, VAO, EBO;
unsigned int shaderProgram;
//float vertices[] = {
//    /// first triangle
//    0.5f, 0.5f,0.0f,    /// top right
//    0.5f, -0.5f, 0.0f,  /// bottom right
//    -0.5f, 0.5f, 0.0f,  /// top left
//    /// second triangle
//    0.5f, -0.5f, 0.0f,
//    -0.5f, -0.5f, 0.0f,
//    -0.5f, 0.5f, 0.0f
//};

float vertices[] = {
    /// first triangle
    0.5f, 0.5f,0.0f,    /// top right
    0.5f, -0.5f, 0.0f,  /// bottom right
    -0.5f, -0.5f, 0.0f, /// bottom left
    -0.5f, 0.5f, 0.0f,  /// top left
   
};

unsigned int indices[] = {
    0, 1, 3, /// first triangle
    1, 2, 3 /// second triangle
};

const char *vertexShaderSource =
"attribute vec3 aPos;\n"
"void main(){\n"
"  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n\0";


const char *fragmentShaderSource =
"void main(){\n"
"   gl_FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
"}\n\0";

HelloOpenGLWidget::HelloOpenGLWidget(QWidget *parent)
: QOpenGLWidget{parent}
{
    
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    /// 开启VAO管理的第一个属性值
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
   
    
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    /// check fot shader complie errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog;
    }
    
    
    
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    /// check fot shader complie errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog;
    }
    
    
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    /// check fot link errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(fragmentShader, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArrayAPPLE(0);
    
}

void HelloOpenGLWidget::resizeGL(int w, int h)
{
    Q_UNUSED(w);
    Q_UNUSED(h);
//    glViewport(0, 0, w, h);
}

void HelloOpenGLWidget::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(shaderProgram);
    glBindVertexArrayAPPLE(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0/* &indices */);
}
