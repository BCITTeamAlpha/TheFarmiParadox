#pragma once

#include <string>
#include <GL/glew.h>

struct FontType {
    std::string Name;
    float       AspectRatio;
    GLuint      TextureLocation;
};