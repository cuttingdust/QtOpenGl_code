#ifndef HELLOOPENGLWIDGET_H
#define HELLOOPENGLWIDGET_H

#include <QWidget>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include <QTimer>
#include <QTime>
#include <QElapsedTimer>
#include <QImage>

class HelloOpenGLWidget : public QOpenGLWidget,  QOpenGLFunctions
{
    Q_OBJECT
public:
    enum Shape {
        None,
        Rect,
        Circle,
        Triangle
    };
    
    explicit HelloOpenGLWidget(QWidget *parent = nullptr);
    
    ~HelloOpenGLWidget();
    
    void drawShape(Shape shape);
    
    void setWireframe(bool wireframe);
    
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent* event) override;
    
protected slots:
    void slotUpdateTimer();
    
signals:
    
private:
    Shape shape_;
    QOpenGLShaderProgram shaderProgram_;
    QOpenGLTexture* textureWall_;
    QOpenGLTexture* textureSmile_;
    QOpenGLTexture* textureSmall_;
    QTimer timer_;
    QElapsedTimer elapsedTimer_;
    
    
    QVector3D cameraPos_;
    QVector3D camerTarget_;
    QVector3D cameraDirection_;
    QVector3D up_;
    QVector3D cameraRight_;
    QVector3D cameraUp_;
    
    
    
};

#endif // HELLOOPENGLWIDGET_H
