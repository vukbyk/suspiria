#pragma once

#include <QtOpenGL/QOpenGLWindow>
#include <QOpenGLExtraFunctions>
#include <glwindow.h>

class Suspiria: protected QOpenGLExtraFunctions
{
//    Q_OBJECT
    GLWindow glWindow;
public:
    Suspiria();
};
