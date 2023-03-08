#ifndef __MODEL_H__
#define __MODEL_H__

#include "mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <QOpenGLTexture>

class Model {
public:
    Model(QOpenGLFunctions *glfuns, const char *path);
public:
    void Draw(QOpenGLShaderProgram &shader);
private:
    void loadModel(const std::string& path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    unsigned int TextureFromFile(const char *path, const std::string &directory);

public:
    float maxX_ = -100.0f;
    float maxY_ = -100.0f;
    float minX_ = 100.0f;
    float minY_ = 100.0f;
    std::vector<Texture> texturesLoaded_;

private:
    // model data
    QOpenGLFunctions *glFuns_;
    std::string directory_;
    std::vector<Mesh> meshes_;

};

#endif // __MODEL_H__
