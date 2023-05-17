#include "depthmapmodel.h"
#include "verticesData.h"

const static unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
static unsigned int depthMapFBO;

const static float near_plane = 1.0f, far_plane = 25.0f;
static QMatrix4x4 lightSpaceMatrix;
static unsigned int depthCubemap;

void DepthMapModel::processNode(aiNode *node, const aiScene *scene)
{
    quadMesh_ = processMesh(nullptr, nullptr);
}

Mesh* DepthMapModel::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    int iSize = sizeof( gSscreenQuadVertices ) / sizeof( float );

//    memcpy(&vertices[0], gCubeVertices, sizeof(gCubeVertices));
    vertices.clear();
    for (int i = 0; i < iSize; i += 8) {
        Vertex vertex;
        // 处理顶点位置、法线和纹理坐标
        QVector3D vector;
        vector.setX(gSscreenQuadVertices[i + 0]);
        vector.setY(gSscreenQuadVertices[i + 1]);
        vector.setZ(gSscreenQuadVertices[i + 2]);
        vertex.position_ = vector;

        QVector2D vec;
        vec.setX(gSscreenQuadVertices[i + 3]);
        vec.setY(gSscreenQuadVertices[i + 4]);
        vertex.texCoords_ = vec;

        vector.setX(gSscreenQuadVertices[i + 5]);
        vector.setY(gSscreenQuadVertices[i + 6]);
        vector.setZ(gSscreenQuadVertices[i + 7]);
        vertex.normal_ = vector;

        vertices.push_back(vertex);
    }

     qDebug() << "#######################DepthMapMesh########################";
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
//    textures.push_back(tex);
    tex.id_ = specularTexture->textureId();
    tex.type_ = "texture_specular";
//    textures.push_back(tex);

    /// configure depth map FBO
    /// -----------------------
    glFuns_->glGenFramebuffers(1, &depthMapFBO);
    /// create depth texture
    glFuns_->glGenTextures(1, &depthCubemap);
    glFuns_->glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    glFuns_->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFuns_->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFuns_->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glFuns_->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // attach depth texture as FBO's depth buffer
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFuns_->glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    glFuns_->glDrawBuffer(GL_NONE);
    glFuns_->glReadBuffer(GL_NONE);
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, QOpenGLContext::currentContext()->defaultFramebufferObject());

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        qDebug() << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!";
    glBindFramebuffer(GL_FRAMEBUFFER, QOpenGLContext::currentContext()->defaultFramebufferObject() );

    glEnable(GL_CULL_FACE);

    return nullptr;

}


DepthMapModel::DepthMapModel(QOpenGLFunctions_4_1_Core *glfuns, int width, int height):
    glFuns_(glfuns)
    , width_(width)
    , height_(height)
{

    auto fmt = QOpenGLContext::currentContext()->format();
    qDebug() << fmt.version();

    processNode(nullptr, nullptr);
}

void DepthMapModel::Draw(QOpenGLShaderProgram &shader) {
//    QMatrix4x4 model;
//    shader.bind();
//    // reset viewport
//    glFuns_->glViewport(0, 0, width_ , height_);
//   // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    // render Depth map to quad for visual debugging
//    // ---------------------------------------------
//    quadMesh_->Draw(shader);
//    shader.release();

//    shader.bind();
//    shader.setUniformValue("lightSpaceMatrix", lightSpaceMatrix);
//    glFuns_->glViewport(0, 0, width_, height_);
//    glFuns_->glActiveTexture(GL_TEXTURE1);
//    glFuns_->glBindTexture(GL_TEXTURE_2D, depthMap);
//    shader.setUniformValue("depthMap",1);

    shader.bind();
    shader.setUniformValue("far_plane", far_plane);
    shader.setUniformValue("shadows", true);
    shader.setUniformValue("depthMap",1);
    glFuns_->glActiveTexture(GL_TEXTURE1);
    glFuns_->glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
}

DepthMapModel::~DepthMapModel() {
    delete quadMesh_;
}

void DepthMapModel::bindFramer(QOpenGLShaderProgram &shader, QVector3D lightPos) {
//    shader.bind();
//    /// 1. render depth of scene to texture (from light's perspective)
//    /// --------------------------------------------------------------
//    QMatrix4x4 lightProjection, lightView;
//
//    lightProjection.ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
//    lightView.lookAt(lightPos, QVector3D(0.0,0.0,0.0), QVector3D(0.0, 1.0, 0.0));
//    lightSpaceMatrix = lightProjection * lightView;
//
//    glFuns_->glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
//    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
//    glFuns_->glClear(GL_DEPTH_BUFFER_BIT);
//    shader.setUniformValue("lightSpaceMatrix", lightSpaceMatrix);
//
//    glFuns_->glEnable(GL_CULL_FACE);
//    glFuns_->glCullFace(GL_FRONT);


    /// 0. create depth cubemap transformation matrices
    /// -----------------------------------------------
    float near_plane = 1.0f;
    float far_plane  = 25.0f;
    QMatrix4x4 shadowProj;
    QMatrix4x4 shadowView;
    shadowProj.perspective(90.0f, (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
    std::vector<QMatrix4x4> shadowTransforms;
    shadowView.lookAt(lightPos, lightPos + QVector3D( 1.0, 0.0, 0.0), QVector3D(0.0,-1.0, 0.0));
    shadowTransforms.push_back(shadowProj * shadowView);    shadowView.setToIdentity();
    shadowView.lookAt(lightPos, lightPos + QVector3D(-1.0, 0.0, 0.0), QVector3D(0.0,-1.0, 0.0));
    shadowTransforms.push_back(shadowProj * shadowView);    shadowView.setToIdentity();
    shadowView.lookAt(lightPos, lightPos + QVector3D( 0.0, 1.0, 0.0), QVector3D(0.0, 0.0, 1.0));
    shadowTransforms.push_back(shadowProj * shadowView);    shadowView.setToIdentity();
    shadowView.lookAt(lightPos, lightPos + QVector3D( 0.0,-1.0, 0.0), QVector3D(0.0, 0.0,-1.0));
    shadowTransforms.push_back(shadowProj * shadowView);    shadowView.setToIdentity();
    shadowView.lookAt(lightPos, lightPos + QVector3D( 0.0, 0.0, 1.0), QVector3D(0.0,-1.0, 0.0));
    shadowTransforms.push_back(shadowProj * shadowView);    shadowView.setToIdentity();
    shadowView.lookAt(lightPos, lightPos + QVector3D( 0.0, 0.0,-1.0), QVector3D(0.0,-1.0, 0.0));
    shadowTransforms.push_back(shadowProj * shadowView);

    // 1. render scene to depth cubemap
    // --------------------------------
    glFuns_->glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFuns_->glClear(GL_DEPTH_BUFFER_BIT);
    shader.bind();
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFuns_->glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glFuns_->glClear(GL_DEPTH_BUFFER_BIT);
    for (unsigned int i = 0; i < 6; ++i){
        std::string str="shadowMatrices[" + std::to_string(i) + "]";
        shader.setUniformValue(str.c_str(), shadowTransforms[i]);
    }
    shader.setUniformValue("far_plane", far_plane);
    shader.setUniformValue("lightPos", lightPos);

}
