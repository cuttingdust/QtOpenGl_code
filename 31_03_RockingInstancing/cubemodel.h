#ifndef CUBE_MODEL_H
#define CUBE_MODEL_H

#include "abstractModel.h"

#include <QOpenGLTexture>
#include <QOpenGLVersionFunctionsFactory>

class CubeModel : public AbstractModel{
public:
    CubeModel(QOpenGLFunctions_4_1_Core *glfuns);
    ~CubeModel();

public:
    void Draw(QOpenGLShaderProgram &shader) override;
private:
    void processNode(aiNode *node, const aiScene *scene) override;
    Mesh* processMesh(aiMesh *mesh, const aiScene *scene) override;

private:
    // model data
    QOpenGLFunctions_4_1_Core *glFuns_ = nullptr;
    Mesh* cubeMesh_ = nullptr;
};

#endif
