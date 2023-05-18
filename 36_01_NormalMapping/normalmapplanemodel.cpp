#include "normalmapplanemodel.h"
#include "verticesData.h"


void NormalMapPlaneModel::processNode(aiNode *node, const aiScene *scene)
{
    quadMesh = processMesh(nullptr, nullptr);
}

Mesh* NormalMapPlaneModel::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    int iSize = sizeof( gNormalQuadVertices ) / sizeof( float );

//    memcpy(&vertices[0], gPlaneVertices, sizeof(gPlaneVertices));
    vertices.clear();
    for (int i = 0; i < iSize; i += 8) {
        Vertex vertex;
        // 处理顶点位置、法线和纹理坐标
        QVector3D vector;
        vector.setX(gNormalQuadVertices[i + 0]);
        vector.setY(gNormalQuadVertices[i + 1]);
        vector.setZ(gNormalQuadVertices[i + 2]);
        vertex.position_ = vector;

        QVector2D vec;
        vec.setX(gNormalQuadVertices[i + 3]);
        vec.setY(gNormalQuadVertices[i + 4]);
        vertex.texCoords_ = vec;

        vector.setX(gNormalQuadVertices[i + 5]);
        vector.setY(gNormalQuadVertices[i + 6]);
        vector.setZ(gNormalQuadVertices[i + 7]);
        vertex.normal_ = vector;

        vertices.push_back(vertex);
    }

    qDebug() << "#######################normalMapPlaneMesh########################";
    for (auto vert: vertices) {
        qDebug() << vert.position_ << " " << vert.texCoords_ << " " << vert.normal_;
    }

    for (int i = 0; i < iSize; ++i) {
        indices.push_back(i);
    }

    auto diffuseTexture = new QOpenGLTexture(QImage(":/image/images/brickwall.jpg").mirrored());
    auto normalMapTexture = new QOpenGLTexture(QImage(":/image/images/brickwall_normal.jpg").mirrored());

    Texture tex;
    tex.id_= diffuseTexture->textureId();
    tex.type_ = "texture_diffuse";
    textures.push_back(tex);
    tex.id_= normalMapTexture->textureId();
    tex.type_ = "texture_normal1";
    textures.push_back(tex);

    return new Mesh(glFuns_ ,vertices, indices, textures);
}


NormalMapPlaneModel::NormalMapPlaneModel(QOpenGLFunctions_4_1_Core *glfuns):
    glFuns_(glfuns)
{
    auto fmt = QOpenGLContext::currentContext()->format();
    qDebug() << fmt.version();

    processNode(nullptr, nullptr);
}

void NormalMapPlaneModel::Draw(QOpenGLShaderProgram &shader) {
    QMatrix4x4 model;
    model.setToIdentity();
    shader.bind();
    shader.setUniformValue("model", model);
//    shader.setUniformValue("skybox", 15);
    quadMesh->Draw(shader);
}

NormalMapPlaneModel::~NormalMapPlaneModel() {
    delete quadMesh;
}
