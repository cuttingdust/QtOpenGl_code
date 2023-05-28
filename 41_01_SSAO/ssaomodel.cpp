#include <random>
#include "ssaomodel.h"
#include "verticesData.h"

static unsigned int gBuffer;
static unsigned int gPosition, gNormal, gAlbedo;
static unsigned int ssaoFBO, ssaoBlurFBO;
static std::vector<QVector3D> ssaoKernel;
static unsigned int noiseTexture;
static unsigned int ssaoColorBuffer, ssaoColorBufferBlur;


static QVector3D lightPos = QVector3D(2.0f, 4.0f, -2.0f);
static QVector3D lightColor = QVector3D(0.2f, 0.2f, 0.7f);

float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}


void SSAOModel::processNode(aiNode *node, const aiScene *scene)
{
    cubeMesh_ = processMesh(nullptr, nullptr);
}

Mesh* SSAOModel::processMesh(aiMesh *mesh, const aiScene *scene)
{
//    glFuns_->glEnable(GL_DEPTH_TEST);
    // configure g-buffer framebuffer
    // ------------------------------
    glFuns_->glGenFramebuffers(1, &gBuffer);
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    // position color buffer
    glFuns_->glGenTextures(1, &gPosition);
    glFuns_->glBindTexture(GL_TEXTURE_2D, gPosition);
    glFuns_->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width_, height_, 0, GL_RGBA, GL_FLOAT, NULL);
    glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFuns_->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
    // normal color buffer
    glFuns_->glGenTextures(1, &gNormal);
    glFuns_->glBindTexture(GL_TEXTURE_2D, gNormal);
    glFuns_->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width_, height_, 0, GL_RGBA, GL_FLOAT, NULL);
    glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFuns_->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
    // color + specular color buffer
    glFuns_->glGenTextures(1, &gAlbedo);
    glFuns_->glBindTexture(GL_TEXTURE_2D, gAlbedo);
    glFuns_->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFuns_->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);
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
        qDebug() << "Framebuffer not complete!";
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, QOpenGLContext::currentContext()->defaultFramebufferObject());

    // also create framebuffer to hold SSAO processing stage
    // -----------------------------------------------------

    glFuns_->glGenFramebuffers(1, &ssaoFBO);
    glFuns_->glGenFramebuffers(1, &ssaoBlurFBO);
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

    // SSAO color buffer
    glFuns_->glGenTextures(1, &ssaoColorBuffer);
    glFuns_->glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
    glFuns_->glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width_, height_, 0, GL_RED, GL_FLOAT, NULL);
    glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFuns_->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
    if (glFuns_->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        qDebug() << "SSAO Framebuffer not complete!";
    // and blur stage
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
    glFuns_->glGenTextures(1, &ssaoColorBufferBlur);
    glFuns_->glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
    glFuns_->glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width_, height_, 0, GL_RED, GL_FLOAT, NULL);
    glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFuns_->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
    if (glFuns_->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        qDebug() << "SSAO Blur Framebuffer not complete!";
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, QOpenGLContext::currentContext()->defaultFramebufferObject());

    // generate sample kernel
    // ----------------------
    std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
    std::default_random_engine generator;

    for (unsigned int i = 0; i < 64; ++i)
    {
        QVector3D sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
        sample.normalize();;
        sample *= randomFloats(generator);
        float scale = float(i) / 64.0;

        // scale samples s.t. they're more aligned to center of kernel
        scale = lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;
        ssaoKernel.push_back(sample);
    }

    // generate noise texture
    // ----------------------
    std::vector<QVector3D> ssaoNoise;
    for (unsigned int i = 0; i < 16; i++)
    {
        QVector3D noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
        ssaoNoise.push_back(noise);
    }

    glFuns_->glGenTextures(1, &noiseTexture);
    glFuns_->glBindTexture(GL_TEXTURE_2D, noiseTexture);
    glFuns_->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
    glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glFuns_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

////////////////////////////////////////////////////////////////////////
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


SSAOModel::SSAOModel(QOpenGLFunctions_4_1_Core *glfuns, int width, int height, Model *model) :
        glFuns_(glfuns)
        , width_(width)
        , height_(height), model_(model) {
    auto fmt = QOpenGLContext::currentContext()->format();
    qDebug() << fmt.version();

    processNode(nullptr, nullptr);
}

