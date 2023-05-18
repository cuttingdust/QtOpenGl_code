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

//    auto diffuseTexture = new QOpenGLTexture(QImage(":/image/texMap/container2.png").mirrored());
//    auto specularTexture = new QOpenGLTexture(QImage(":/image/texMap/lighting_maps_specular_color.png").mirrored());

    auto diffuseTexture = new QOpenGLTexture(QImage(":/image/images/wood.png").mirrored());
    auto specularTexture = new QOpenGLTexture(QImage(":/image/images/wood.png").mirrored());

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

    model.setToIdentity();
    shader.setUniformValue("model", model);
    // room cube
    model.scale(5.0f);
    shader.setUniformValue("model", model);
    /// note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
    glFuns_->glDisable(GL_CULL_FACE);
    shader.setUniformValue("reverse_normals", 1); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
    cubeMesh_->Draw(shader);
    shader.setUniformValue("reverse_normals", 0); // and of course disable it
    glFuns_->glEnable(GL_CULL_FACE);

    // 三个箱子
    model.setToIdentity();
    model.translate(QVector3D(4.0f, -3.5f, 0.0));
    model.scale(0.5);
    shader.setUniformValue("model", model);
    cubeMesh_->Draw(shader);

    model.setToIdentity();
    model.translate(QVector3D(2.0f, 3.0f, 1.0));
    model.scale(0.75);
    shader.setUniformValue("model", model);
    cubeMesh_->Draw(shader);

    model.setToIdentity();
    model.translate(QVector3D(-3.0f, -1.0f, 0.0));
    model.scale(0.5);
    shader.setUniformValue("model", model);
    cubeMesh_->Draw(shader);

    model.setToIdentity();
    model.translate(QVector3D(-1.5f, 1.0f, 1.5));
    model.scale(0.5);
    //model.rotate(60,QVector3D(1.0, 0.0, 1.0));
    shader.setUniformValue("model", model);
    cubeMesh_->Draw(shader);

    model.setToIdentity();
    model.translate(QVector3D(-1.5f, 2.0f, -3.0));
    model.scale(0.75);
    model.rotate(60,QVector3D(1.0, 0.0, 1.0));
    shader.setUniformValue("model", model);
    cubeMesh_->Draw(shader);
}

CubeModel::~CubeModel() {
    delete cubeMesh_;
}
