#ifndef DEFERRED_SHADING_MODEL_H
#define DEFERRED_SHADING_MODEL_H

#include "abstractModel.h"
#include "model.h"

#include <QOpenGLTexture>
#include <QOpenGLVersionFunctionsFactory>

class DeferredShadingModel : public AbstractModel{
public:
    DeferredShadingModel(QOpenGLFunctions_4_1_Core *glfuns, Model *model, int width, int height);
    ~DeferredShadingModel();

public:
    void Draw(QOpenGLShaderProgram &shader) override;
    void DrawLighting(QOpenGLShaderProgram &shader);
    void DrawLightingBox(QOpenGLShaderProgram &shader);
    void renderQuad();
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
    Model* model_ = nullptr;
    int width_ = 0;
    int height_ = 0;


    bool bHDR_ = false;
    float exposure_ = 20.1f;
};

#endif
