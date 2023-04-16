#ifndef ABSTRACT_MODEL_H
#define ABSTRACT_MODEL_H

#include <QOpenGLShaderProgram>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"

class AbstractModel
{
public:
    virtual void Draw(QOpenGLShaderProgram &shader) = 0;
public:
    virtual void processNode(aiNode *node, const aiScene *scene) = 0;
    virtual Mesh* processMesh(aiMesh *mesh, const aiScene *scene) = 0;
};

#endif