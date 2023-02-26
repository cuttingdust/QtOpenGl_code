#ifndef __CAMERA_H__
#define __CAMERA_H__

#include<QMatrix4x4>

#include <vector>
#include <QtGui/QtGui>

// 移动方向枚举量. 是一种抽象，以避开特定于窗口系统的输入方法
// 我们这里是WSAD
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// 默认值
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


// 一个抽象的camera类，用于处理输入并计算相应的Euler角度、向量和矩阵，以便在OpenGL中使用
class Camera
{

private:
    // camera Attributes
    QVector3D position_;
    QVector3D front_;
    QVector3D up_;
    QVector3D right_;
    QVector3D worldUp_;
    // euler Angles
    float yaw_;
    float pitch_;
    // camera options
    float movementSpeed_;
    float mouseSensitivity_;
    float zoom_;
public:
    void setPosition(const QVector3D &position);

    const QVector3D &getPosition() const;

    float getZoom() const;

    void setZoom(float zoom);

    const QVector3D &getFront() const;

    void setFront(const QVector3D &front);

    const QVector3D &getUp() const;

    void setUp(const QVector3D &up);

    float getMovementSpeed() const;

    void setMovementSpeed(float movementSpeed);

    float getMouseSensitivity() const;

    void setMouseSensitivity(float mouseSensitivity);

public:
    // constructor with vectors
    explicit Camera(QVector3D position = QVector3D(0.0f, 0.0f, 0.0f), QVector3D up = QVector3D(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

    // constructor with scalar values
    explicit Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    QMatrix4x4 getViewMatrix() const;

    // 处理从任何类似键盘的输入系统接收的输入。接受摄像机定义枚举形式的输入参数（从窗口系统中提取）
    void processKeyboard(Camera_Movement direction, float deltaTime);

    // 处理从鼠标输入系统接收的输入。需要x和y方向上的偏移值。
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

    // 处理从鼠标滚轮事件接收的输入。仅需要在垂直车轮轴上输入
    void processMouseScroll(float yoffset);

private:
    // 根据相机的（更新的）Euler角度计算前矢量
    void updateCameraVectors();
};
#endif
