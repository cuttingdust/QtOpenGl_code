#include "vegetationModel.h"
#include "verticesData.h"

std::vector<QVector3D> gWindowsPoints;

void VegetationModel::processNode(aiNode *node, const aiScene *scene)
{
    gWindowsPoints.push_back(QVector3D(-1.5f, 0.0f, -0.48f));
    gWindowsPoints.push_back(QVector3D(1.5f, 0.0f, 0.51f));
    gWindowsPoints.push_back(QVector3D(0.0f, 0.0f, 0.7f));
    gWindowsPoints.push_back(QVector3D(-0.3f, 0.0f, -2.3f));
    gWindowsPoints.push_back(QVector3D(0.5f, 0.0f, -0.6f));


    for (auto pos :gWindowsPoints) {
        meshes_.push_back(processMesh(nullptr, nullptr));
    }
}

Mesh* VegetationModel::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices(6);
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    int iSize = sizeof( gTransparentVertices ) / sizeof( float );
//    memcpy(&vertices[0], gPlaneVertices, sizeof(gPlaneVertices));
    vertices.clear();
    for (int i = 0; i < iSize; i += 8) {
        Vertex vertex;
        // 处理顶点位置、法线和纹理坐标
        QVector3D vector;
        vector.setX(gTransparentVertices[i + 0]);
        vector.setY(gTransparentVertices[i + 1]);
        vector.setZ(gTransparentVertices[i + 2]);
        vertex.position_ = vector;

        QVector2D vec;
        vec.setX(gTransparentVertices[i + 3]);
        vec.setY(gTransparentVertices[i + 4]);
        vertex.texCoords_ = vec;

        vector.setX(gTransparentVertices[i + 5]);
        vector.setY(gTransparentVertices[i + 6]);
        vector.setZ(gTransparentVertices[i + 7]);
        vertex.normal_ = vector;

        vertices.push_back(vertex);
    }

    qDebug() << "#######################VegetationMesh########################";
    for (auto vert: vertices) {
        qDebug() << vert.position_ << " " << vert.texCoords_ << " " << vert.normal_;
    }

    for (int i = 0; i < iSize; ++i) {
        indices.push_back(i);
    }

    auto diffuseTexture = new QOpenGLTexture(QImage(":/image/images/grass.png").mirrored());
    auto specularTexture = new QOpenGLTexture(QImage(":/image/images/grass.png").mirrored());

    Texture tex;
    tex.id_= diffuseTexture->textureId();
    tex.type_ = "texture_diffuse";
    textures.push_back(tex);
    tex.id_ = specularTexture->textureId();
    tex.type_ = "texture_specular";
    textures.push_back(tex);


    return new Mesh(glFuns_ ,vertices, indices, textures);
}


VegetationModel::VegetationModel(QOpenGLFunctions_4_1_Core *glfuns):
    glFuns_(glfuns)
{
    auto fmt = QOpenGLContext::currentContext()->format();
    qDebug() << fmt.version();

    processNode(nullptr, nullptr);
}

void VegetationModel::Draw(QOpenGLShaderProgram &shader) {

    for(unsigned int i = 0; i < meshes_.size(); i++)
    {
        QMatrix4x4 model;
        model.translate(gWindowsPoints[i]);
        shader.bind();
        shader.setUniformValue("model", model);


        for(unsigned int i = 0; i < meshes_[i]->textures_.size(); i++) {
            glFuns_->glActiveTexture(GL_TEXTURE0 + i);

            glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }


        meshes_[i]->Draw(shader);
    }


}

VegetationModel::~VegetationModel() {
    meshes_.clear();
    gWindowsPoints.clear();
}
