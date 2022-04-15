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

#include <iostream>
#include <fstream>
#include "mesh.h"

#include <world.h>


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
        loadScene(scene, fileName);
    }
}

void MeshManager::loadScene(const aiScene* scene, const std::string fileName)
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
