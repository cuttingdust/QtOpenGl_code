#ifndef __MESH_H__
#define __MESH_H__

#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <string>
#include <vector>

struct Vertex {
    QVector3D position_;
    QVector2D texCoords_;
    QVector3D normal_;
};

struct Texture {
    unsigned int id_;
    std::string type_;
    std::string path_;
};

class Mesh {
public:
    Mesh(QOpenGLFunctions *glFuns
            , std::vector<Vertex> vertices
            , std::vector<unsigned int> indices
            , std::vector<Texture> textures
    );

public:
    void Draw(QOpenGLShaderProgram &shader);

public:
    // mesh data
    std::vector<Vertex> vertices_;
    std::vector<unsigned int> indices_;
    std::vector<Texture> textures_;

private:
    // render data
    unsigned int VAO, VBO, EBO;
    void setupMesh();
private:
    QOpenGLFunctions *glFuns_;
};

#endif


