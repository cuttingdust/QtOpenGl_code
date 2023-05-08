#ifndef HOUSE_MODEL_H
#define HOUSE_MODEL_H

#include "abstractModel.h"

#include <QOpenGLTexture>
#include <QOpenGLVersionFunctionsFactory>

class HouseModel : public AbstractModel{
public:
    HouseModel(QOpenGLFunctions_4_1_Core *glfuns);
    ~HouseModel();

public:
    void Draw(QOpenGLShaderProgram &shader) override;
    void setPoints();
private:
    void processNode(aiNode *node, const aiScene *scene) override;
    Mesh* processMesh(aiMesh *mesh, const aiScene *scene) override;

private:
    // model data
    QOpenGLFunctions_4_1_Core *glFuns_ = nullptr;
    Mesh* houseMesh_ = nullptr;

};

#endif // HOUSE_MODEL_H
