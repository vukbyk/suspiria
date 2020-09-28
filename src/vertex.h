#pragma once

#include <glm/glm.hpp>
#include <mesh.h>

class Vertex
{
public:
    glm::vec3 pos;
    glm::vec2 uv;
    glm::vec3 nrm;
    glm::vec3 tng;
    glm::vec3 bit;
  //ctg / [tbn] matrix

    Vertex(const glm::vec3& aPos,
           const glm::vec2& aUv = glm::vec2(0, 0),
           const glm::vec3& aNrm = glm::vec3(0, 0, 0),
           const glm::vec3& aTng = glm::vec3(0, 0, 0),
           const glm::vec3& aBit = glm::vec3(0, 0, 0)
          )
    {
        pos = aPos;
        uv  =  aUv;
        nrm = aNrm;
        tng = aTng;
        bit = aBit;
    }

};
