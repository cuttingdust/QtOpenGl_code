#include "bloomcubemodel.h"
#include "verticesData.h"

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
static unsigned int bloomQuadVAO = 0;
static unsigned int bloomQuadVBO;
// lighting info
// -------------
// positions
static std::vector<QVector3D> bloomLightPositions;

// colors
static std::vector<QVector3D> bloomLightColors;

static unsigned int bloomHdrFBO;
static unsigned int bloomColorBuffers[2];
static unsigned int pingpongFBO[2];
static unsigned int pingpongColorbuffers[2];
static unsigned int bloomRboDepth;

static bool horizontal = true, first_iteration = true;

void BloomCubeModel::processNode(aiNode *node, const aiScene *scene)
{
    cubeMesh_ = processMesh(nullptr, nullptr);
}

Mesh* BloomCubeModel::processMesh(aiMesh *mesh, const aiScene *scene)
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

    QImage wall = QImage(":/image/images/container2.png").convertToFormat(QImage::Format_RGB888);
    auto diffuseTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    glFuns_->glBindTexture(GL_TEXTURE_2D, diffuseTexture->textureId());
    glFuns_->glTexImage2D(GL_TEXTURE_2D,0, GL_SRGB, wall.width(), wall.height(),0, GL_RGB, GL_UNSIGNED_BYTE, wall.bits());
    glFuns_->glGenerateMipmap(GL_TEXTURE_2D);


    auto specularTexture = new QOpenGLTexture(QImage(":/image/images/container2.png").mirrored());
    Texture tex;
    tex.id_= diffuseTexture->textureId();
    tex.type_ = "texture_diffuse";
    textures.push_back(tex);
    tex.id_ = specularTexture->textureId();
    tex.type_ = "texture_specular";
//    textures.push_back(tex);



    /// configure (floating point) framebuffers
    /// ---------------------------------------
    glFuns_->glGenFramebuffers(1, &bloomHdrFBO);
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, bloomHdrFBO);
    // create 2 floating point color buffers (1 for normal rendering, other for brightness threshold values)
    glFuns_->glGenTextures(2, bloomColorBuffers);
    for (unsigned int i = 0; i < 2; i++)
    {
        glFuns_->glBindTexture(GL_TEXTURE_2D, bloomColorBuffers[i]);
        glFuns_->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width_, height_, 0, GL_RGBA, GL_FLOAT, NULL);
        glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // attach texture to framebuffer
        glFuns_->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, bloomColorBuffers[i], 0);
    }
    // create and attach depth buffer (renderbuffer)

    glFuns_->glGenRenderbuffers(1, &bloomRboDepth);
    glFuns_->glBindRenderbuffer(GL_RENDERBUFFER, bloomRboDepth);
    glFuns_->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width_, height_);
    glFuns_->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, bloomRboDepth);
    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glFuns_->glDrawBuffers(2, attachments);
    // finally check if framebuffer is complete
    if (glFuns_->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        qDebug()<< "Framebuffer not complete!" ;
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, QOpenGLContext::currentContext()->defaultFramebufferObject());


    // ping-pong-framebuffer for blurring
    glFuns_->glGenFramebuffers(2, pingpongFBO);
    glFuns_->glGenTextures(2, pingpongColorbuffers);
    for (unsigned int i = 0; i < 2; i++)
    {
        glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glFuns_->glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
        glFuns_->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width_, height_, 0, GL_RGBA, GL_FLOAT, NULL);
        glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFuns_->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
        // also check if framebuffers are complete (no need for depth buffer)
        if (glFuns_->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            qDebug()<< "Framebuffer not complete!";
    }


    return new Mesh(glFuns_ ,vertices, indices, textures);
}

void BloomCubeModel::resizeGL(int w, int h)
{
    glFuns_->glViewport(0,0,w,h);
    for (unsigned int i = 0; i < 2; i++)
    {
        glFuns_->glBindTexture(GL_TEXTURE_2D, bloomColorBuffers[i]);
        glFuns_->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
    }

    for (unsigned int i = 0; i < 2; i++)
    {
        glFuns_->glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
        glFuns_->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
    }

    glFuns_->glBindRenderbuffer(GL_RENDERBUFFER, bloomRboDepth);
    glFuns_->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
}


