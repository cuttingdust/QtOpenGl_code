#include "rockmodel.h"
#include "verticesData.h"
#include <QtGlobal>

#include <QTime>
#include <QTimer>
#include <QRandomGenerator>

constexpr unsigned int amount = 10000;
QMatrix4x4 *modelMatrices;

void RockModel::processNode(aiNode *node, const aiScene *scene)
{
    modelMatrices = new QMatrix4x4[amount];
    auto* qvector4d = new QVector4D[4 * amount];
    QRandomGenerator random(QTime(0,0,0).secsTo(QTime::currentTime()));
    float radius = 25.0;
    float offset = 7.5f;
    for(unsigned int i = 0; i < amount; i++) {
        QMatrix4x4 model;
        // 1. translation: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (random.bounded(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (random.bounded(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
        displacement = (random.bounded(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model.translate(QVector3D(x, y, z));

        // 2. scale: scale between 0.05 and 0.25f
        float scale = (random.bounded(20)) / 100.0f + 0.05;
        model.scale(scale);

        // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        float rotAngle = (random.bounded(360));
        model.rotate(rotAngle, QVector3D(0.4f, 0.6f, 0.8f));

        // 4. now add to list of matrices
        modelMatrices[i] = model;
        qvector4d[i*4+0] = model.column(0);
        qvector4d[i*4+1] = model.column(1);
        qvector4d[i*4+2] = model.column(2);
        qvector4d[i*4+3] = model.column(3);
    }

    rockModel_ = new Model(glFuns_ , "../../../../../rock/rock.obj");

    // vertex buffer object
    unsigned int buffer;
    glFuns_->glGenBuffers(1, &buffer);
    glFuns_->glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glFuns_->glBufferData(GL_ARRAY_BUFFER, amount * 64, &qvector4d[0], GL_STATIC_DRAW);

    for(unsigned int i = 0; i < rockModel_->getMeshes().size(); i++)
    {
        glFuns_->glBindVertexArray(rockModel_->getMeshes()[i]->getVao()); // vertex attributes
        rockModel_->getMeshes()[i]->setBInstancing(true);
        rockModel_->getMeshes()[i]->setInstancingNum(amount);
        std::size_t vec4Size = sizeof(QVector4D);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
        glEnableVertexAttribArray(3);

        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
        glEnableVertexAttribArray(4);

        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
        glEnableVertexAttribArray(5);

        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));
        glEnableVertexAttribArray(6);

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
        glBindVertexArray(0);
    }


        planetModel_ = new Model(glFuns_ , "../../../../../planet/planet.obj");
}

Mesh* RockModel::processMesh(aiMesh *mesh, const aiScene *scene)
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


RockModel::RockModel(QOpenGLFunctions_4_1_Core *glfuns):
    glFuns_(glfuns)
{
    auto fmt = QOpenGLContext::currentContext()->format();
    qDebug() << fmt.version();

    processNode(nullptr, nullptr);
}

void RockModel::Draw(QOpenGLShaderProgram &shader) {
    QMatrix4x4 model;
    shader.bind();
    shader.setUniformValue("bRockModel", true);
    shader.setUniformValue("model", model);
    rockModel_->Draw(shader);
    shader.setUniformValue("bRockModel", false);
    planetModel_->Draw(shader);
    shader.release();

}

RockModel::~RockModel() {
    delete rockModel_;
}
