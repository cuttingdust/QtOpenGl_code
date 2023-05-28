#ifndef BLOOM_CUBE_MODEL_H
#define BLOOM_CUBE_MODEL_H

#include "abstractModel.h"

#include <QOpenGLTexture>
#include <QOpenGLVersionFunctionsFactory>

class BloomCubeModel : public AbstractModel{
public:
    BloomCubeModel(QOpenGLFunctions_4_1_Core *glfuns, int width, int height);
    ~BloomCubeModel();

public:
    void Draw(QOpenGLShaderProgram &shader) override;
    void DrawLight(QOpenGLShaderProgram &shader);
    void DrawBlur(QOpenGLShaderProgram &shader);
    void DrawBloom(QOpenGLShaderProgram &shader);    void resizeGL(int w, int h);
    void renderQuad();
    void bindFrambuffer();
    void realeaseFrambuffer();

    bool isBBloom() const;
    void setBBloom(bool bBloom);
    float getExposure() const;
    void setExposure(float exposure);
private:
    void processNode(aiNode *node, const aiScene *scene) override;
    Mesh* processMesh(aiMesh *mesh, const aiScene *scene) override;

private:
    // model data
    QOpenGLFunctions_4_1_Core *glFuns_ = nullptr;
    Mesh* cubeMesh_ = nullptr;
    int width_ = 0;
    int height_ = 0;


    bool bBloom_ = false;
    float exposure_ = 1.0f;

};

#endif
