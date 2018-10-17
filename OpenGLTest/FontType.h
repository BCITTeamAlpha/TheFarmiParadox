#pragma once

#include <string>
#include <GL/glew.h>

struct FontType {
    std::string             Name;
    int                     TexWidth, TexHeight;
    GLuint                  TextureLocation;
    std::vector<GLubyte>    TextureData;
};