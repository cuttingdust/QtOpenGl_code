#include "planemodel.h"
#include "verticesData.h"


void PlaneModel::processNode(aiNode *node, const aiScene *scene)
{
    planeMesh_ = processMesh(nullptr, nullptr);
}

Mesh* PlaneModel::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices(6);
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    int iSize = sizeof( gPlaneVertices ) / sizeof( float );

//    memcpy(&vertices[0], gPlaneVertices, sizeof(gPlaneVertices));
    vertices.clear();
    for (int i = 0; i < iSize; i += 8) {
        Vertex vertex;
        // 处理顶点位置、法线和纹理坐标
        QVector3D vector;
        vector.setX(gPlaneVertices[i + 0]);
        vector.setY(gPlaneVertices[i + 1]);
        vector.setZ(gPlaneVertices[i + 2]);
        vertex.position_ = vector;

        QVector2D vec;
        vec.setX(gPlaneVertices[i + 3]);
        vec.setY(gPlaneVertices[i + 4]);
        vertex.texCoords_ = vec;

        vector.setX(gPlaneVertices[i + 5]);
        vector.setY(gPlaneVertices[i + 6]);
        vector.setZ(gPlaneVertices[i + 7]);
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
//    auto diffuseTexture = new QOpenGLTexture(QImage(":/image/images/wall.jpg").mirrored());
    QImage wall = QImage(":/image/images/wall.jpg").convertToFormat(QImage::Format_RGB888);
    auto diffuseTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    glFuns_->glBindTexture(GL_TEXTURE_2D, diffuseTexture->textureId());
    glFuns_->glTexImage2D(GL_TEXTURE_2D,0, GL_SRGB, wall.width(), wall.height(),0, GL_RGB, GL_UNSIGNED_BYTE, wall.bits());
    glFuns_->glGenerateMipmap(GL_TEXTURE_2D);


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


PlaneModel::PlaneModel(QOpenGLFunctions_4_1_Core *glfuns):
    glFuns_(glfuns)
{
    auto fmt = QOpenGLContext::currentContext()->format();
    qDebug() << fmt.version();

    processNode(nullptr, nullptr);
}

void PlaneModel::Draw(QOpenGLShaderProgram &shader) {
    QMatrix4x4 model;
    shader.bind();
    shader.setUniformValue("model", model);
    shader.setUniformValue("skybox", 15);
    planeMesh_->Draw(shader);
}

PlaneModel::~PlaneModel() {
    delete planeMesh_;
}
