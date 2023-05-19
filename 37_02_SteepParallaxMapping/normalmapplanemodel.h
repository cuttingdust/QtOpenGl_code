#ifndef NORMALMAP_PLANE_MODEL_H
#define NORMALMAP_PLANE_MODEL_H

#include "abstractModel.h"

#include <QOpenGLTexture>
#include <QOpenGLVersionFunctionsFactory>

class NormalMapPlaneModel : public AbstractModel{
public:
    NormalMapPlaneModel(QOpenGLFunctions_4_1_Core *glfuns);
    ~NormalMapPlaneModel();

public:
    void Draw(QOpenGLShaderProgram &shader) override;
private:
    void processNode(aiNode *node, const aiScene *scene) override;
    Mesh* processMesh(aiMesh *mesh, const aiScene *scene) override;

private:
    // model data
    QOpenGLFunctions_4_1_Core *glFuns_ = nullptr;
    QOpenGLTexture* diffuseTexture_;
    QOpenGLTexture* normalMapTexture_;
    QOpenGLTexture* heightTexture_;

    Mesh* quadMesh = nullptr;

    void renderQuad();
};

#endif
