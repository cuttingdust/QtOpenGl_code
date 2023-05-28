#ifndef SSAO_MODEL_H
#define SSAO_MODEL_H

#include "abstractModel.h"
#include "model.h"

#include <QOpenGLTexture>
#include <QOpenGLVersionFunctionsFactory>

class SSAOModel : public AbstractModel{
public:
    SSAOModel(QOpenGLFunctions_4_1_Core *glfuns, int width, int height, Model *model);
    ~SSAOModel();

public:
    void Draw(QOpenGLShaderProgram &shader) override;
    void bindFramebuffer();
    void releaseFramebuffer();
    void DrawSSAO(QOpenGLShaderProgram &shader, QMatrix4x4 projection);
    void DrawBlur(QOpenGLShaderProgram &shader);
    void DrawLighting(QOpenGLShaderProgram &shader, QMatrix4x4 view);
private:
    void processNode(aiNode *node, const aiScene *scene) override;
    Mesh* processMesh(aiMesh *mesh, const aiScene *scene) override;
    void  renderQuad();

private:
    // model data
    QOpenGLFunctions_4_1_Core *glFuns_ = nullptr;
    Mesh* cubeMesh_ = nullptr;
    Model* model_ = nullptr;

    int width_ = 0;
    int height_ = 0;
};

#endif
