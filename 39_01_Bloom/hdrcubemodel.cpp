#include "hdrcubemodel.h"
#include "verticesData.h"

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
static unsigned int quadVAO = 0;
static unsigned int quadVBO;
// lighting info
// -------------
// positions
static std::vector<QVector3D> lightPositions;

// colors
static std::vector<QVector3D> lightColors;

static unsigned int hdrFBO;
static unsigned int colorBuffer;

void HDRCubeModel::processNode(aiNode *node, const aiScene *scene)
{
    cubeMesh_ = processMesh(nullptr, nullptr);
}

Mesh* HDRCubeModel::processMesh(aiMesh *mesh, const aiScene *scene)
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

    QImage wall = QImage(":/image/images/wood.png").convertToFormat(QImage::Format_RGB888);
    auto diffuseTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    glFuns_->glBindTexture(GL_TEXTURE_2D, diffuseTexture->textureId());
    glFuns_->glTexImage2D(GL_TEXTURE_2D,0, GL_SRGB, wall.width(), wall.height(),0, GL_RGB, GL_UNSIGNED_BYTE, wall.bits());
    glFuns_->glGenerateMipmap(GL_TEXTURE_2D);


    auto specularTexture = new QOpenGLTexture(QImage(":/image/images/wood.png").mirrored());
    Texture tex;
    tex.id_= diffuseTexture->textureId();
    tex.type_ = "texture_diffuse";
    textures.push_back(tex);
    tex.id_ = specularTexture->textureId();
    tex.type_ = "texture_specular";
//    textures.push_back(tex);



    // configure floating point framebuffer
    // ------------------------------------

    glFuns_->glGenFramebuffers(1, &hdrFBO);
    // create floating point color buffer

    glFuns_->glGenTextures(1, &colorBuffer);
    glFuns_->glBindTexture(GL_TEXTURE_2D, colorBuffer);
    glFuns_->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width_, height_, 0, GL_RGBA, GL_FLOAT, NULL);
    glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // create depth buffer (renderbuffer)
    unsigned int rboDepth;
    glFuns_->glGenRenderbuffers(1, &rboDepth);
    glFuns_->glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glFuns_->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width_, height_);
    // attach buffers
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    glFuns_->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
    glFuns_->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    if (glFuns_->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        qDebug() << "Framebuffer not complete!" ;
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, QOpenGLContext::currentContext()->defaultFramebufferObject());


    return new Mesh(glFuns_ ,vertices, indices, textures);
}


void HDRCubeModel::renderQuad(QOpenGLShaderProgram &shader)
{
    shader.bind();
    glFuns_->glActiveTexture(GL_TEXTURE0);
    glFuns_->glBindTexture(GL_TEXTURE_2D, colorBuffer);
    shader.setUniformValue("hdrBuffer", 0);
    shader.setUniformValue("hdr", bHDR);
    shader.setUniformValue("exposure", exposure);

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

    shader.release();
}

HDRCubeModel::HDRCubeModel(QOpenGLFunctions_4_1_Core *glfuns, int width, int height):
    glFuns_(glfuns)
    , width_(width)
    , height_(height)
{
    auto fmt = QOpenGLContext::currentContext()->format();
    qDebug() << fmt.version();

    lightPositions.push_back(QVector3D( 0.0f,  0.0f, 49.5f)); // back light
    lightPositions.push_back(QVector3D(-1.4f, -1.9f, 9.0f));
    lightPositions.push_back(QVector3D( 0.0f, -1.8f, 4.0f));
    lightPositions.push_back(QVector3D( 0.8f, -1.7f, 6.0f));

    lightColors.push_back(QVector3D(200.0f, 200.0f, 200.0f));
    lightColors.push_back(QVector3D(0.1f, 0.0f, 0.0f));
    lightColors.push_back(QVector3D(0.0f, 0.0f, 0.2f));
    lightColors.push_back(QVector3D(0.0f, 0.1f, 0.0f));


    processNode(nullptr, nullptr);
}

void HDRCubeModel::Draw(QOpenGLShaderProgram &shader) {
    QMatrix4x4 model;
    shader.bind();
    model.setToIdentity();
    model.translate(QVector3D(0.0f, 0.0f, 25.0));
    model.scale(QVector3D(2.5f, 2.5f, 27.5f));
    //model.rotate(60,QVector3D(1.0, 0.0, 1.0));
    shader.setUniformValue("model", model);
    shader.setUniformValue("skybox", 15);

    for (unsigned int i = 0; i < lightPositions.size(); i++){
        std::string str="lights[" + std::to_string(i) + "].Position";
        shader.setUniformValue(str.c_str(), lightPositions[i]);
        str="lights[" + std::to_string(i) + "].Color";
        shader.setUniformValue(str.c_str(), lightColors[i]);
    }

    shader.setUniformValue("inverse_normals", true);

    cubeMesh_->Draw(shader);
    shader.release();
}

HDRCubeModel::~HDRCubeModel() {
    delete cubeMesh_;
}

void HDRCubeModel::bindFrambuffer() {
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
}

void HDRCubeModel::realeaseFrambuffer() {
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, QOpenGLContext::currentContext()->defaultFramebufferObject());
}

bool HDRCubeModel::isBhdr() const {
    return bHDR;
}

void HDRCubeModel::setBhdr(bool bHdr) {
    bHDR = bHdr;
}

int HDRCubeModel::getExposure() const {
    return exposure;
}

void HDRCubeModel::setExposure(int exposure) {
    HDRCubeModel::exposure = exposure;
}
