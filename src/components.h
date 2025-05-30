#ifndef ECSCOMPONENTS_H
#define ECSCOMPONENTS_H

#include "transform.h"
#include "camera.h"
// #include "glmtransform.h"
#include <glm/gtc/type_ptr.hpp>
#include "shaderprogram.h"
#include <QOpenGLExtraFunctions>
// #include <string>
#include <entt/entt.hpp>
#include <vector>

//struct SimpleRenderComponent
//{

////    Transform transform;
////    btScalar transform[16];


//    GLuint VAO;
////    GLuint VBO;
////    GLuint IBO;
//    GLuint indicesSize;

//    GLuint albedoId;
//    GLuint normalId;

//    SimpleRenderComponent()=default;
//    SimpleRenderComponent(const GLuint aVAO, const GLuint aIndicesSize, const GLuint albedo, const GLuint normal)
//    :VAO(aVAO), indicesSize(aIndicesSize), albedoId(albedo), normalId(normal) {};
//};

//struct CameraComponent
//{
//    GLfloat zoom=1.0f;
//    CameraComponent(const GLfloat val):zoom(val){};
//    operator GLfloat&() {return zoom;};
//    operator const GLfloat&() const {return zoom;}
//};

struct CameraComp
{
    Camera camera;

    CameraComp(const Camera val): camera(val){};
    CameraComp(const GLfloat aNear=0.3f, const GLfloat aFar=1000.0f,
               const GLfloat aFov=45.0f, const GLfloat aAspect=0.66f):
               camera(aNear, aFar, aFov, aAspect){};
    operator Camera&() {return camera;};
    operator const Camera&() const {return camera;}
};

struct DynamicShadowComp
{
    // Empty: presence of this component = "entity casts dynamic shadow"
};


//struct PerspectiveComp
//{
//    GLfloat fov    ;//= 45.0f;
//    GLfloat zNear  ;//= 0.3f;
//    GLfloat zFar   ;//= 100.0f;
//    GLfloat aspect ;//= 0.66;

//    PerspectiveComp(const GLfloat aNear=0.3f, const GLfloat aFar=1000.0f,
//                    const GLfloat aFov=45.0f, const GLfloat aAspect=0.66f):
//                    zNear(aNear), zFar(aFar), fov(aFov), aspect(aAspect){};
//};

struct ShaderComp
{
    ShaderProgram *shaderProgram;
    ShaderComp(ShaderProgram *val)
    {shaderProgram = val;};
    operator ShaderProgram&() {return *shaderProgram;};
    operator const ShaderProgram&() const {return *shaderProgram;}
};

struct LightComp
{
    GLuint lightID;
    LightComp(const GLuint aLightID):lightID(aLightID){};

    operator GLuint&() {return lightID;};
    operator const GLuint&() const {return lightID;}
};

struct MeshComp
{
    GLuint VAO;
    GLuint indicesSize;
//    GLuint verticesSize;

    MeshComp(const GLuint aVAO,// const GLuint aVBO, const GLuint aIBO,
          const GLuint aIndicesSize)://, const GLuint aVerticesSize):
            VAO(aVAO),// VBO(aVBO), IBO(aIBO),
            indicesSize(aIndicesSize)//, verticesSize(aVerticesSize)
    {};

//    operator GLuint&() {return VAO;};
//    operator const GLuint&() const {return VAO;}
};

struct TransformComp
{
    Transform transform;
    TransformComp(const Transform val): transform(val){}

    operator Transform&() {return transform;};
    operator const Transform&() const {return transform;}
};

struct ParentComponent
{
    entt::entity parent = entt::null;
    Transform* parentTransform = nullptr;
};

struct ChildrenComponent
{
    std::vector<entt::entity> children;
};

inline bool AttachParent(entt::registry& registry, entt::entity child, entt::entity parent) {
    if (!registry.valid(child) || !registry.valid(parent)) return false;
    if (!registry.all_of<TransformComp>(parent)) return false;

    auto& parentTransform = registry.get<TransformComp>(parent).transform;

    registry.emplace_or_replace<ParentComponent>(child, parent, &parentTransform);

    auto& children = registry.get_or_emplace<ChildrenComponent>(parent).children;
    children.push_back(child);

    return true;
}

inline void DetachParent(entt::registry& registry, entt::entity child) {
    if (!registry.all_of<ParentComponent>(child)) return;

    auto& parentComp = registry.get<ParentComponent>(child);
    if (registry.valid(parentComp.parent) && registry.all_of<ChildrenComponent>(parentComp.parent)) {
        auto& children = registry.get<ChildrenComponent>(parentComp.parent).children;
        children.erase(std::remove(children.begin(), children.end(), child), children.end());
    }

    registry.remove<ParentComponent>(child);
}




