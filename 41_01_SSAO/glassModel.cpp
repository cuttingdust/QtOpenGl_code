#include "glassModel.h"
#include "verticesData.h"
#include <QOpenGLContext>

static QVector3D gGlassPoints =  QVector3D(0.0f, 0.0f, 0.7f);

static unsigned int fbo;
static unsigned int rbo;
static unsigned int texture;

static unsigned int fboMultiSample;
static unsigned int rboMultiSample;
static unsigned int multiSampleTex;

void GlassModel::processNode(aiNode *node, const aiScene *scene)
{
    mesh_ = processMesh(nullptr, nullptr);
}

Mesh* GlassModel::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices(6);
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    int iSize = sizeof( gGlassVertices ) / sizeof( float );
//    memcpy(&vertices[0], gPlaneVertices, sizeof(gPlaneVertices));
    vertices.clear();
    for (int i = 0; i < iSize; i += 8) {
        Vertex vertex;
        // 处理顶点位置、法线和纹理坐标
        QVector3D vector;
        vector.setX(gGlassVertices[i + 0]);
        vector.setY(gGlassVertices[i + 1]);
        vector.setZ(gGlassVertices[i + 2]);
        vertex.position_ = vector;

        QVector2D vec;
        vec.setX(gGlassVertices[i + 3]);
        vec.setY(gGlassVertices[i + 4]);
        vertex.texCoords_ = vec;

        vector.setX(gGlassVertices[i + 5]);
        vector.setY(gGlassVertices[i + 6]);
        vector.setZ(gGlassVertices[i + 7]);
        vertex.normal_ = vector;

        vertices.push_back(vertex);
    }

    qDebug() << "#######################GlassMesh########################";
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
//    textures.push_back(tex);
    tex.id_ = specularTexture->textureId();
    tex.type_ = "texture_specular";
//    textures.push_back(tex);

    /// 创建一个自定义的multiSample帧缓冲
    glFuns_->glGenFramebuffers(1, &fboMultiSample);
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, fboMultiSample);
    glFuns_->glGenTextures(1, &multiSampleTex);
    glFuns_->glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, multiSampleTex);
    glFuns_->glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, width_, height_, GL_TRUE);
    glFuns_->glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFuns_->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, multiSampleTex, 0);
    glFuns_->glGenRenderbuffers(1, &rboMultiSample);
    glFuns_->glBindRenderbuffer(GL_RENDERBUFFER, rboMultiSample);
    glFuns_->glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width_, height_);
    glFuns_->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboMultiSample);
    /// 善后工作
    if (glFuns_->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        qDebug() << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!";
    glBindFramebuffer(GL_FRAMEBUFFER, QOpenGLContext::currentContext()->defaultFramebufferObject() );



    /// 创建一个自定义的帧缓冲
    glFuns_->glGenFramebuffers(1, &fbo);
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    /// 创建一个纹理并作为帧缓冲的附件
    glFuns_->glGenTextures(1, &texture);
    glFuns_->glBindTexture(GL_TEXTURE_2D, texture);
    glFuns_->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFuns_->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    /// 创建深度、模板缓冲对象，使用渲染缓冲对象
    glFuns_->glGenRenderbuffers(1, &rbo);
    glFuns_->glBindRenderbuffer(GL_RENDERBUFFER, rbo);

    /// 指定存储在 renderbuffer 中图像的宽高以及颜色格式，并按照此规格为之分配存储空间
    glFuns_->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width_, height_);

    /// 最后一件事就是附加这个渲染缓冲对象
    glFuns_->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    /// 善后工作
    if (glFuns_->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        qDebug() << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!";
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER,  QOpenGLContext::currentContext()->defaultFramebufferObject() );




    Texture frambufferTex;
    frambufferTex.id_= texture;
    frambufferTex.type_ = "texture_diffuse";
    textures.push_back(frambufferTex);

    return new Mesh(glFuns_ ,vertices, indices, textures);
}


GlassModel::GlassModel(QOpenGLFunctions_4_1_Core *glfuns, int width, int height):
    glFuns_(glfuns)
    , width_(width)
    , height_(height)
{
    auto fmt = QOpenGLContext::currentContext()->format();
    qDebug() << fmt.version();

//    glFuns_->glEnable(GL_BLEND);
//    glFuns_->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    processNode(nullptr, nullptr);
}

void GlassModel::Draw(QOpenGLShaderProgram &shader)
{
    QMatrix4x4 model;
    model.setToIdentity();
    model.translate(gGlassPoints);
    shader.bind();
    shader.setUniformValue("model", model);

//    for (unsigned int i = 0; i < mesh_->textures_.size(); i++) {
//        glFuns_->glActiveTexture(GL_TEXTURE0 + i);
//
//        glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//        glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    }

    glFuns_->glBindFramebuffer(GL_READ_FRAMEBUFFER, fboMultiSample);
    glFuns_->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
    glFuns_->glBlitFramebuffer(0, 0, width_, height_, 0, 0, width_, height_, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    // second pass
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER,QOpenGLContext::currentContext()->defaultFramebufferObject() ); // back to default

    mesh_->Draw(shader);
}

GlassModel::~GlassModel()
{
    delete mesh_;
    glFuns_->glDeleteFramebuffers(1, &fbo);
}

void GlassModel::bindFramer() {
    // first pass
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, fboMultiSample);
}
