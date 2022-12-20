#include "texture.hpp"
#include "renderer.hpp"
#include "image.hpp"
#include "globals.hpp"
#include "default_texture_data.hpp"


namespace prim
{

    Texture::Texture(const std::string path)
    {
        unload();
        Image image(path);
        loadIntoGpu(image.getData(), image.getWidth(), image.getHeight(), image.getType());
        width = image.getWidth();
        height = image.getHeight();
        channelCount = image.getChannelCount();
    }

    Texture::Texture(const Image& image)
    {
        unload();
        loadIntoGpu(image.getData(), image.getWidth(), image.getHeight(), image.getType());
        width = image.getWidth();
        height = image.getHeight();
        channelCount = image.getChannelCount();
    }
    
    Texture::Texture(const unsigned char* data, unsigned int width, unsigned int height, ImageType type) 
    {
        unload();
        loadIntoGpu(data, width, height, type);
        this->width = width;
        this->height = height;
        channelCount = type == ImageType::png ? 4 : 3;
    }
    
    void Texture::loadIntoGpu(const unsigned char* data, int width, int height, ImageType type)
    {
        GL_CALL(glGenTextures(1, &gl_id));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, gl_id));

        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
        GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, static_cast<unsigned int>(type), width, height, 0, static_cast<unsigned int>(type), GL_UNSIGNED_BYTE, data));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));       
    }

    
    Texture::~Texture()
    {
        unload();
    }
    
    Texture* Texture::create(std::string resPath) 
    {
        auto it = textureCache.find(resPath);    
        if(it == textureCache.end())
        {
            Texture* texture = new Texture(resPath);
            textureCache[resPath] = texture;
            Globals::logger->logInfo("Texture loaded. Path: " + resPath);
            return texture;
        }
        
        return it->second;
    }
    
    Texture* Texture::create(const Image& image) 
    {
        if(image.wasModified())
        {
            Texture* texture = new Texture(image);
            modifiedImageTextureCache.push_back(texture);
            Globals::logger->logInfo("Texture loaded from modified image");
            return texture;
        }
        else
        {
            auto it = textureCache.find(image.getFilePath());    
            if(it == textureCache.end())
            {
                std::string imageFilePath = image.getFilePath();
                Texture* texture = new Texture(imageFilePath);
                Globals::logger->logInfo("Texture loaded. Path: " + imageFilePath);
                textureCache[imageFilePath] = texture;
                return texture;
            }

            return it->second;
        }
    }
    
    Texture* Texture::create(const unsigned char* data, unsigned int width, unsigned int height, ImageType type) 
    {
        Texture* texture = new Texture(data, width, height, type);
        modifiedImageTextureCache.push_back(texture);
        return texture;
    }
    
    Texture* Texture::getDefaultTexture() 
    {
        if(!defaultTexture)
            defaultTexture = Texture::create(defaultTextureData, defaultTextureDataWidth, defaultTextureDataHeight, ImageType::png);
        return defaultTexture;
    }
    
    void Texture::terminate() 
    {
        for(auto& pair : textureCache)
            delete pair.second;

        for(auto& texture : modifiedImageTextureCache)
            delete texture;

        textureCache.clear();
    }
    
    void Texture::bind(unsigned int slot) const
    {
        if(boundTextureSlot != slot)
        {
            GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
            boundTextureSlot = slot;
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
        textureMap[boundTextureSlot] = 0u;
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