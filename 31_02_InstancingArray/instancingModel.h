#ifndef INSTANCING_MODEL_H
#define INSTANCING_MODEL_H

#include "abstractModel.h"

#include <QOpenGLTexture>
#include <QOpenGLVersionFunctionsFactory>

class InstancingModel : public AbstractModel{
public:
    InstancingModel(QOpenGLFunctions_4_1_Core *glfuns);
    ~InstancingModel();

public:
    void Draw(QOpenGLShaderProgram &shader) override;
private:
    void processNode(aiNode *node, const aiScene *scene) override;
    Mesh* processMesh(aiMesh *mesh, const aiScene *scene) override;

private:
    // model data
    QOpenGLFunctions_4_1_Core *glFuns_ = nullptr;
    Mesh* instancingMesh_ = nullptr;
};

#endif //INSTANCING_MODEL_H
