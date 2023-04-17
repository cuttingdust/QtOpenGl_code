#ifndef WINDOWS_MODEL_H
#define WINDOWS_MODEL_H

#include "abstractModel.h"
#include "camera.h"

#include <QOpenGLTexture>
#include <QOpenGLVersionFunctionsFactory>

class WindowsModel: public AbstractModel{
public:
    WindowsModel(QOpenGLFunctions_4_1_Core *glfuns, Camera camera);
    ~WindowsModel();

public:
    void Draw(QOpenGLShaderProgram &shader) override;
private:
    void processNode(aiNode *node, const aiScene *scene) override;
    Mesh* processMesh(aiMesh *mesh, const aiScene *scene) override;

private:
    // model data
    QOpenGLFunctions_4_1_Core *glFuns_ = nullptr;
    std::map<float, std::pair<QVector3D, Mesh*>> meshes_;
    Camera camera_;
};

#endif
