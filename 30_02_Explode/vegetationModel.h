#ifndef VEGETATION_MODEL_H
#define VEGETATION_MODEL_H

#include "abstractModel.h"

#include <QOpenGLTexture>
#include <QOpenGLVersionFunctionsFactory>

class VegetationModel: public AbstractModel{
public:
    VegetationModel(QOpenGLFunctions_4_1_Core *glfuns);
    ~VegetationModel();

public:
    void Draw(QOpenGLShaderProgram &shader) override;
private:
    void processNode(aiNode *node, const aiScene *scene) override;
    Mesh* processMesh(aiMesh *mesh, const aiScene *scene) override;

private:
    // model data
    QOpenGLFunctions_4_1_Core *glFuns_ = nullptr;
    std::vector<Mesh*> meshes_;
};

#endif
