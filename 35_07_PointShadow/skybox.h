#ifndef SKY_BOX_H
#define SKY_BOX_H

#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_1_Core>
#include <string>
#include <vector>
#include <QOpenGLTexture>

class SkyBox {
public:
    SkyBox(QOpenGLFunctions_4_1_Core *glFuns);

    ~SkyBox();

public:
    void Draw(QOpenGLShaderProgram &shader);

private:
    void createSkyBoxTex();
    void setupSkyBox();

private:
    // render data
    unsigned int skyboxVAO_;
    unsigned int skyboxVBO_;
    QOpenGLFunctions_4_1_Core *glFuns_;
    QOpenGLTexture* skyCubeMap_;
};

#endif // SKY_BOX_H


