#ifndef HDR_CUBE_MODEL_H
#define HDR_CUBE_MODEL_H

#include "abstractModel.h"

#include <QOpenGLTexture>
#include <QOpenGLVersionFunctionsFactory>

class HDRCubeModel : public AbstractModel{
public:
    HDRCubeModel(QOpenGLFunctions_4_1_Core *glfuns, int width, int height);
    ~HDRCubeModel();

public:
    void Draw(QOpenGLShaderProgram &shader) override;
    void renderQuad(QOpenGLShaderProgram &shader);
    void bindFrambuffer();
    void realeaseFrambuffer();
private:
    void processNode(aiNode *node, const aiScene *scene) override;
    Mesh* processMesh(aiMesh *mesh, const aiScene *scene) override;

public:
    bool isBhdr() const;
    void setBhdr(bool bHdr);
    int getExposure() const;
    void setExposure(int exposure);

private:
    // model data
    QOpenGLFunctions_4_1_Core *glFuns_ = nullptr;
    Mesh* cubeMesh_ = nullptr;
    int width_ = 0;
    int height_ = 0;


    bool bHDR = true;
    float exposure = 10.1f;
};

#endif
