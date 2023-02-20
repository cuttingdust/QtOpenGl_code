#ifndef HELLOOPENGLWIDGET_H
#define HELLOOPENGLWIDGET_H

#include <QWidget>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include <QTimer>
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

    
signals:
    
private:
    Shape shape_;
    QOpenGLShaderProgram shaderProgram_;
    QOpenGLTexture* textureWall_;
    QOpenGLTexture* textureSmile_;
    
    
    
    
};

#endif // HELLOOPENGLWIDGET_H
