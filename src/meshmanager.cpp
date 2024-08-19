//#include "CustomIOSystem.h"
#include "meshmanager.h"
//#include "components/MeshRenderer.h"

//#include "Logger.h"
#include "qlogging.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>


#include <string>
#include <QFile>
#include <QFileInfo>
//#include <QMessageBox>

// #include <iostream>
// #include <fstream>

#include "mesh.h"

#include <world.h>
// #include <tiny_gltf.h>

MeshManager::~MeshManager()
{
    for ( const auto &m : mesh )
    {
        delete m.second;
    }
}



void MeshManager::loadAssimp(const std::string fileName)
{
    if(mesh.count(fileName))
    {
            qDebug("Mesh \"%s\" already loaded", fileName.c_str());
            return;
    }

    QFile FullFilePath( std::string(":/assets/").append(fileName).c_str() );



    if(!FullFilePath.open(QIODevice::ReadOnly))
        qDebug("!!! ERROR !!! Failed to load file \"%s\"", fileName.c_str());
    else
        qDebug("File \"%s\" is loaded into mesh", fileName.c_str());

    QByteArray DataFile = FullFilePath.readAll();
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFileFromMemory( DataFile.data(), DataFile.size(),//buffer.data(), buffer.size(),
                                                        aiProcess_Triangulate |
                                                        aiProcess_GenSmoothNormals |
//                                                        aiProcess_FlipUVs |
                                                        aiProcess_CalcTangentSpace);//, "obj");
    if (!scene)
    {
        qDebug("!!! ERROR !!! Failed to load mesh: %s", FullFilePath.symLinkTarget().toStdString().c_str());
        return;
    }
    else
    {
        qDebug("Success to load mesh: %s", FullFilePath.symLinkTarget().toStdString().c_str());
        loadAssimpScene(scene, fileName);
    }
}

// void MeshManager::loadGltf(const std::string fileName)
// {
//     if(mesh.count(fileName))
//     {
//         qDebug("Mesh \"%s\" already loaded", fileName.c_str());
//         return;
//     }

//     QFile FullFilePath( std::string(":/assets/").append(fileName).c_str() );



//     if(!FullFilePath.open(QIODevice::ReadOnly))
//         qDebug("!!! ERROR !!! Failed to load file \"%s\"", fileName.c_str());
//     else
//         qDebug("File \"%s\" is loaded into mesh", fileName.c_str());

//     QByteArray DataFile = FullFilePath.readAll();

//     tinygltf::Model gltfmodel;
//     tinygltf::TinyGLTF loader;
//     std::string err;
//     std::string warn;
//     std::string gltf_json = DataFile.toStdString();


//     bool ret = loader.LoadASCIIFromString(&gltfmodel, &err, &warn,
//                                            gltf_json.c_str(),                // Pointer to the JSON data
//                                            static_cast<unsigned int>(gltf_json.size()),  // Length of the JSON data
//                                            QFileInfo(FullFilePath).absolutePath().toStdString()); // Base directory
//     if (!warn.empty())
//     {
//         qDebug("Warning: %s", warn.c_str());
//     }
//     if (!ret)
//     {
//         qDebug("!!! ERROR !!! Failed to load glTF model: %s", err.c_str());
//         return;
//     }
//     else
//     {
//         qDebug("Success to load glTF model: %s", fileName.c_str());
//         loadGltfScene(gltfmodel, fileName);
//     }
// }

void MeshManager::loadAssimpScene(const aiScene* scene, const std::string fileName)
{
    for (unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        const aiMesh* model = scene->mMeshes[i];

        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);
        for(GLuint j = 0; j < model->mNumVertices; j++)
        {
            const aiVector3D* pos = &(model->mVertices[j]);
            const aiVector3D* uv = model->HasTextureCoords(0) ? &(model->mTextureCoords[0][j]) : &aiZeroVector;
            const aiVector3D* normal = &(model->mNormals[j]);
            const aiVector3D* tng = model->HasTangentsAndBitangents() ? &(model->mTangents[j]) : &aiZeroVector;
            const aiVector3D* bit = model->HasTangentsAndBitangents() ? &(model->mBitangents[j]) : &aiZeroVector;

            Vertex vert(glm::vec3(pos->x, pos->y, pos->z),
                        glm::vec2(uv->x, uv->y),
                        glm::vec3(normal->x, normal->y, normal->z),
                        glm::vec3(tng->x, tng->y, tng->z),
                        glm::vec3(bit->x, bit->y, bit->z)
                        );

            vertices.push_back(vert);
        }

        for(GLuint j = 0; j < model->mNumFaces; j++)
        {
            const aiFace& face = model->mFaces[j];
            indices.push_back(face.mIndices[0]);
            indices.push_back(face.mIndices[1]);
            indices.push_back(face.mIndices[2]);
        }

        mesh[fileName]=new Mesh(&vertices[0], vertices.size(), &indices[0], indices.size());;

    }
}