void SSAOModel::Draw(QOpenGLShaderProgram &shader) {
    QMatrix4x4 model;
    // room cube
    model.setToIdentity();
    shader.bind();
    model.translate(QVector3D(0.0, 7.0f, 0.0f));
    model.scale(7.5f, 7.5f, 7.5f);
    shader.setUniformValue("model", model);
    shader.setUniformValue("invertedNormals", 1); // invert normals as we're inside the cube
    cubeMesh_->Draw(shader);
    shader.setUniformValue("invertedNormals", 0);
    // backpack model on the floor
    model.setToIdentity();
    model.translate(QVector3D(0.0f, 0.5f, 0.0));
    model.rotate(-90.0f, QVector3D(1.0, 0.0, 0.0));
    model.scale(1.0f);
    shader.setUniformValue("model", model);
    model_->Draw(shader);
}

SSAOModel::~SSAOModel() {
    delete cubeMesh_;
}

void SSAOModel::renderQuad() {

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

void SSAOModel::bindFramebuffer() {
      // 1. geometry pass: render scene's geometry/color data into gbuffer
    // -----------------------------------------------------------------
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);


}

void SSAOModel::releaseFramebuffer() {
     glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, QOpenGLContext::currentContext()->defaultFramebufferObject());
}

void SSAOModel::DrawSSAO(QOpenGLShaderProgram &shader, QMatrix4x4 projection) {
     // 2. generate SSAO texture
    // ------------------------
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
    glFuns_->glClear(GL_COLOR_BUFFER_BIT);
    shader.bind();

    shader.setUniformValue("gPosition", 0);
    shader.setUniformValue("gNormal", 1);
    shader.setUniformValue("texNoise", 2);
    shader.setUniformValue("_width", width_);
    shader.setUniformValue("_height", height_);

    // Send kernel + rotation
    for (unsigned int i = 0; i < 64; ++i){
        std::string str="samples[" + std::to_string(i) + "]";
        shader.setUniformValue(str.c_str(), ssaoKernel[i]);
    }
    shader.setUniformValue("projection", projection);
    glFuns_->glActiveTexture(GL_TEXTURE0);
    glFuns_->glBindTexture(GL_TEXTURE_2D, gPosition);
    glFuns_->glActiveTexture(GL_TEXTURE1);
    glFuns_->glBindTexture(GL_TEXTURE_2D, gNormal);
    glFuns_->glActiveTexture(GL_TEXTURE2);
    glFuns_->glBindTexture(GL_TEXTURE_2D, noiseTexture);
    renderQuad();
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, QOpenGLContext::currentContext()->defaultFramebufferObject());

}

void SSAOModel::DrawBlur(QOpenGLShaderProgram &shader) {
        // 3. blur SSAO texture to remove noise
    // ------------------------------------
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
    glFuns_->glClear(GL_COLOR_BUFFER_BIT);
    shader.bind();
    shader.setUniformValue("ssaoInput", 0);

    glFuns_->glActiveTexture(GL_TEXTURE0);
    glFuns_->glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
    renderQuad();
    glFuns_->glBindFramebuffer(GL_FRAMEBUFFER, QOpenGLContext::currentContext()->defaultFramebufferObject());

}

void SSAOModel::DrawLighting(QOpenGLShaderProgram &shader, QMatrix4x4 view) {
        // 4. lighting pass: traditional deferred Blinn-Phong lighting with added screen-space ambient occlusion
    // -----------------------------------------------------------------------------------------------------
    glFuns_->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.bind();
    shader.setUniformValue("gPosition", 0);
    shader.setUniformValue("gNormal", 1);
    shader.setUniformValue("gAlbedo", 2);
    shader.setUniformValue("ssao", 3);

    // send light relevant uniforms
    QVector3D lightPosView = QVector3D(view * QVector4D(lightPos, 1.0));
    shader.setUniformValue("light.Position", lightPosView);
    shader.setUniformValue("light.Color", lightColor);
    // Update attenuation parameters
    const float linear    = 0.09f;
    const float quadratic = 0.032f;
    shader.setUniformValue("light.Linear", linear);
    shader.setUniformValue("light.Quadratic", quadratic);
    glFuns_->glActiveTexture(GL_TEXTURE0);
    glFuns_->glBindTexture(GL_TEXTURE_2D, gPosition);
    glFuns_->glActiveTexture(GL_TEXTURE1);
    glFuns_->glBindTexture(GL_TEXTURE_2D, gNormal);
    glFuns_->glActiveTexture(GL_TEXTURE2);
    glFuns_->glBindTexture(GL_TEXTURE_2D, gAlbedo);
    glFuns_->glActiveTexture(GL_TEXTURE3); // add extra SSAO texture to lighting pass
    glFuns_->glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
    renderQuad();

}
