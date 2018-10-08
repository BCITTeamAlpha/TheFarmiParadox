#include "Renderable.h"

GLuint Renderable::count = 0;

Renderable::Renderable() : _id(Renderable::count++) {}