#include "texture.hpp"
#include "renderer.hpp"
#include "stb_image.h"


namespace prim
{
    Texture::Texture(const std::string path): filePath(path), localBuffer(nullptr), width(0), height(0), bitsPerPixel(0)
    {
        stbi_set_flip_vertically_on_load(1);
        localBuffer = stbi_load(path.c_str(), &width, &height, &bitsPerPixel, 4);

        GL_CALL(glGenTextures(1, &gl_id));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, gl_id));

        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP));

        GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

        if(localBuffer) stbi_image_free(localBuffer);
    }
    
    Texture::~Texture()
    {
        GL_CALL(glDeleteTextures(1, &gl_id));
    }
    
    void Texture::bind(unsigned int slot) const
    {
        GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, gl_id));
    }
    
    void Texture::unbind() const
    {
        GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
    }

}