//struct TransformComponent
//{
//    GLMTransform transform;
//    TransformComponent(const GLMTransform val):transform(val){};
//    TransformComponent(const Transform val)
//    {
//        transform.setTransform( glm::mat4(1));
//        transform.setTransform( val.getTransformMatrix());
//    };

//    operator GLMTransform&() {return transform;};
//    operator const GLMTransform&() const {return transform;}
//};
//struct ShaderComp
//{
//    ShaderProgram shaderProgram;

//    ShaderComp(ShaderProgram &val):
//        shaderProgram(val){};

//    ShaderComp(const std::string &val):
//        shaderProgram(val){};
//    ShaderComp(const std::string &vsName, const std::string &fsName)
//        :shaderProgram(vsName, fsName){};

//    operator ShaderProgram&() { return shaderProgram; }
//    operator const ShaderProgram&() const {return shaderProgram;}
//};

//struct UniformPBRComp
//{

//    GLuint model;
//    GLuint view;
//    GLuint projection;
//    GLuint lightSpaceMatrix;
//    UniformPBRComp( GLuint aCamPosUniform, GLuint aLightSpaceMatrix,
//                    GLuint aProjection, GLuint aView, GLuint aVodelUniform):
//                    model(aCamPosUniform), view(aLightSpaceMatrix),
//                    projection(aProjection), lightSpaceMatrix(aView) {}
//};

//struct ShaderPBRComp:public ShaderComp
//{
//    ShaderPBRComp(const std::string &val)
//        :ShaderComp(val){};
//    GLint camPosUniform;
//    GLint modelUniform;
//    GLint lightSpaceMatrixUniform;
//    GLint viewUniform;
//};

struct MaterialPBRComp
{
    GLuint albedoId;
    GLuint normalId;
    GLuint metallicId;
    GLuint roughnessId;
    GLuint aoId;
    MaterialPBRComp(const GLuint aAlbedoId,
                    const GLuint aNormalID,
                    const GLuint aMetallicId,
                    const GLuint aRoughnessId,
                    const GLuint aAoId)
        :albedoId(aAlbedoId), normalId(aNormalID), metallicId(aMetallicId), roughnessId(aRoughnessId), aoId(aAoId)
    {};
};

struct mvpComp
{
public:

    GLuint model;
    GLuint view;
    GLuint projection;

    mvpComp(ShaderProgram &shaderProgram)// const std::string &shaderName): ShaderProgram(shaderName)
    { initUniforms(shaderProgram); };

    mvpComp(ShaderProgram *shaderProgram)// const std::string &shaderName): ShaderProgram(shaderName)
    { initUniforms(*shaderProgram); };

    void initUniforms(ShaderProgram &shaderProgram)
    {
        model=shaderProgram.getUniform("model");
        view=shaderProgram.getUniform("view");
        projection=shaderProgram.getUniform("projection");

    };
};


struct MaterialAlbedoNormalComp

{
    GLuint albedoId;
    GLuint normalId;
    //GLuint metalId;
    MaterialAlbedoNormalComp(const GLuint aAlbedoId, const GLuint aNormalID)
        :albedoId(aAlbedoId), normalId(aNormalID){};
};

struct CubeMapComp
{
    GLuint cubeTextureId;
    CubeMapComp(const GLuint aBoxTextureId)
        :cubeTextureId(aBoxTextureId){};

    operator GLuint&() {return cubeTextureId;};
    operator const GLuint&() const {return cubeTextureId;}
};

struct AlbedoTextureComponent
{
    GLuint textureId;
    AlbedoTextureComponent(const GLuint val):textureId(val){};

    operator GLuint&() {return textureId;};
    operator const GLuint&() const {return textureId;}
};

struct NormalTextureComponent
{
    GLuint textureId;
    NormalTextureComponent(const GLuint val):textureId(val){};

    operator GLuint&() {return textureId;};
    operator const GLuint&() const {return textureId;}
};

struct Plan {
    btVector3 normal;
    float distance;

    Plan() = default;

    Plan(const btVector3& normal, float distance)
        : normal(normal), distance(distance) {}

    // Plane from 4 coefficients
    Plan(float a, float b, float c, float d)
    {
        normal = btVector3(a, b, c);
        float length = normal.length();
        normal /= length;
        distance = d / length;
    }

