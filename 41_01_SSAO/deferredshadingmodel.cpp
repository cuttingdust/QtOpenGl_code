#include <QtGui/QtGui>
#include "deferredshadingmodel.h"
#include "verticesData.h"

static std::vector<QVector3D> objectPositions;
static unsigned int gBuffer;
static unsigned int gPosition, gNormal, gAlbedoSpec;
static const unsigned int NR_LIGHTS = 32;
static std::vector<QVector3D> lightPositions;
static  std::vector<QVector3D> lightColors;

void DeferredShadingModel::processNode(aiNode *node, const aiScene *scene)
{
    cubeMesh_ = processMesh(nullptr, nullptr);
}

Mesh* DeferredShadingModel::processMesh(aiMesh *mesh, const aiScene *scene)
{
    // configure g-buffer framebuffer
    // ------------------------------
    glFuns_->glGenFramebuffers(1, &gBuffer);
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    /// position color buffer
    glFuns_->glGenTextures(1, &gPosition);
    glFuns_->glBindTexture(GL_TEXTURE_2D, gPosition);
    glFuns_->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width_, height_, 0, GL_RGB, GL_FLOAT, NULL);
    glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFuns_->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
    /// normal color buffer
    glFuns_->glGenTextures(1, &gNormal);
    glFuns_->glBindTexture(GL_TEXTURE_2D, gNormal);
    glFuns_->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width_, height_, 0, GL_RGB, GL_FLOAT, NULL);
    glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFuns_->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
    // color + specular color buffer
    glFuns_->glGenTextures(1, &gAlbedoSpec);
    glFuns_->glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    glFuns_->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFuns_->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glFuns_->glDrawBuffers(3, attachments);
    // create and attach depth buffer (renderbuffer)
    unsigned int rboDepth;
    glFuns_->glGenRenderbuffers(1, &rboDepth);
    glFuns_->glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glFuns_->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width_, height_);
    glFuns_->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    // finally check if framebuffer is complete
    if (glFuns_->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        qDebug() << "Framebuffer not complete!" ;
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, QOpenGLContext::currentContext()->defaultFramebufferObject());


    ////////////////////////////////////////////////////////////////////////////////////////////////
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

//    QImage wall = QImage(":/image/images/wood.png").convertToFormat(QImage::Format_RGB888);
//    auto diffuseTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);
//    glFuns_->glBindTexture(GL_TEXTURE_2D, diffuseTexture->textureId());
//    glFuns_->glTexImage2D(GL_TEXTURE_2D,0, GL_SRGB, wall.width(), wall.height(),0, GL_RGB, GL_UNSIGNED_BYTE, wall.bits());
//    glFuns_->glGenerateMipmap(GL_TEXTURE_2D);

    auto diffuseTexture = new QOpenGLTexture(QImage(":/image/images/wood.png").mirrored());
    auto specularTexture = new QOpenGLTexture(QImage(":/image/images/wood.png").mirrored());
    Texture tex;
    tex.id_= diffuseTexture->textureId();
    tex.type_ = "texture_diffuse";
//    textures.push_back(tex);
    tex.id_ = specularTexture->textureId();
    tex.type_ = "texture_specular";
//    textures.push_back(tex);

    return new Mesh(glFuns_ ,vertices, indices, textures);
}


void DeferredShadingModel::renderQuad()
{
// renderQuad() renders a 1x1 XY quad in NDC
    // -----------------------------------------
    unsigned int quadVAO = 0;
    unsigned int quadVBO;
   if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glFuns_->glGenVertexArrays(1, &quadVAO);
        glFuns_->glGenBuffers(1, &quadVBO);
        glFuns_->glBindVertexArray(quadVAO);
        glFuns_->glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glFuns_->glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glFuns_->glEnableVertexAttribArray(0);
        glFuns_->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glFuns_->glEnableVertexAttribArray(1);
        glFuns_->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glFuns_->glBindVertexArray(quadVAO);
    glFuns_->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glFuns_->glBindVertexArray(0);
}

DeferredShadingModel::DeferredShadingModel(QOpenGLFunctions_4_1_Core *glfuns, Model *model, int width, int height):
    glFuns_(glfuns)
    , model_(model)
    , width_(width)
    , height_(height)
{
    auto fmt = QOpenGLContext::currentContext()->format();
    qDebug() << fmt.version();

    objectPositions.push_back(QVector3D(-3.0, -0.5, -3.0));
    objectPositions.push_back(QVector3D( 0.0, -0.5, -3.0));
    objectPositions.push_back(QVector3D( 3.0, -0.5, -3.0));
    objectPositions.push_back(QVector3D(-3.0, -0.5,  0.0));
    objectPositions.push_back(QVector3D( 0.0, -0.5,  0.0));
    objectPositions.push_back(QVector3D( 3.0, -0.5,  0.0));
    objectPositions.push_back(QVector3D(-3.0, -0.5,  3.0));
    objectPositions.push_back(QVector3D( 0.0, -0.5,  3.0));
    objectPositions.push_back(QVector3D( 3.0, -0.5,  3.0));

    // lighting info
    // -------------
    srand(13);
    for (unsigned int i = 0; i < NR_LIGHTS; i++)
    {
        // calculate slightly random offsets
        float xPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
        float yPos = ((rand() % 100) / 100.0) * 6.0 - 4.0;
        float zPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
        lightPositions.push_back(QVector3D(xPos, yPos, zPos));
        // also calculate random color
        float rColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
        float gColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
        float bColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
        lightColors.push_back(QVector3D(rColor, gColor, bColor));
    }


    processNode(nullptr, nullptr);
}