void BloomCubeModel::renderQuad()
{
    if (bloomQuadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glFuns_->glGenVertexArrays(1, &bloomQuadVAO);
        glFuns_->glGenBuffers(1, &bloomQuadVBO);
        glFuns_->glBindVertexArray(bloomQuadVAO);
        glFuns_->glBindBuffer(GL_ARRAY_BUFFER, bloomQuadVBO);
        glFuns_->glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glFuns_->glEnableVertexAttribArray(0);
        glFuns_->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glFuns_->glEnableVertexAttribArray(1);
        glFuns_->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glFuns_->glBindVertexArray(bloomQuadVAO);
    glFuns_->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glFuns_->glBindVertexArray(0);
}

BloomCubeModel::BloomCubeModel(QOpenGLFunctions_4_1_Core *glfuns, int width, int height):
    glFuns_(glfuns)
    , width_(width)
    , height_(height)
{
    auto fmt = QOpenGLContext::currentContext()->format();
    qDebug() << fmt.version();

    bloomLightPositions.push_back(QVector3D(0.0f, 0.5f, 1.5f)); /// back light
    bloomLightPositions.push_back(QVector3D(-4.0f, 0.5f, -3.0f));
    bloomLightPositions.push_back(QVector3D(3.0f, 0.5f, 1.0f));
    bloomLightPositions.push_back(QVector3D(-.8f, 2.4f, -1.0f));

    bloomLightColors.push_back(QVector3D(5.0f, 5.0f, 5.0f));
    bloomLightColors.push_back(QVector3D(10.0f, 0.0f, 0.0f));
    bloomLightColors.push_back(QVector3D(0.0f, 0.0f, 15.0f));
    bloomLightColors.push_back(QVector3D(0.0f, 5.0f, 0.0f));


    processNode(nullptr, nullptr);
}

/// lighting
void BloomCubeModel::Draw(QOpenGLShaderProgram &shader) {
    QMatrix4x4 model;
    shader.bind();
    model.setToIdentity();

    for (unsigned int i = 0; i < bloomLightPositions.size(); i++){
        std::string str="lights[" + std::to_string(i) + "].Position";
        shader.setUniformValue(str.c_str(), bloomLightPositions[i]);
        str="lights[" + std::to_string(i) + "].Color";
        shader.setUniformValue(str.c_str(), bloomLightColors[i]);
    }


    model.translate(QVector3D(0.0f, -1.0f, 0.0));
    model.scale(12.5f, 0.5f, 12.5f);
    //model.rotate(60,QVector3D(1.0, 0.0, 1.0));
    shader.setUniformValue("model", model);
    cubeMesh_->Draw(shader);

    // then create multiple cubes as the scenery
    model.setToIdentity();
    model.translate(QVector3D(0.0f, 1.5f, 0.0));
    model.scale(0.5f);
    shader.setUniformValue("model", model);
    cubeMesh_->Draw(shader);

    model.setToIdentity();
    model.translate(QVector3D(2.0f, 0.0f, 1.0));
    model.scale(0.5f);
    shader.setUniformValue("model", model);
    cubeMesh_->Draw(shader);

    model.setToIdentity();
    model.translate(QVector3D(-1.0f, -1.0f, 2.0));
    model.rotate(60.0f, QVector3D(1.0, 0.0, 1.0));
    shader.setUniformValue("model", model);
    cubeMesh_->Draw(shader);

    model.setToIdentity();
    model.translate(QVector3D(0.0f, 2.7f, 4.0));
    model.rotate(23.0f,QVector3D(1.0, 0.0, 1.0).normalized());
    model.scale(1.25);
    shader.setUniformValue("model", model);
    cubeMesh_->Draw(shader);

    model.setToIdentity();
    model.translate(QVector3D(-2.0f, 1.0f, -3.0));
    model.rotate(124.0f, QVector3D(1.0, 0.0, 1.0).normalized());
    shader.setUniformValue("model", model);
    cubeMesh_->Draw(shader);

    model.setToIdentity();
    model.translate(QVector3D(-3.0f, 0.0f, 0.0));
    model.scale(0.5f);
    shader.setUniformValue("model", model);
    cubeMesh_->Draw(shader);
    shader.release();
}

BloomCubeModel::~BloomCubeModel() {
    delete cubeMesh_;
}

void BloomCubeModel::bindFrambuffer() {
    // 1. render scene into floating point framebuffer
    // -----------------------------------------------
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, bloomHdrFBO);
}

void BloomCubeModel::realeaseFrambuffer() {
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, QOpenGLContext::currentContext()->defaultFramebufferObject());
}

bool BloomCubeModel::isBBloom() const {
    return bBloom_;
}

void BloomCubeModel::setBBloom(bool bBloom) {
    bBloom_ = bBloom;
}

int BloomCubeModel::getExposure() const {
    return exposure_;
}

void BloomCubeModel::setExposure(int exposure) {
    BloomCubeModel::exposure_ = exposure;
}

/// light
void BloomCubeModel::DrawLight(QOpenGLShaderProgram &shader) {
   ///渲染灯源
    QMatrix4x4 model;
    shader.bind();
    for (unsigned int i = 0; i < bloomLightPositions.size(); i++)
    {
        model.setToIdentity();
        model.translate(QVector3D(bloomLightPositions[i]));
        model.scale(0.25f);
        shader.setUniformValue("model", model);
        shader.setUniformValue("lightColor", bloomLightColors[i]);
        cubeMesh_->Draw(shader);
    }
    shader.release();
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, QOpenGLContext::currentContext()->defaultFramebufferObject());

}

/// blur
void BloomCubeModel::DrawBlur(QOpenGLShaderProgram &shader) {
    // 2. blur bright fragments with two-pass Gaussian Blur
    // --------------------------------------------------
    unsigned int amount = 10;
    shader.bind();
    for (unsigned int i = 0; i < amount; i++)
    {
        glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
        shader.setUniformValue("horizontal", horizontal);
        glFuns_->glBindTexture(GL_TEXTURE_2D, first_iteration ? bloomColorBuffers[1] : pingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
        renderQuad();
        horizontal = !horizontal;
        if (first_iteration)
            first_iteration = false;
    }
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, QOpenGLContext::currentContext()->defaultFramebufferObject());
    shader.release();

}

/// bloom
void BloomCubeModel::DrawBloom(QOpenGLShaderProgram &shader) {
    // 3. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
    // --------------------------------------------------------------------------------------------------------------------------
    shader.bind();
    glFuns_->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glFuns_->glActiveTexture(GL_TEXTURE0);
    glFuns_->glBindTexture(GL_TEXTURE_2D, bloomColorBuffers[0]);
    shader.setUniformValue("bloomBlur",1);
    glFuns_->glActiveTexture(GL_TEXTURE1);
    glFuns_->glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);
    shader.setUniformValue("bloom", bBloom_);
    shader.setUniformValue("exposure", exposure_);
    renderQuad();
    shader.release();

}
