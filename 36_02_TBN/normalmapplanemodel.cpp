#include "normalmapplanemodel.h"
#include "verticesData.h"
// ------------------------------------------------------------------
static unsigned int quadVAO = 0;
static unsigned int quadVBO;

void NormalMapPlaneModel::processNode(aiNode *node, const aiScene *scene)
{
    quadMesh = processMesh(nullptr, nullptr);
}

Mesh* NormalMapPlaneModel::processMesh(aiMesh *mesh, const aiScene *scene)
{
//    std::vector<Vertex> vertices;
//    std::vector<unsigned int> indices;
//    std::vector<Texture> textures;
//
//    int iSize = sizeof( gNormalQuadVertices ) / sizeof( float );
//
////    memcpy(&vertices[0], gPlaneVertices, sizeof(gPlaneVertices));
//    vertices.clear();
//    for (int i = 0; i < iSize; i += 8) {
//        Vertex vertex;
//        // 处理顶点位置、法线和纹理坐标
//        QVector3D vector;
//        vector.setX(gNormalQuadVertices[i + 0]);
//        vector.setY(gNormalQuadVertices[i + 1]);
//        vector.setZ(gNormalQuadVertices[i + 2]);
//        vertex.position_ = vector;
//
//        QVector2D vec;
//        vec.setX(gNormalQuadVertices[i + 3]);
//        vec.setY(gNormalQuadVertices[i + 4]);
//        vertex.texCoords_ = vec;
//
//        vector.setX(gNormalQuadVertices[i + 5]);
//        vector.setY(gNormalQuadVertices[i + 6]);
//        vector.setZ(gNormalQuadVertices[i + 7]);
//        vertex.normal_ = vector;
//
//        vertices.push_back(vertex);
//    }
//
//    qDebug() << "#######################normalMapPlaneMesh########################";
//    for (auto vert: vertices) {
//        qDebug() << vert.position_ << " " << vert.texCoords_ << " " << vert.normal_;
//    }
//
//    for (int i = 0; i < iSize; ++i) {
//        indices.push_back(i);
//    }

    diffuseTexture_ = new QOpenGLTexture(QImage(":/image/images/brickwall.jpg").mirrored());
    normalMapTexture_ = new QOpenGLTexture(QImage(":/image/images/brickwall_normal.jpg").mirrored());

//    Texture tex;
//    tex.id_= diffuseTexture_->textureId();
//    tex.type_ = "texture_diffuse";
//    textures.push_back(tex);
//    tex.id_= normalMapTexture_->textureId();
//    tex.type_ = "texture_normal1";
//    textures.push_back(tex);

//    return new Mesh(glFuns_ ,vertices, indices, textures);

    return nullptr;
}

