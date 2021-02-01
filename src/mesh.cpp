#include "mesh.h"
#include <iostream>
#include <fstream>
#include <string>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "qlogging.h"

//need to add CPU version. Most of this is moved from assimpload
Mesh::Mesh(const std::string fileName)
{
    initializeOpenGLFunctions();
    QFile file( std::string(":/assets/").append(fileName).c_str() );
    if(!file.open(QIODevice::ReadOnly))
        qDebug("Failed to load file %s", fileName.c_str());
    else
        qDebug("File %s is loaded", fileName.c_str());

    QByteArray DataFile = file.readAll();
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFileFromMemory( DataFile.data(), DataFile.size(),//buffer.data(), buffer.size(),
                                                        aiProcess_Triangulate |
                                                        aiProcess_GenSmoothNormals |
//                                                        aiProcess_FlipUVs |
                                                        aiProcess_CalcTangentSpace);//, "obj");
    if (!scene)
    {
        qDebug("Failed to load mesh: %s", file.symLinkTarget().toStdString().c_str());
        return;
    }

    for (unsigned int i = 0;i < scene->mNumMeshes; i++)
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
        createMesh(&vertices[0], vertices.size(), &indices[0], indices.size());
    }
    qDebug("Suicess to load mesh: %s", file.symLinkTarget().toStdString().c_str());
}


Mesh::Mesh(const Vertex vertices[], const GLuint vertSize, const GLuint indices[], const GLuint indexSize)
{
    initializeOpenGLFunctions();
    createMesh(vertices, vertSize, indices, indexSize);
}

Mesh::~Mesh(void)
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Mesh::createMesh(const Vertex vertices[], const GLuint vertSize, const GLuint indices[], const GLuint indexSize)
{
    verticesSize  = vertSize;
    indicesSize = indexSize;

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);                                                                    //         gl generisi bafer                     |VBO|
    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                                       //         state machine to              [AB] -> |VBO|
//    glBufferData(GL_ARRAY_BUFFER, (sizeof(acube) /*+ sizeof(amesh)*/), acube  , GL_STATIC_DRAW); // rezervisi mem i kopiraj podatke       [AB] -> |VBO|->{______ vertex___data_______}
    glBufferData(GL_ARRAY_BUFFER, vertSize * sizeof(Vertex), vertices, GL_STATIC_DRAW);
//    glBufferData(GL_ARRAY_BUFFER, vertSize * sizeof(Vertex), vertices, GL_STATIC_DRAW);
//    glBufferSubData(GL_ARRAY_BUFFER,             0, sizeof(acube), acube);
//    glBufferSubData(GL_ARRAY_BUFFER, sizeof(acube), sizeof(amesh), amesh);

    glGenBuffers(1, &IBO);                                                                      // gl generisi bafer          |EBO|
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);                                                 // state machine to  [EAB] -> |EBO|
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(acubei), acubei, GL_STATIC_DRAW);            //                   [EAB] -> |EBO|-> {________index___data________}

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * sizeof(GLuint), indices, GL_STATIC_DRAW);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * sizeof(GLuint), indices, GL_STATIC_DRAW);

    //Position attribute -> 0
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) 0);

    //UV attribute ->1
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(glm::vec3));

    //Normal attribute ->2
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

    //Tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3)));

    //Bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3) *2));

//    glBindVertexArray(0);

//    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,            0, sizeof(acubei), acubei);
////    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(acubei), sizeof(ameshi), ameshi);

////    glGenVertexArrays(1, &VAO2);
////    glBindVertexArray(VAO2);
////    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
////    glGenBuffers(1, &VBO2);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
////    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
////    glBufferData(GL_ARRAY_BUFFER, sizeof(mesh), mesh, GL_STATIC_DRAW);

////    //add triangle
////    glEnableVertexAttribArray(0);
////    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)sizeof(acube));

////    glEnableVertexAttribArray(1);
////    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(sizeof(acube) + (sizeof(GLfloat) * 3)));

//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
}

void Mesh::render(void)
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, /*(void*)*/0);
}
