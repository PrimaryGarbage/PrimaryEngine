#include "texture.hpp"
#include "renderer.hpp"
#include "stb_image.h"


namespace prim
{

    Texture::Texture(const std::string path): filePath(path), width(0), height(0), bitsPerPixel(0)
    {
        stbi_set_flip_vertically_on_load(1);
        unsigned char* localBuffer = stbi_load(path.c_str(), &width, &height, &bitsPerPixel, 4);

        GL_CALL(glGenTextures(1, &gl_id));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, gl_id));

        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

        GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

        if(localBuffer)
        {
            stbi_image_free(localBuffer);
            localBuffer = nullptr;
        }
    }
    
    Texture::Texture(Texture&& other):
        gl_id(other.gl_id), filePath(std::move(other.filePath)),
        width(other.width), height(other.height), bitsPerPixel(other.bitsPerPixel) 
    {
        other.gl_id = 0;
        other.filePath = "";
        other.width = other.height = 0;
    }
    
    Texture& Texture::operator=(Texture&& other)
    {
        destroy();

        gl_id = other.gl_id;
        filePath = other.filePath;
        width = other.width;
        height = other.height;
        bitsPerPixel = other.bitsPerPixel;

        other.gl_id = 0;
        other.filePath = "";
        other.width = other.height = 0;
        other.bitsPerPixel = 0;

        return *this;
    }
    
    Texture::~Texture()
    {
        destroy();
    }
    
    void Texture::load(std::string filePath)
    {
        destroy();

        this->filePath = filePath;
        
        stbi_set_flip_vertically_on_load(1);
        unsigned char* localBuffer = stbi_load(filePath.c_str(), &width, &height, &bitsPerPixel, 4);

        GL_CALL(glGenTextures(1, &gl_id));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, gl_id));

        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

        GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

        if(localBuffer)
        {
            stbi_image_free(localBuffer);
            localBuffer = nullptr;
        }

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
    
    void Texture::destroy()
    {
        if(gl_id > 0)
        {
            unbind();
            GL_CALL(glDeleteTextures(1, &gl_id));
        }
    }

}