// renders a 1x1 quad in NDC with manually calculated tangent vectors
void NormalMapPlaneModel::renderQuad()
{
    if (quadVAO == 0)
    {
        // positions
        QVector3D pos1(-1.0f,  1.0f, 0.0f);
        QVector3D pos2(-1.0f, -1.0f, 0.0f);
        QVector3D pos3( 1.0f, -1.0f, 0.0f);
        QVector3D pos4( 1.0f,  1.0f, 0.0f);
        // texture coordinates
        QVector2D uv1(0.0f, 1.0f);
        QVector2D uv2(0.0f, 0.0f);
        QVector2D uv3(1.0f, 0.0f);
        QVector2D uv4(1.0f, 1.0f);
        // normal vector
        QVector3D nm(0.0f, 0.0f, 1.0f);

        // calculate tangent/bitangent vectors of both triangles
        QVector3D tangent1, bitangent1;
        QVector3D tangent2, bitangent2;
        // triangle 1
        // ----------
        QVector3D edge1 = pos2 - pos1;
        QVector3D edge2 = pos3 - pos1;
        QVector2D deltaUV1 = uv2 - uv1;
        QVector2D deltaUV2 = uv3 - uv1;

        float f = 1.0f / (deltaUV1.x() * deltaUV2.y() - deltaUV2.x() * deltaUV1.y());

        tangent1.setX(f * (deltaUV2.y() * edge1.x() - deltaUV1.y() * edge2.x()));
        tangent1.setY(f * (deltaUV2.y() * edge1.y() - deltaUV1.y() * edge2.y()));
        tangent1.setZ(f * (deltaUV2.y() * edge1.z() - deltaUV1.y() * edge2.z()));

        bitangent1.setX(f * (-deltaUV2.x() * edge1.x() + deltaUV1.x() * edge2.x()));
        bitangent1.setY(f * (-deltaUV2.x() * edge1.y() + deltaUV1.x() * edge2.y()));
        bitangent1.setZ(f * (-deltaUV2.x() * edge1.z() + deltaUV1.x() * edge2.z()));

        // triangle 2
        // ----------
        edge1 = pos3 - pos1;
        edge2 = pos4 - pos1;
        deltaUV1 = uv3 - uv1;
        deltaUV2 = uv4 - uv1;

        f = 1.0f / (deltaUV1.x() * deltaUV2.y() - deltaUV2.x() * deltaUV1.y());

        tangent2.setX(f * (deltaUV2.y() * edge1.x() - deltaUV1.y() * edge2.x()));
        tangent2.setY(f * (deltaUV2.y() * edge1.y() - deltaUV1.y() * edge2.y()));
        tangent2.setZ(f * (deltaUV2.y() * edge1.z() - deltaUV1.y() * edge2.z()));


        bitangent2.setX(f * (-deltaUV2.x() * edge1.x() + deltaUV1.x() * edge2.x()));
        bitangent2.setY(f * (-deltaUV2.x() * edge1.y() + deltaUV1.x() * edge2.y()));
        bitangent2.setZ(f * (-deltaUV2.x() * edge1.z() + deltaUV1.x() * edge2.z()));


        float quadVertices[] = {
            // positions            // normal         // texcoords  // tangent                          // bitangent
            pos1.x(), pos1.y(), pos1.z(), nm.x(), nm.y(), nm.z(), uv1.x(), uv1.y(), tangent1.x(), tangent1.y(), tangent1.z(), bitangent1.x(), bitangent1.y(), bitangent1.z(),
            pos2.x(), pos2.y(), pos2.z(), nm.x(), nm.y(), nm.z(), uv2.x(), uv2.y(), tangent1.x(), tangent1.y(), tangent1.z(), bitangent1.x(), bitangent1.y(), bitangent1.z(),
            pos3.x(), pos3.y(), pos3.z(), nm.x(), nm.y(), nm.z(), uv3.x(), uv3.y(), tangent1.x(), tangent1.y(), tangent1.z(), bitangent1.x(), bitangent1.y(), bitangent1.z(),

            pos1.x(), pos1.y(), pos1.z(), nm.x(), nm.y(), nm.z(), uv1.x(), uv1.y(), tangent2.x(), tangent2.y(), tangent2.z(), bitangent2.x(), bitangent2.y(), bitangent2.z(),
            pos3.x(), pos3.y(), pos3.z(), nm.x(), nm.y(), nm.z(), uv3.x(), uv3.y(), tangent2.x(), tangent2.y(), tangent2.z(), bitangent2.x(), bitangent2.y(), bitangent2.z(),
            pos4.x(), pos4.y(), pos4.z(), nm.x(), nm.y(), nm.z(), uv4.x(), uv4.y(), tangent2.x(), tangent2.y(), tangent2.z(), bitangent2.x(), bitangent2.y(), bitangent2.z()
        };
        // configure plane VAO
        glFuns_->glGenVertexArrays(1, &quadVAO);
        glFuns_->glGenBuffers(1, &quadVBO);
        glFuns_->glBindVertexArray(quadVAO);
        glFuns_->glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glFuns_->glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glFuns_->glEnableVertexAttribArray(0);
        glFuns_->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
        glFuns_->glEnableVertexAttribArray(1);
        glFuns_->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
        glFuns_->glEnableVertexAttribArray(2);
        glFuns_->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
        glFuns_->glEnableVertexAttribArray(3);
        glFuns_->glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
        glFuns_->glEnableVertexAttribArray(4);
        glFuns_->glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
    }
    glFuns_->glBindVertexArray(quadVAO);
    glFuns_->glDrawArrays(GL_TRIANGLES, 0, 6);
    glFuns_->glBindVertexArray(0);
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
    shader.setUniformValue("normalMap", 1);
    glFuns_->glActiveTexture(GL_TEXTURE0);
    glFuns_->glBindTexture(GL_TEXTURE_2D, diffuseTexture_->textureId());
    glFuns_->glActiveTexture(GL_TEXTURE1);
    glFuns_->glBindTexture(GL_TEXTURE_2D, normalMapTexture_->textureId());
    renderQuad();

//    shader.setUniformValue("skybox", 15);
//    quadMesh->Draw(shader);
}

NormalMapPlaneModel::~NormalMapPlaneModel() {
//    delete quadMesh;
}
