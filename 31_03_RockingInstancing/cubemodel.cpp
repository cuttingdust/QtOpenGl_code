#include "cubemodel.h"
#include "verticesData.h"


void CubeModel::processNode(aiNode *node, const aiScene *scene)
{
    cubeMesh_ = processMesh(nullptr, nullptr);
}

Mesh* CubeModel::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices(36);
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    int iSize = sizeof( gCubeVertices ) / sizeof( float );

//    memcpy(&vertices[0], gCubeVertices, sizeof(gCubeVertices));
    vertices.clear();
    for (int i = 0; i < iSize; i += 8) {
        Vertex vertex;
        // 处理顶点位置、法线和纹理坐标
        QVector3D vector;
        vector.setX(gCubeVertices[i + 0]);
        vector.setY(gCubeVertices[i + 1]);
        vector.setZ(gCubeVertices[i + 2]);
        vertex.position_ = vector;

        QVector2D vec;
        vec.setX(gCubeVertices[i + 3]);
        vec.setY(gCubeVertices[i + 4]);
        vertex.texCoords_ = vec;

        vector.setX(gCubeVertices[i + 5]);
        vector.setY(gCubeVertices[i + 6]);
        vector.setZ(gCubeVertices[i + 7]);
        vertex.normal_ = vector;

        vertices.push_back(vertex);
    }

    for (auto vert: vertices) {
        qDebug() << vert.position_ << " " << vert.texCoords_ << " " << vert.normal_;
    }

    for (int i = 0; i < 36; ++i) {
        indices.push_back(i);
    }

    auto diffuseTexture = new QOpenGLTexture(QImage(":/image/texMap/container2.png").mirrored());
    auto specularTexture = new QOpenGLTexture(QImage(":/image/texMap/lighting_maps_specular_color.png").mirrored());

    Texture tex;
    tex.id_= diffuseTexture->textureId();
    tex.type_ = "texture_diffuse";
    textures.push_back(tex);
    tex.id_ = specularTexture->textureId();
    tex.type_ = "texture_specular";
    textures.push_back(tex);

    return new Mesh(glFuns_ ,vertices, indices, textures);
}


CubeModel::CubeModel(QOpenGLFunctions_4_1_Core *glfuns):
    glFuns_(glfuns)
{
    auto fmt = QOpenGLContext::currentContext()->format();
    qDebug() << fmt.version();

    processNode(nullptr, nullptr);
}

void CubeModel::Draw(QOpenGLShaderProgram &shader) {
    QMatrix4x4 model;
    shader.bind();
    shader.setUniformValue("model", model);
    shader.setUniformValue("skybox", 15);
    cubeMesh_->Draw(shader);
}

CubeModel::~CubeModel() {
    delete cubeMesh_;
}