// void MeshManager::loadGltfScene(const tinygltf::Model &model, const std::string fileName)
// {
//     // for (const auto &mesh : model.meshes)
//     for(int i=0; i< model.meshes.size();i++)
//     {
//         tinygltf::Mesh gltfmesh = model.meshes[i];
//         std::vector<Vertex> vertices;
//         std::vector<GLuint> indices;

//         const auto &primitive = gltfmesh.primitives[0]; // Assuming the first primitive
//         const tinygltf::Accessor &posAccessor = model.accessors[primitive.attributes.find("POSITION")->second];
//         const tinygltf::BufferView &posBufferView = model.bufferViews[posAccessor.bufferView];
//         const tinygltf::Buffer &posBuffer = model.buffers[posBufferView.buffer];

//         const tinygltf::Accessor &normalAccessor = model.accessors[primitive.attributes.find("NORMAL")->second];
//         const tinygltf::BufferView &normalBufferView = model.bufferViews[normalAccessor.bufferView];
//         const tinygltf::Buffer &normalBuffer = model.buffers[normalBufferView.buffer];

//         const tinygltf::Accessor &uvAccessor = model.accessors[primitive.attributes.find("TEXCOORD_0")->second];
//         const tinygltf::BufferView &uvBufferView = model.bufferViews[uvAccessor.bufferView];
//         const tinygltf::Buffer &uvBuffer = model.buffers[uvBufferView.buffer];

//         const float *posData = reinterpret_cast<const float *>(&posBuffer.data[posBufferView.byteOffset + posAccessor.byteOffset]);
//         const float *normalData = reinterpret_cast<const float *>(&normalBuffer.data[normalBufferView.byteOffset + normalAccessor.byteOffset]);
//         const float *uvData = reinterpret_cast<const float *>(&uvBuffer.data[uvBufferView.byteOffset + uvAccessor.byteOffset]);

//         for (size_t i = 0; i < posAccessor.count; ++i)
//         {
//             glm::vec3 pos(posData[i * 3 + 0], posData[i * 3 + 1], posData[i * 3 + 2]);
//             glm::vec3 normal(normalData[i * 3 + 0], normalData[i * 3 + 1], normalData[i * 3 + 2]);
//             glm::vec2 uv(uvData[i * 2 + 0], uvData[i * 2 + 1]);

//             Vertex vert(pos, uv, normal);
//             vertices.push_back(vert);
//         }

//         const tinygltf::Accessor &indexAccessor = model.accessors[primitive.indices];
//         const tinygltf::BufferView &indexBufferView = model.bufferViews[indexAccessor.bufferView];
//         const tinygltf::Buffer &indexBuffer = model.buffers[indexBufferView.buffer];

//         if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
//         {
//             const unsigned short *indexData = reinterpret_cast<const unsigned short *>(&indexBuffer.data[indexBufferView.byteOffset + indexAccessor.byteOffset]);
//             for (size_t i = 0; i < indexAccessor.count; ++i)
//             {
//                 indices.push_back(static_cast<GLuint>(indexData[i]));
//             }
//         }
//         else if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
//         {
//             const unsigned int *indexData = reinterpret_cast<const unsigned int *>(&indexBuffer.data[indexBufferView.byteOffset + indexAccessor.byteOffset]);
//             for (size_t i = 0; i < indexAccessor.count; ++i)
//             {
//                 indices.push_back(static_cast<GLuint>(indexData[i]));
//             }
//         }
//         mesh[fileName] = new Mesh(&vertices[0], vertices.size(), &indices[0], indices.size());
//     }
// }

Mesh* MeshManager::get(const std::string fileName)
{
    if(mesh.count(fileName))
    {
        return mesh[fileName];
    }
    else
    {
        qDebug("!!! ERROR !!! Mesh \"%s\" not loaded. Try to load...", fileName.c_str());
        loadAssimp(fileName);

        return mesh[fileName];
    }
}
