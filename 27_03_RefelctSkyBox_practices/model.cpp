#include "model.h"

void Model::loadModel(const std::string& path)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path,  aiProcess_Triangulate | aiProcess_FlipUVs);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        qDebug() << "ERROR::ASSIMP::" << import.GetErrorString();
        return;
    }
    directory_ = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);

}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        qDebug() << mesh->mName.data;
        meshes_.push_back(processMesh(mesh, scene) );
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }

}

Mesh* Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        if(maxX_ < mesh->mVertices[i].x) maxX_= mesh->mVertices[i].x;
        if(maxY_< mesh->mVertices[i].y) maxY_ = mesh->mVertices[i].y;
        if(minX_ > mesh->mVertices[i].x) minX_ = mesh->mVertices[i].x;
        if(minY_ > mesh->mVertices[i].y) minY_ = mesh->mVertices[i].y;

        Vertex vertex;
        // 处理顶点位置、法线和纹理坐标
        QVector3D vector;
        vector.setX(mesh->mVertices[i].x);
        vector.setY(mesh->mVertices[i].y);
        vector.setZ(mesh->mVertices[i].z);
        vertex.position_ = vector;

        vector.setX(mesh->mNormals[i].x);
        vector.setY(mesh->mNormals[i].y);
        vector.setZ(mesh->mNormals[i].z);
        vertex.normal_ = vector;

        if(mesh->mTextureCoords[0]) // 有纹理坐标？
        {
            QVector2D vec;
            vec.setX(mesh->mTextureCoords[0][i].x);
            vec.setY(mesh->mTextureCoords[0][i].y);
            vertex.texCoords_ = vec;
        } else
            vertex.texCoords_ = QVector2D(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    // 处理索引
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // 处理材质
    if(mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps =
                loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps =
                loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        std::vector<Texture> reflectionMaps =
                loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_reflection");
        textures.insert(textures.end(), reflectionMaps.begin(), reflectionMaps.end());
    }
    else
    {
        loadMaterialTextures(directory_ + "/diffuse.jpg", "texture_diffuse");
        loadMaterialTextures(directory_ + "/specular.jpg", "texture_specular");
        textures = texturesLoaded_;
    }

    return new Mesh(glFuns_,vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for(unsigned int j = 0; j < texturesLoaded_.size(); j++) {
            if(std::strcmp(texturesLoaded_[j].path_.data(), str.C_Str()) == 0) {
                textures.push_back(texturesLoaded_[j]);
                skip = true;
                break;
            }
        }
        if(!skip){
            Texture texture;
            texture.id_ = TextureFromFile(str.C_Str(), directory_);
            texture.type_ = typeName;
            texture.path_ = str.C_Str();
            textures.push_back(texture);
            texturesLoaded_.push_back(texture);
        }
    }
    return textures;
}

void Model::loadMaterialTextures(const std::string &filePath, std::string typeName) {

    QOpenGLTexture *textureData = new QOpenGLTexture(QImage(filePath.c_str()).mirrored());
    if(textureData == nullptr)
        qDebug()<<"texture is NULL";
    else
        qDebug()<<filePath.c_str()<<"loaded";

    Texture texture;
    texture.id_ = textureData->textureId();
    texture.type_ = typeName;
    texture.path_ = filePath;

    auto iter = std::find_if(texturesLoaded_.begin(), texturesLoaded_.end(), [&](Texture tex){
        return  tex.path_ == texture.path_;
    });
    if (iter == texturesLoaded_.end())
    {
        texturesLoaded_.push_back(texture);
    }
}

unsigned int Model::TextureFromFile(const char *path, const std::string &directory)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    QOpenGLTexture *texture = new QOpenGLTexture(QImage(filename.c_str()).mirrored());
    if(texture==NULL) qDebug()<<"texture is NULL";
    else qDebug()<<filename.c_str()<<"loaded";

    return texture->textureId();
}

Model::Model(QOpenGLFunctions_4_1_Core *glfuns, const char *path):
    glFuns_(glfuns)
{
    auto fmt = QOpenGLContext::currentContext()->format();
    qDebug() << fmt.version();
    loadModel(path);

}

void Model::Draw(QOpenGLShaderProgram &shader) {

//    QVector3D gCubePositions[] = {
//            QVector3D( 0.0f, 0.0f, 0.0f),
//            QVector3D( 2.0f, 5.0f, -15.0f),
//            QVector3D(-1.5f, -2.2f, -2.5f),
//            QVector3D(-3.8f, -2.0f, -12.3f),
//            QVector3D( 2.4f, -0.4f, -3.5f),
//            QVector3D(-1.7f, 3.0f, -7.5f),
//            QVector3D( 1.3f, -2.0f, -2.5f),
//            QVector3D( 1.5f, 2.0f, -2.5f),
//            QVector3D( 1.5f, 0.2f, -1.5f),
//            QVector3D(-1.3f, 1.0f, -1.5f)
//    };

    for(unsigned int i = 0; i < meshes_.size(); i++)
    {
//        QMatrix4x4 model;
//        model.translate(gCubePositions[i]);
        meshes_[i]->Draw(shader);
//        shader.bind();
//        shader.setUniformValue("model", model);
    }
}

Model::~Model() {
    meshes_.clear();
    texturesLoaded_.clear();
}
