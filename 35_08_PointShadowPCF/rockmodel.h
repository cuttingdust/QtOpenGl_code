#ifndef ROCK_MODEL_H
#define ROCK_MODEL_H

#include "abstractModel.h"
#include "model.h"

#include <QOpenGLTexture>
#include <QOpenGLVersionFunctionsFactory>

class RockModel : public AbstractModel{
public:
    RockModel(QOpenGLFunctions_4_1_Core *glfuns);
    ~RockModel();

public:
    void Draw(QOpenGLShaderProgram &shader) override;
private:
    void processNode(aiNode *node, const aiScene *scene) override;
    Mesh* processMesh(aiMesh *mesh, const aiScene *scene) override;

private:
    // model data
    QOpenGLFunctions_4_1_Core *glFuns_ = nullptr;
    Model* planetModel_ = nullptr;
    Model* rockModel_ = nullptr;
};

#endif
