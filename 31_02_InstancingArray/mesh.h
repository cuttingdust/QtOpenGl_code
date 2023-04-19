#ifndef __MESH_H__
#define __MESH_H__

#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_1_Core>
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

static int gCount = 0;

class Mesh {
public:
    Mesh(QOpenGLFunctions_4_1_Core *glFuns
            , std::vector<Vertex> vertices
            , std::vector<unsigned int> indices
            , std::vector<Texture> textures
    );

    ~Mesh();

public:
    void setPel1(GLenum pel);
    void Draw(QOpenGLShaderProgram &shader);
    void openInstancing();


    bool isBInstancing() const;
    void setBInstancing(bool bInstancing);

    int getInstancingNum() const;
    void setInstancingNum(int instancingNum);

public:
    // mesh data
    std::vector<Vertex> vertices_;
    std::vector<unsigned int> indices_;
    std::vector<Texture> textures_;

private:
    void setupMesh();

private:
        // render data
    unsigned int VAO_, VBO_, EBO_;
    QOpenGLFunctions_4_1_Core *glFuns_;
    GLenum pel_;
    bool bInstancing_;
    int instancingNum_;
};

#endif


