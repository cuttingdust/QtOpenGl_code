#include "windowsModel.h"
#include "verticesData.h"

static std::vector<QVector3D> gWinsowsPoints;
void WindowsModel::processNode(aiNode *node, const aiScene *scene)
{
    gWinsowsPoints.push_back(QVector3D(-1.5f, 0.0f, -0.48f));
    gWinsowsPoints.push_back(QVector3D(1.5f, 0.0f, 0.51f));
    gWinsowsPoints.push_back(QVector3D(0.0f, 0.0f, 0.7f));
    gWinsowsPoints.push_back(QVector3D(-0.3f, 0.0f, -2.3f));
    gWinsowsPoints.push_back(QVector3D(0.5f, 0.0f, -0.6f));


    foreach(auto item, gWinsowsPoints) {
        float distance = camera_.getPosition().distanceToPoint(item);
        meshes_.emplace(distance, std::make_pair(item, processMesh(nullptr, nullptr)));
    }
}

Mesh* WindowsModel::processMesh(aiMesh *mesh, const aiScene *scene)
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

    auto diffuseTexture = new QOpenGLTexture(QImage(":/image/images/blending_transparent_window.png").mirrored());
    auto specularTexture = new QOpenGLTexture(QImage(":/image/images/blending_transparent_window.png").mirrored());

    Texture tex;
    tex.id_= diffuseTexture->textureId();
    tex.type_ = "texture_diffuse";
    textures.push_back(tex);
    tex.id_ = specularTexture->textureId();
    tex.type_ = "texture_specular";
    textures.push_back(tex);


    return new Mesh(glFuns_ ,vertices, indices, textures);
}


WindowsModel::WindowsModel(QOpenGLFunctions_4_1_Core *glfuns, Camera camera):
    glFuns_(glfuns)
    , camera_(camera)
{
    auto fmt = QOpenGLContext::currentContext()->format();
    qDebug() << fmt.version();

    glFuns_->glEnable(GL_BLEND);
    glFuns_->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    processNode(nullptr, nullptr);
}

void WindowsModel::Draw(QOpenGLShaderProgram &shader) {

    for(auto riter=meshes_.rbegin(); riter != meshes_.rend(); ++riter)
    {
        QMatrix4x4 model;
        model.setToIdentity();
        model.translate(riter->second.first);
        shader.bind();
        shader.setUniformValue("model", model);

        for(unsigned int i = 0; i < riter->second.second->textures_.size(); i++) {
            glFuns_->glActiveTexture(GL_TEXTURE0 + i);

            glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        riter->second.second->Draw(shader);
    }


}

WindowsModel::~WindowsModel() {
    meshes_.clear();
    gWinsowsPoints.clear();
}
