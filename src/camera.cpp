#include "camera.h"

Camera::Camera(const GLfloat aNear, const GLfloat aFar=1000.0f,
               const GLfloat aFov=45.0f, const GLfloat aAspect=0.66f):
              zNear(aNear), zFar(aFar), fov(aFov), aspect(aAspect){};

