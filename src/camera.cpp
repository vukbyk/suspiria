#include "camera.h"

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
