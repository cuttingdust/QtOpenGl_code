#include "lightmodel.h"

float cubeVertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f
};

void LightModel::processNode(aiNode *node, const aiScene *scene)
{
    for (int i = 0; i < pointLightPosVec_.size(); ++i) {
        meshes_.push_back(processMesh(nullptr, nullptr));
    }
}

Mesh* LightModel::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices(36);
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

//    int iSize = sizeof( cubeVertices ) / sizeof( float );

    memcpy(&vertices[0], cubeVertices, sizeof(cubeVertices));
//    for (int i = 0; i < iSize; i += 8) {
//        Vertex vertex;
//        // 处理顶点位置、法线和纹理坐标
//        QVector3D vector;
//        vector.setX(cubeVertices[i + 0]);
//        vector.setY(cubeVertices[i + 1]);
//        vector.setZ(cubeVertices[i + 2]);
//        vertex.position_ = vector;
//
//        QVector2D vec;
//        vec.setX(cubeVertices[i + 3]);
//        vec.setY(cubeVertices[i + 4]);
//        vertex.texCoords_ = vec;
//
//        vector.setX(cubeVertices[i + 5]);
//        vector.setY(cubeVertices[i + 6]);
//        vector.setZ(cubeVertices[i + 7]);
//        vertex.normal_ = vector;
//
//        vertices.push_back(vertex);
//    }

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


LightModel::LightModel(QOpenGLFunctions_4_1_Core *glfuns,  std::vector<QVector3D> pointLightPos, std::vector<QVector3D> pointLightColor):
    glFuns_(glfuns)
    , pointLightPosVec_(pointLightPos)
    , pointLightColorVec_(pointLightColor)
{
    auto fmt = QOpenGLContext::currentContext()->format();
    qDebug() << fmt.version();

    processNode(nullptr, nullptr);
}

void LightModel::Draw(QOpenGLShaderProgram &shader) {
    for(unsigned int i = 0; i < meshes_.size(); i++)
    {
        QMatrix4x4 model;
        model.translate(pointLightPosVec_[i]);
        meshes_[0]->Draw(shader);
        shader.bind();
        shader.setUniformValue("model", model);
        shader.setUniformValue("lightColor", pointLightColorVec_[i]);
    }
}

LightModel::~LightModel() {
    meshes_.clear();
    pointLightPosVec_.clear();
    pointLightColorVec_.clear();
}
