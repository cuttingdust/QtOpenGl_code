#include "mesh.h"

Mesh::Mesh(QOpenGLFunctions_4_1_Core *glFuns
           , std::vector<Vertex> vertices
           , std::vector<unsigned int> indices
           , std::vector<Texture> textures)
    : glFuns_(glFuns)
    , pel_(GL_TRIANGLES)
    , bInstancing_(false)
    , instancingNum_(0)
{
    this->vertices_ = vertices;
    this->indices_ = indices;
    this->textures_ = textures;
    setupMesh();
}

void Mesh::setupMesh()
{
    /// 创建VBO和VAO对象，并赋予ID
    glFuns_->glGenVertexArrays(1, &VAO_);
    glFuns_->glGenBuffers(1, &VBO_);

    /// 绑定VBO和VAO对象
    glFuns_->glBindVertexArray(VAO_);
    glFuns_->glBindBuffer(GL_ARRAY_BUFFER, VBO_);

    /// 为当前绑定到target的缓冲区对象创建一个新的数据存储。
    /// 如果data不是NULL，则使用来自此指针的数据初始化数据存储
    glFuns_->glBufferData(GL_ARRAY_BUFFER, vertices_.size()*sizeof(Vertex),
                           &vertices_[0], GL_STATIC_DRAW);

    /// 告知显卡如何解析缓冲里的属性值
    glFuns_->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glFuns_->glEnableVertexAttribArray(0);

    glFuns_->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoords_)));
    glFuns_->glEnableVertexAttribArray(1);

    glFuns_->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal_)));
    glFuns_->glEnableVertexAttribArray(2);

    glFuns_->glGenBuffers(1,&EBO_);
    glFuns_->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glFuns_->glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                          indices_.size() * sizeof(unsigned int),&indices_[0], GL_STATIC_DRAW);
}


void Mesh::Draw(QOpenGLShaderProgram &shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int emissionNr = 1;
    unsigned int reflectionNr = 1;
    unsigned int normalMapNr = 1;

    for(unsigned int i = 0; i < textures_.size(); i++) {
        glFuns_->glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = textures_[i].type_;
        if(name == "texture_diffuse")
              number = std::to_string(diffuseNr++);
        else if(name == "texture_specular")
            number = std::to_string(specularNr++);
        else if(name == "texture_emission")
            number = std::to_string(emissionNr++);
        else if(name == "texture_reflect")
            number = std::to_string(reflectionNr++);
        else if(name == "texture_normal1")
            number = std::to_string(normalMapNr++);

        shader.setUniformValue(("material." + name + number).c_str(), i);
        glFuns_->glBindTexture(GL_TEXTURE_2D, textures_[i].id_);
    }

    glFuns_->glBindVertexArray(this->VAO_);
    glFuns_->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO_);
    if (bInstancing_)
    {
        glFuns_->glDrawArraysInstanced(GL_TRIANGLES, 0, indices_.size(),instancingNum_);
    }
    else
    {
         glFuns_->glDrawElements(pel_,indices_.size(),GL_UNSIGNED_INT,0);
    }

}

Mesh::~Mesh() {
    glFuns_->glDeleteVertexArrays(1, &VAO_);
    glFuns_->glDeleteBuffers(1, &VBO_);
    glFuns_->glDeleteBuffers(1, &EBO_);
}

void Mesh::setPel1(GLenum pel) {
    pel_ = pel;
}

bool Mesh::isBInstancing() const {
    return bInstancing_;
}

void Mesh::setBInstancing(bool bInstancing) {
    bInstancing_ = bInstancing;
}

int Mesh::getInstancingNum() const {
    return instancingNum_;
}

void Mesh::setInstancingNum(int instancingNum) {
    instancingNum_ = instancingNum;
}

unsigned int Mesh::getVao() {
    return VAO_;
}



