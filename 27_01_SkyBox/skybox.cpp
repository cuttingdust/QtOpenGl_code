#include "skybox.h"
#include "verticesData.h"

SkyBox::SkyBox(QOpenGLFunctions_4_1_Core *glFuns)
    : glFuns_(glFuns)
{
    createSkyBoxTex();
    setupSkyBox();
}

void SkyBox::setupSkyBox()
{
    // skybox VAO
    glFuns_->glGenVertexArrays(1, &skyboxVAO_);
    glFuns_->glGenBuffers(1, &skyboxVBO_);
    glFuns_->glBindVertexArray(skyboxVAO_);
    glFuns_->glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO_);
    glFuns_->glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glFuns_->glEnableVertexAttribArray(0);
    glFuns_->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}


void SkyBox::Draw(QOpenGLShaderProgram &shader)
{
// draw skybox as last
    glFuns_->glDepthFunc(GL_LEQUAL); // 确保天空盒=1也能通过测试
    // skybox cube
    glFuns_->glBindVertexArray(skyboxVAO_);
    glFuns_->glActiveTexture(GL_TEXTURE0);
    glFuns_->glBindTexture(GL_TEXTURE_CUBE_MAP, skyCubeMap_->textureId());
    glFuns_->glDrawArrays(GL_TRIANGLES, 0, 36);
    glFuns_->glBindVertexArray(0);
    glFuns_->glDepthFunc(GL_LESS); // set depth function back to default
}

SkyBox::~SkyBox() {
    glFuns_->glDeleteVertexArrays(1, &skyboxVAO_);
    glFuns_->glDeleteBuffers(1, &skyboxVBO_);
}

void SkyBox::createSkyBoxTex() {
    skyCubeMap_ = new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);
    QImage _right   = QImage(":/image/skybox/right.jpg").convertToFormat(QImage::Format_RGB888);
    QImage _left    = QImage(":/image/skybox/left.jpg").convertToFormat(QImage::Format_RGB888);
    QImage _top     = QImage(":/image/skybox/top.jpg").convertToFormat(QImage::Format_RGB888);
    QImage _bottom  = QImage(":/image/skybox/bottom.jpg").convertToFormat(QImage::Format_RGB888);
    QImage _front   = QImage(":/image/skybox/front.jpg").convertToFormat(QImage::Format_RGB888);
    QImage _back    = QImage(":/image/skybox/back.jpg").convertToFormat(QImage::Format_RGB888);
    skyCubeMap_->setSize(_right.width(), _right.height());
    skyCubeMap_->setFormat(QOpenGLTexture::RGBFormat);
    skyCubeMap_->allocateStorage(QOpenGLTexture::RGB, QOpenGLTexture::UInt8);
    skyCubeMap_->setData(0, 0, QOpenGLTexture::CubeMapPositiveX,QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)_right.bits());
    skyCubeMap_->setData(0, 0, QOpenGLTexture::CubeMapNegativeX,QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)_left.bits());
    skyCubeMap_->setData(0, 0, QOpenGLTexture::CubeMapPositiveY,QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)_top.bits());
    skyCubeMap_->setData(0, 0, QOpenGLTexture::CubeMapNegativeY,QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)_bottom.bits());
    skyCubeMap_->setData(0, 0, QOpenGLTexture::CubeMapPositiveZ,QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)_front.bits());
    skyCubeMap_->setData(0, 0, QOpenGLTexture::CubeMapNegativeZ,QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)_back.bits());

    skyCubeMap_->setMinificationFilter(QOpenGLTexture::Linear);     /// 纹理放大或缩小时，像素的取值方法 ，线性或就近抉择
    skyCubeMap_->setMagnificationFilter(QOpenGLTexture::Linear);
    skyCubeMap_->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::ClampToEdge);   /// 设置纹理边缘的扩展方法
    skyCubeMap_->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::ClampToEdge);
    //   m_CubeMap->setWrapMode(QOpenGLTexture::DirectionR, QOpenGLTexture::ClampToEdge);

}


