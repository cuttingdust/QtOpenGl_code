#include "camera.h"

Camera::Camera(QVector3D position, QVector3D up, float yaw, float pitch) :
front_(QVector3D(0.0f, 0.0f, -1.0f))
, movementSpeed_(SPEED)
, mouseSensitivity_(SENSITIVITY)
, zoom_(ZOOM)
{
    position_ = position;
    worldUp_ = up;
    yaw_ = yaw;
    pitch_ = pitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch):
front_(QVector3D(0.0f, 0.0f, -1.0f))
, movementSpeed_(SPEED)
, mouseSensitivity_(SENSITIVITY)
, zoom_(ZOOM)
{
    position_ = QVector3D(posX, posY, posZ);;
    worldUp_ = QVector3D(upX, upY, upZ);;
    yaw_ = yaw;
    pitch_ = pitch;
    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    // calculate the new front_ vector
    QVector3D front;
    front.setX(cos(yaw_ * M_PI / 180.0) * cos(pitch_ * M_PI / 180.0));
    front.setY( sin(pitch_ * M_PI / 180.0));
    front.setZ(sin(yaw_ * M_PI / 180.0) * cos(pitch_ * M_PI / 180.0));
    front.normalize();
    front_ = front;
    // also re-calculate the right_ and up_ vector
    right_ = QVector3D::crossProduct(front_, worldUp_);
    // 标准化向量，因为向上或向下看得越多，向量的长度就越接近0，这会导致移动速度变慢。
    right_.normalize();
    up_    = QVector3D::crossProduct(right_, front_);
    up_.normalize();
}

QMatrix4x4 Camera::getViewMatrix() const {
    QMatrix4x4 theMatrix;
    theMatrix.lookAt(position_, position_ + front_, up_);
    return theMatrix;
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = movementSpeed_ * deltaTime;
    if (direction == FORWARD)
        position_ += front_ * velocity;
    if (direction == BACKWARD)
        position_ -= front_ * velocity;
    if (direction == LEFT)
        position_ -= right_ * velocity;
    if (direction == RIGHT)
        position_ += right_ * velocity;
    if (direction == DOWN)
        position_ -= up_ * velocity;
    if (direction == UP)
        position_ += up_ * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= mouseSensitivity_;
    yoffset *= mouseSensitivity_;

    yaw_   += xoffset;
    pitch_ += yoffset;

    // 确保当投球超出边界时，屏幕不会翻转
    if (constrainPitch)
    {
        if (pitch_ > 89.0f)
            pitch_ = 89.0f;
        if (pitch_ < -89.0f)
            pitch_ = -89.0f;
    }

    // 使用更新的Euler角度更新前、右和上矢量
    updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset) {
    zoom_ -= (float)yoffset;
    if (zoom_ < 1.0f)
        zoom_ = 1.0f;
    if (zoom_ > 75.0f)
        zoom_ = 75.0f;
}

void Camera::setPosition(const QVector3D &position) {
    position_ = position;
}

float Camera::getZoom() const {
    return zoom_;
}

void Camera::setZoom(float zoom) {
    zoom_ = zoom;
}

const QVector3D &Camera::getPosition() const {
    return position_;
}
