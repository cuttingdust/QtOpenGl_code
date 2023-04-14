#ifndef PLANE_MODEL_H
#define PLANE_MODEL_H

#include "abstractModel.h"

#include <QOpenGLTexture>
#include <QOpenGLVersionFunctionsFactory>

class PlaneModel : public AbstractModel{
public:
    PlaneModel(QOpenGLFunctions_4_1_Core *glfuns);
    ~PlaneModel();

public:
    void Draw(QOpenGLShaderProgram &shader) override;
private:
    void processNode(aiNode *node, const aiScene *scene) override;
    Mesh* processMesh(aiMesh *mesh, const aiScene *scene) override;

private:
    // model data
    QOpenGLFunctions_4_1_Core *glFuns_ = nullptr;
    Mesh* planeMesh_ = nullptr;
};

#endif
