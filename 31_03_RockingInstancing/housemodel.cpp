#include "housemodel.h"
#include "verticesData.h"


void HouseModel::processNode(aiNode *node, const aiScene *scene)
{
    houseMesh_ = processMesh(nullptr, nullptr);
}

Mesh* HouseModel::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    int iSize = sizeof( gHousePoints ) / sizeof( float );

    vertices.clear();
    for (int i = 0; i < iSize; i += 5) {
        Vertex vertex;
        // 处理顶点位置、法线和纹理坐标
        QVector3D vector;
        vector.setX(gHousePoints[i + 0]);
        vector.setY(gHousePoints[i + 1]);
        vector.setZ(0);
        vertex.position_ = vector;

        QVector2D vec;
        vec.setX(0);
        vec.setY(0);
        vertex.texCoords_ = vec;

        vector.setX(gHousePoints[i + 2]);
        vector.setY(gHousePoints[i + 3]);
        vector.setZ(gHousePoints[i + 4]);
        vertex.normal_ = vector;

        vertices.push_back(vertex);
    }

    qDebug() << "#######################HouseMesh########################";
    for (auto vert: vertices) {
        qDebug() << vert.position_ << " " << vert.texCoords_ << " " << vert.normal_;
    }

    for (int i = 0; i < iSize; ++i) {
        indices.push_back(i);
    }

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


HouseModel::HouseModel(QOpenGLFunctions_4_1_Core *glfuns):
    glFuns_(glfuns)
{
    auto fmt = QOpenGLContext::currentContext()->format();
    qDebug() << fmt.version();


    processNode(nullptr, nullptr);
    setPoints();
}

void HouseModel::Draw(QOpenGLShaderProgram &shader) {
    QMatrix4x4 model;
    shader.bind();
    shader.setUniformValue("model", model);
    houseMesh_->Draw(shader);
}

void HouseModel::setPoints() {
    houseMesh_->setPel1(GL_POINTS);
}

HouseModel::~HouseModel() {
    delete houseMesh_;
}
