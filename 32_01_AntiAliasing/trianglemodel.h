#ifndef TRIANGLE_MODEL_H
#define TRIANGLE_MODEL_H

#include "abstractModel.h"

#include <QOpenGLTexture>
#include <QOpenGLVersionFunctionsFactory>

class TriangleModel : public AbstractModel{
public:
    TriangleModel(QOpenGLFunctions_4_1_Core *glfuns);
    ~TriangleModel();

public:
    void Draw(QOpenGLShaderProgram &shader) override;
private:
    void processNode(aiNode *node, const aiScene *scene) override;
    Mesh* processMesh(aiMesh *mesh, const aiScene *scene) override;

private:
    // model data
    QOpenGLFunctions_4_1_Core *glFuns_ = nullptr;
    Mesh* triangleMesh_ = nullptr;
};

#endif // TRIANGLE_MODEL_H