    float getSignedDistanceToPlan(const btVector3& point) const {
        return normal.dot(point) + distance;
    }
};


struct Frustum
{
    Plan topFace;
    Plan bottomFace;

    Plan rightFace;
    Plan leftFace;

    Plan farFace;
    Plan nearFace;
};

// Frustum extractFrustumPlanes(const glm::mat4& viewProj)
// {
//     Frustum frustum;

//     // Row-major access to matrix elements
//     const float* m = glm::value_ptr(viewProj);

//     frustum.leftFace   = Plan(m[3]  + m[0], m[7]  + m[4], m[11] + m[8],  m[15] + m[12]);
//     frustum.rightFace  = Plan(m[3]  - m[0], m[7]  - m[4], m[11] - m[8],  m[15] - m[12]);
//     frustum.bottomFace = Plan(m[3]  + m[1], m[7]  + m[5], m[11] + m[9],  m[15] + m[13]);
//     frustum.topFace    = Plan(m[3]  - m[1], m[7]  - m[5], m[11] - m[9],  m[15] - m[13]);
//     frustum.nearFace   = Plan(m[3]  + m[2], m[7]  + m[6], m[11] + m[10], m[15] + m[14]);
//     frustum.farFace    = Plan(m[3]  - m[2], m[7]  - m[6], m[11] - m[10], m[15] - m[14]);

//     return frustum;
// };


struct FPSEulerComponent
{
    btVector3 euler;
    FPSEulerComponent(const btVector3 val):euler(val){};

    operator btVector3&() {return euler;};
    operator const btVector3&() const {return euler;}
};

struct FixSphereBVComp
{
    float radius;

    FixSphereBVComp(const float inRadius=2.5f)
        :radius(inRadius)
    {}

    bool isOnOrForwardPlan(const btVector3 position, const Plan& plan) const
    {
        return plan.getSignedDistanceToPlan(position) > -radius;
    }

    bool isOnFrustum(const btVector3 position, const Frustum& camFrustum) const
    {
        //Check Firstly the result that have the most chance to faillure to avoid to call all functions.
        return (this->isOnOrForwardPlan(position, camFrustum.nearFace));/*  &&
                this->isOnOrForwardPlan(position, camFrustum.leftFace)  &&
                this->isOnOrForwardPlan(position, camFrustum.rightFace) &&
                this->isOnOrForwardPlan(position, camFrustum.farFace)   &&
                this->isOnOrForwardPlan(position, camFrustum.topFace)   &&
                this->isOnOrForwardPlan(position, camFrustum.bottomFace));*/
    };


//    operator FixSphereBVComp*() {return *this;};
//    operator const FixSphereBVComp*() const {return *this;}
};

//struct BoundingVolume
//{
//    virtual bool isOnFrustum(const Frustum& camFrustum, const Transform& transform) const = 0;

//    virtual bool isOnOrForwardPlan(const Plan& plan) const = 0;

//    bool isOnFrustum(const Frustum& camFrustum) const
//    {
//        return (isOnOrForwardPlan(camFrustum.leftFace) &&
//            isOnOrForwardPlan(camFrustum.rightFace) &&
//            isOnOrForwardPlan(camFrustum.topFace) &&
//            isOnOrForwardPlan(camFrustum.bottomFace) &&
//            isOnOrForwardPlan(camFrustum.nearFace) &&
//            isOnOrForwardPlan(camFrustum.farFace));
//    };
//};

//struct Sphere : public BoundingVolume
//{
//    btVector3 center{ 0.f, 0.f, 0.f };
//    float radius{ 0.f };

//    Sphere(const btVector3& inCenter, float inRadius)
//        : BoundingVolume{}, center{ inCenter }, radius{ inRadius }
//    {}

//    bool isOnOrForwardPlan(const Plan& plan) const final
//    {
//        return plan.getSignedDistanceToPlan(center) > -radius;
//    }

//    bool isOnFrustum(const Frustum& camFrustum, const Transform& transform) const final
//    {
//        //Get global scale thanks to our transform
//        const glm::vec3 globalScale = transform.getGlobalScale();

//        //Get our global center with process it with the global model matrix of our transform
//        const glm::vec3 globalCenter{ transform.getModelMatrix() * glm::vec4(center, 1.f) };

//        //To wrap correctly our shape, we need the maximum scale scalar.
//        const float maxScale = std::max(std::max(globalScale.x, globalScale.y), globalScale.z);

