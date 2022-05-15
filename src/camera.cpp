#include "camera.h"

Camera::Camera(const GLfloat aNear, const GLfloat aFar=1000.0f,
               const GLfloat aFov=45.0f, const GLfloat aAspect=0.66f):
              zNear(aNear), zFar(aFar), fov(aFov), aspect(aAspect){};


//Frustum Camera::createFrustumFromCamera(const Transform& cam, float nearOffset, float farOffset)
//{
//    Frustum frustum;
////    Camera &camPersp = world->reg()->get<CameraComp>(camera);
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

//glm::mat4 Camera::getViewMatrix(void) const
//{
////    return glm::inverse(getCameraTransformMatrix());
//    return glm::toMat4( getTransform().getRotation()) * glm::translate(getPosition());// * glm::scale(scale);
//}

//Transform &Camera::moveForward(const float f)
//{
//    return translate(forward() * f );
//}

//glm::vec3 Camera::getForward() const
//{
//    glm::vec3 dir = glm::rotate(glm::inverse(rotation), glm::vec3(0.0, 0.0, -1.0));
//    return dir;
//}
