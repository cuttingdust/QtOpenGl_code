#ifndef __MODEL_H__
#define __MODEL_H__

#include "abstractModel.h"

#include <QOpenGLTexture>

class Model : public AbstractModel{
public:
    Model(QOpenGLFunctions_4_1_Core *glfuns, const char *path);
    ~Model();

public:
    void Draw(QOpenGLShaderProgram &shader) override;
    void setPoints(bool  bPoints);
    std::vector<Mesh *> getMeshes();
private:
    void loadModel(const std::string& path);
    void processNode(aiNode *node, const aiScene *scene) override;
    Mesh* processMesh(aiMesh *mesh, const aiScene *scene) override;
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    void loadMaterialTextures(const std::string& filePath, std::string typeName);
    unsigned int TextureFromFile(const char *path, const std::string &directory);


public:
    float maxX_ = -100.0f;
    float maxY_ = -100.0f;
    float minX_ = 100.0f;
    float minY_ = 100.0f;
    std::vector<Texture> texturesLoaded_;

private:
    // model data
    QOpenGLFunctions_4_1_Core *glFuns_;
    std::string directory_;
    std::vector<Mesh*> meshes_;

};

#endif // __MODEL_H__