//        //Max scale is assuming for the diameter. So, we need the half to apply it to our radius
//        Sphere globalSphere(globalCenter, radius * (maxScale * 0.5f));

//        //Check Firstly the result that have the most chance to faillure to avoid to call all functions.
//        return (globalSphere.isOnOrForwardPlan(camFrustum.leftFace) &&
//            globalSphere.isOnOrForwardPlan(camFrustum.rightFace) &&
//            globalSphere.isOnOrForwardPlan(camFrustum.farFace) &&
//            globalSphere.isOnOrForwardPlan(camFrustum.nearFace) &&
//            globalSphere.isOnOrForwardPlan(camFrustum.topFace) &&
//            globalSphere.isOnOrForwardPlan(camFrustum.bottomFace));
//    };
//};

//struct SquareAABB : public BoundingVolume
//{
//    glm::vec3 center{ 0.f, 0.f, 0.f };
//    float extent{ 0.f };

//    SquareAABB(const glm::vec3& inCenter, float inExtent)
//        : BoundingVolume{}, center{ inCenter }, extent{ inExtent }
//    {}

//    bool isOnOrForwardPlan(const Plan& plan) const final
//    {
//        // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
//        const float r = extent * (std::abs(plan.normal.x) + std::abs(plan.normal.y) + std::abs(plan.normal.z));
//        return -r <= plan.getSignedDistanceToPlan(center);
//    }

//    bool isOnFrustum(const Frustum& camFrustum, const Transform& transform) const final
//    {
//        //Get global scale thanks to our transform
//        const glm::vec3 globalCenter{ transform.getModelMatrix() * glm::vec4(center, 1.f) };

//        // Scaled orientation
//        const glm::vec3 right = transform.getRight() * extent;
//        const glm::vec3 up = transform.getUp() * extent;
//        const glm::vec3 forward = transform.getForward() * extent;

//        const float newIi = std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, right)) +
//            std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, up)) +
//            std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, forward));

//        const float newIj = std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, right)) +
//            std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, up)) +
//            std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, forward));

//        const float newIk = std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, right)) +
//            std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, up)) +
//            std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, forward));

//        const SquareAABB globalAABB(globalCenter, std::max(std::max(newIi, newIj), newIk));

//        return (globalAABB.isOnOrForwardPlan(camFrustum.leftFace) &&
//            globalAABB.isOnOrForwardPlan(camFrustum.rightFace) &&
//            globalAABB.isOnOrForwardPlan(camFrustum.topFace) &&
//            globalAABB.isOnOrForwardPlan(camFrustum.bottomFace) &&
//            globalAABB.isOnOrForwardPlan(camFrustum.nearFace) &&
//            globalAABB.isOnOrForwardPlan(camFrustum.farFace));
//    };
//};

//struct AABB : public BoundingVolume
//{
//    glm::vec3 center{ 0.f, 0.f, 0.f };
//    glm::vec3 extents{ 0.f, 0.f, 0.f };

//    AABB(const glm::vec3& min, const glm::vec3& max)
//        : BoundingVolume{}, center{ (max + min) * 0.5f }, extents{ max.x - center.x, max.y - center.y, max.z - center.z }
//    {}

//    AABB(const glm::vec3& inCenter, float iI, float iJ, float iK)
//        : BoundingVolume{}, center{ inCenter }, extents{ iI, iJ, iK }
//    {}

//    std::array<glm::vec3, 8> getVertice() const
//    {
//        std::array<glm::vec3, 8> vertice;
//        vertice[0] = { center.x - extents.x, center.y - extents.y, center.z - extents.z };
//        vertice[1] = { center.x + extents.x, center.y - extents.y, center.z - extents.z };
//        vertice[2] = { center.x - extents.x, center.y + extents.y, center.z - extents.z };
//        vertice[3] = { center.x + extents.x, center.y + extents.y, center.z - extents.z };
//        vertice[4] = { center.x - extents.x, center.y - extents.y, center.z + extents.z };
//        vertice[5] = { center.x + extents.x, center.y - extents.y, center.z + extents.z };
//        vertice[6] = { center.x - extents.x, center.y + extents.y, center.z + extents.z };
//        vertice[7] = { center.x + extents.x, center.y + extents.y, center.z + extents.z };
//        return vertice;
//    }

//    //see https://gdbooks.gitbooks.io/3dcollisions/content/Chapter2/static_aabb_plan.html
//    bool isOnOrForwardPlan(const Plan& plan) const final
//    {
//        // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
//        const float r = extents.x * std::abs(plan.normal.x) + extents.y * std::abs(plan.normal.y) +
//            extents.z * std::abs(plan.normal.z);

