#ifndef LIGHT_MODEL_H
#define LIGHT_MODEL_H

#include "abstractModel.h"

#include <QOpenGLTexture>
#include <QOpenGLVersionFunctionsFactory>

class LightModel : public AbstractModel{
public:
    LightModel(QOpenGLFunctions_4_1_Core *glfuns,  std::vector<QVector3D> pointLightPos, std::vector<QVector3D> pointLightColor);
    ~LightModel();

public:
    void Draw(QOpenGLShaderProgram &shader) override;
private:
    void processNode(aiNode *node, const aiScene *scene) override;
    Mesh* processMesh(aiMesh *mesh, const aiScene *scene) override;

private:
    // model data
    QOpenGLFunctions_4_1_Core *glFuns_;
    std::vector<Mesh*> meshes_;
    std::vector<QVector3D> pointLightPosVec_;
    std::vector<QVector3D> pointLightColorVec_;
};

#endif // __MODEL_H__
