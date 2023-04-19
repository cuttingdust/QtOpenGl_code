#include "instancingModel.h"
#include "verticesData.h"


void InstancingModel::processNode(aiNode *node, const aiScene *scene)
{
    instancingMesh_ = processMesh(nullptr, nullptr);
    instancingMesh_->setBInstancing(true);
    instancingMesh_->setInstancingNum(100);
}

Mesh* InstancingModel::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    int iSize = sizeof( gQuadVertices ) / sizeof( float );

//    memcpy(&vertices[0], gPlaneVertices, sizeof(gPlaneVertices));
    vertices.clear();
    for (int i = 0; i < iSize; i += 5) {
        Vertex vertex;
        // 处理顶点位置、法线和纹理坐标
        QVector3D vector;
        vector.setX(gQuadVertices[i + 0]);
        vector.setY(gQuadVertices[i + 1]);
        vector.setZ(0);
        vertex.position_ = vector;

        QVector2D vec;
        vec.setX(0);
        vec.setY(0);
        vertex.texCoords_ = vec;

        vector.setX(gQuadVertices[i + 2]);
        vector.setY(gQuadVertices[i + 3]);
        vector.setZ(gQuadVertices[i + 4]);
        vertex.normal_ = vector;

        vertices.push_back(vertex);
    }

    qDebug() << "#######################planeMesh########################";
    for (auto vert: vertices) {
        qDebug() << vert.position_ << " " << vert.texCoords_ << " " << vert.normal_;
    }

    for (int i = 0; i < iSize; ++i) {
        indices.push_back(i);
    }

//    auto diffuseTexture = new QOpenGLTexture(QImage(":/image/texMap/container2.png").mirrored());
    auto diffuseTexture = new QOpenGLTexture(QImage(":/image/images/wall.jpg").mirrored());
    auto specularTexture = new QOpenGLTexture(QImage(":/image/images/wall.jpg").mirrored());

    Texture tex;
    tex.id_= diffuseTexture->textureId();
    tex.type_ = "texture_diffuse";
    textures.push_back(tex);
    tex.id_ = specularTexture->textureId();
    tex.type_ = "texture_specular";
    textures.push_back(tex);

    return new Mesh(glFuns_ ,vertices, indices, textures);
}


InstancingModel::InstancingModel(QOpenGLFunctions_4_1_Core *glfuns):
    glFuns_(glfuns)
{
    auto fmt = QOpenGLContext::currentContext()->format();
    qDebug() << fmt.version();

    processNode(nullptr, nullptr);
}

void InstancingModel::Draw(QOpenGLShaderProgram &shader) {
//    QMatrix4x4 model;
//    shader.bind();
//    shader.setUniformValue("model", model);
    instancingMesh_->Draw(shader);
}

InstancingModel::~InstancingModel() {
    delete instancingMesh_;
}