//        return -r <= plan.getSignedDistanceToPlan(center);
//    }

//    bool isOnFrustum(const Frustum& camFrustum, const Transform& transform) const final
//    {
//        //Get global scale thanks to our transform
//        const glm::vec3 globalCenter{ transform.getModelMatrix() * glm::vec4(center, 1.f) };

//        // Scaled orientation
//        const glm::vec3 right = transform.getRight() * extents.x;
//        const glm::vec3 up = transform.getUp() * extents.y;
//        const glm::vec3 forward = transform.getForward() * extents.z;

//        const float newIi = std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, right)) +
//            std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, up)) +
//            std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, forward));

//        const float newIj = std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, right)) +
//            std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, up)) +
//            std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, forward));

//        const float newIk = std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, right)) +
//            std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, up)) +
//            std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, forward));

//        const AABB globalAABB(globalCenter, newIi, newIj, newIk);

//        return (globalAABB.isOnOrForwardPlan(camFrustum.leftFace) &&
//            globalAABB.isOnOrForwardPlan(camFrustum.rightFace) &&
//            globalAABB.isOnOrForwardPlan(camFrustum.topFace) &&
//            globalAABB.isOnOrForwardPlan(camFrustum.bottomFace) &&
//            globalAABB.isOnOrForwardPlan(camFrustum.nearFace) &&
//            globalAABB.isOnOrForwardPlan(camFrustum.farFace));
//    };
//};

//Frustum createFrustumFromCamera(const Transform& cam, float near, float far)
//{
//    frustum frustum;
//    const float halfVSide = zFar * tanf(fov * .5f);
//    const float halfHSide = halfVSide * aspect;
//    const btVector3 frontMultFar = zFar * cam.forward();

//    frustum.nearFace = { cam.getPosition() + zNear * cam.forward(), cam.forward() };
//    frustum.farFace = { cam.getPosition() + frontMultFar, -cam.forward() };
//    frustum.rightFace = { cam.getPosition(),
//                            btCross(cam.up(),frontMultFar + cam.right() * halfHSide) };
//    frustum.leftFace = { cam.getPosition(),
//                            btCross(frontMultFar - cam.right() * halfHSide, cam.up()) };
//    frustum.topFace = { cam.getPosition(),
//                            btCross(cam.right(), frontMultFar - cam.up() * halfVSide) };
//    frustum.bottomFace =
//        { cam.getPosition(), btCross(frontMultFar + cam.up() * halfVSide, cam.right()) };

//    return frustum;
//}

//AABB generateAABB(const Model& model)
//{
//    glm::vec3 minAABB = glm::vec3(std::numeric_limits<float>::max());
//    glm::vec3 maxAABB = glm::vec3(std::numeric_limits<float>::min());
//    for (auto&& mesh : model.meshes)
//    {
//        for (auto&& vertex : mesh.vertices)
//        {
//            minAABB.x = std::min(minAABB.x, vertex.Position.x);
//            minAABB.y = std::min(minAABB.y, vertex.Position.y);
//            minAABB.z = std::min(minAABB.z, vertex.Position.z);

//            maxAABB.x = std::max(maxAABB.x, vertex.Position.x);
//            maxAABB.y = std::max(maxAABB.y, vertex.Position.y);
//            maxAABB.z = std::max(maxAABB.z, vertex.Position.z);
//        }
//    }
//    return AABB(minAABB, maxAABB);
//}

//Sphere generateSphereBV(const Model& model)
//{
//    glm::vec3 minAABB = glm::vec3(std::numeric_limits<float>::max());
//    glm::vec3 maxAABB = glm::vec3(std::numeric_limits<float>::min());
//    for (auto&& mesh : model.meshes)
//    {
//        for (auto&& vertex : mesh.vertices)
//        {
//            minAABB.x = std::min(minAABB.x, vertex.Position.x);
//            minAABB.y = std::min(minAABB.y, vertex.Position.y);
//            minAABB.z = std::min(minAABB.z, vertex.Position.z);

//            maxAABB.x = std::max(maxAABB.x, vertex.Position.x);
//            maxAABB.y = std::max(maxAABB.y, vertex.Position.y);
//            maxAABB.z = std::max(maxAABB.z, vertex.Position.z);
//        }
//    }

//    return Sphere((maxAABB + minAABB) * 0.5f, glm::length(minAABB - maxAABB));
//}


#endif // ECSCOMPONENTS_H