void DeferredShadingModel::Draw(QOpenGLShaderProgram &shader) {
    QMatrix4x4 model;
    shader.bind();
    model.setToIdentity();

    for(int i=0;i<objectPositions.size();i++){
        model.setToIdentity();
        model.translate(/*modelInfo.worldPos*/objectPositions[i]);
        shader.setUniformValue("model", model);
        model_->Draw(shader);
    }
    shader.release();
}

DeferredShadingModel::~DeferredShadingModel() {
    delete cubeMesh_;
}

void DeferredShadingModel::bindFrambuffer() {
    // 1. geometry pass: render scene's geometry/color data into gbuffer
// -----------------------------------------------------------------
       glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
}

void DeferredShadingModel::realeaseFrambuffer() {
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, QOpenGLContext::currentContext()->defaultFramebufferObject());
}

bool DeferredShadingModel::isBhdr() const {
    return bHDR_;
}

void DeferredShadingModel::setBhdr(bool bHdr) {
    bHDR_ = bHdr;
}

int DeferredShadingModel::getExposure() const {
    return exposure_;
}

void DeferredShadingModel::setExposure(int exposure) {
    exposure_ = exposure;
}

void DeferredShadingModel::DrawLighting(QOpenGLShaderProgram &shader) {
    // 2. lighting pass: calculate lighting by iterating over a screen filled quad pixel-by-pixel using the gbuffer's content.
// -----------------------------------------------------------------------------------------------------------------------
    glFuns_-> glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    shader.bind();
    shader.setUniformValue("gPosition", 0);
    shader.setUniformValue("gNormal", 1);
    shader.setUniformValue("gAlbedoSpec", 2);
    glFuns_->glActiveTexture(GL_TEXTURE0);
    glFuns_->glBindTexture(GL_TEXTURE_2D, gPosition);
    glFuns_->glActiveTexture(GL_TEXTURE1);
    glFuns_->glBindTexture(GL_TEXTURE_2D, gNormal);
    glFuns_->glActiveTexture(GL_TEXTURE2);
    glFuns_->glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
   // send light relevant uniforms
    for (unsigned int i = 0; i < lightPositions.size(); i++)
    {
        std::string str="lights.Position";
        shader.setUniformValue(str.c_str(), lightPositions[i]);
        str="lights.Color";
        shader.setUniformValue(str.c_str(), lightColors[i]);
        // update attenuation parameters and calculate radius
        const float constant = 1.0f; // note that we don't send this to the shader, we assume it is always 1.0 (in our case)
        const float linear = 0.7f;
        const float quadratic = 1.8f;
        str="lights.Linear";
        shader.setUniformValue(str.c_str(), linear);
        str="lights.Quadratic";
        shader.setUniformValue(str.c_str(), quadratic);
        // then calculate radius of light volume/sphere
        const float maxBrightness = std::fmaxf(std::fmaxf(lightColors[i].x(), lightColors[i].y()), lightColors[i].z());
        float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);
        str="lights.Radius";
        shader.setUniformValue(str.c_str(), radius);
        QMatrix4x4 model;
        model.setToIdentity();
        model.translate(lightPositions[i]);
        model.scale(radius);
        shader.setUniformValue("model",model);
        // finally render quad
       cubeMesh_->Draw(shader);
    }

    shader.release();

// 2.5. copy content of geometry's depth buffer to default framebuffer's depth buffer
// ----------------------------------------------------------------------------------
    glFuns_->glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
    glFuns_->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, QOpenGLContext::currentContext()->defaultFramebufferObject()); // write to default framebuffer
    // blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
    // the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the
    // depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
    glBlitFramebuffer(0, 0, width_, height_, 0, 0, width_, height_, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, QOpenGLContext::currentContext()->defaultFramebufferObject());


}

void DeferredShadingModel::DrawLightingBox(QOpenGLShaderProgram &shader) {
// 3. render lights on top of scene
// --------------------------------
    shader.bind();
    QMatrix4x4 model;
    model.setToIdentity();
    for (unsigned int i = 0; i < lightPositions.size(); i++)
    {
        model.setToIdentity();
        model.translate(lightPositions[i]);
        model.scale(0.125f);
        shader.setUniformValue("model", model);
        shader.setUniformValue("lightColor", lightColors[i]);
        cubeMesh_->Draw(shader);
    }
    shader.release();
}
