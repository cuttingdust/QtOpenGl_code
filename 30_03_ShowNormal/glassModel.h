#ifndef GLASS_MODEL_H
#define GLASS_MODEL_H

#include "abstractModel.h"
#include "camera.h"

#include <QOpenGLTexture>
#include <QOpenGLVersionFunctionsFactory>

class GlassModel: public AbstractModel{
public:
    GlassModel(QOpenGLFunctions_4_1_Core *glfuns, int width, int height);
    ~GlassModel();

public:
    void bindFramer();
    void Draw(QOpenGLShaderProgram &shader) override;
private:
    void processNode(aiNode *node, const aiScene *scene) override;
    Mesh* processMesh(aiMesh *mesh, const aiScene *scene) override;

private:
    // model data
    QOpenGLFunctions_4_1_Core *glFuns_ = nullptr;
    Mesh* mesh_;
    int width_;
    int height_;
};

#endif
