#ifndef DEPTHMAP_MODEL_H
#define DEPTHMAP_MODEL_H

#include "abstractModel.h"

#include <QOpenGLTexture>
#include <QOpenGLVersionFunctionsFactory>

class DepthMapModel : public AbstractModel{
public:
    DepthMapModel(QOpenGLFunctions_4_1_Core *glfuns, int width, int height);
    ~DepthMapModel();

public:
    void bindFramer(QOpenGLShaderProgram &shader, QVector3D lightPos);
    void Draw(QOpenGLShaderProgram &shader) override;
private:
    void processNode(aiNode *node, const aiScene *scene) override;
    Mesh* processMesh(aiMesh *mesh, const aiScene *scene) override;

private:
    // model data
    QOpenGLFunctions_4_1_Core *glFuns_ = nullptr;
    Mesh* quadMesh_ = nullptr;
    int width_ = 0;
    int height_ = 0;
};

#endif
