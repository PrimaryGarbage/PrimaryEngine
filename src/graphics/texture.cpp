#include "texture.hpp"
#include "renderer.hpp"
#include "image.hpp"


namespace prim
{

    Texture::Texture(const std::string path)
    {
        load(path);
    }
    
    Texture::Texture(Texture&& other):
        gl_id(other.gl_id), width(other.width), height(other.height), channelCount(other.channelCount) 
    {
        other.gl_id = 0;
        other.width = other.height = other.channelCount = 0;
    }
    
    void Texture::loadIntoGpu(unsigned char* data, int width, int height, ImageType type)
    {
        GL_CALL(glGenTextures(1, &gl_id));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, gl_id));

        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

        GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, static_cast<unsigned int>(type), width, height, 0, static_cast<unsigned int>(type), GL_UNSIGNED_BYTE, data));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));       
    }

    Texture::Texture(const Image& image)
    {
        load(image);
    }
    
    Texture& Texture::operator=(Texture&& other)
    {
        unload();

        gl_id = other.gl_id;
        width = other.width;
        height = other.height;
        channelCount = other.channelCount;

        other.gl_id = 0;
        other.width = other.height = 0;
        other.channelCount = 0;

        return *this;
    }
    
    Texture::~Texture()
    {
        unload();
    }
    
    void Texture::load(std::string filePath)
    {
        unload();
        Image image(filePath);
        loadIntoGpu(image.getData(), image.getWidth(), image.getHeight(), image.getType());
        width = image.getWidth();
        height = image.getHeight();
        channelCount = image.getChannelCount();
    }
    
    void Texture::load(const Image& image)
    {
        unload();
        loadIntoGpu(image.getData(), image.getWidth(), image.getHeight(), image.getType());
        width = image.getWidth();
        height = image.getHeight();
        channelCount = image.getChannelCount();
    }
    
    void Texture::bind(unsigned int slot) const
    {
        if(currentTextureSlot != slot)
        {
            GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
            currentTextureSlot = slot;
        }
        if(textureMap[slot] != gl_id)
        {
            GL_CALL(glBindTexture(GL_TEXTURE_2D, gl_id));
            textureMap[slot] = gl_id;
        }
    }
    
    void Texture::unbind() const
    {
        GL_CALL(glBindTexture(GL_TEXTURE_2D, 0u));
        textureMap[currentTextureSlot] = 0u;
    }
    
    void Texture::unload()
    {
        if(gl_id > 0)
        {
            unbind();
            GL_CALL(glDeleteTextures(1, &gl_id));
            gl_id = 0;
        }
    }

}