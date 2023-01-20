#ifndef __IMAGE_UTILS_HPP__
#define __IMAGE_UTILS_HPP__

#include "glew/GL/glew.h"

namespace prim
{

    enum class ImageType { jpeg = GL_RGB, png = GL_RGBA, bitmap = GL_RED,unknown = 0 };

}

#endif // __IMAGE_UTILS_HPP__