#include "image.hpp"
#include <filesystem>
#include <cassert>
#include <unordered_map>
#include "stb_image.h"
#include "prim_exception.hpp"
#include "typedefs.hpp"
#include "resource_manager.hpp"

namespace prim
{
    int Image::getChannelCountOfType(ImageType type)
    {
        switch(type)
        {
            case ImageType::jpeg:
                return 3;
            case ImageType::png:
                return 4;
            case ImageType::bitmap:
                return 1;
            default:
                return 4;
        }
    }
    
    ImageType Image::parseType(std::string path)
    {
        const static std::unordered_map<std::string, int> extensionMap = {
            { ".jpeg", static_cast<int>(ImageType::jpeg) },
            { ".jpg", static_cast<int>(ImageType::jpeg) },
            { ".png", static_cast<int>(ImageType::png) },
        };

        assert(std::filesystem::exists(std::filesystem::path(path)));
        std::filesystem::path filePath(path);
        std::string extension = filePath.extension().string(); 
        switch(extensionMap.at(extension))
        {
            case static_cast<int>(ImageType::jpeg):
                return ImageType::jpeg;
            case static_cast<int>(ImageType::png):
                return ImageType::png;
            default:
                return ImageType::unknown;
        }
    }

    Image::Image(std::string filePath): filePath(filePath)
    {
        load(filePath);
    }
    
    Image::Image(const Image& other)
        : filePath(other.filePath), width(other.width), height(other.height), channelCount(other.channelCount), size(other.size)
    {
        std::copy(other.data, other.data + other.size - 1, data);
    }
    
    Image::Image(Image&& other)
        : filePath(other.filePath), width(other.width), height(other.height), channelCount(other.channelCount), size(other.size)
    {
        other.data = nullptr;
        other.filePath = "";
        other.width = other.height = other.channelCount = 0;
        other.size = 0u;
    }
    
    Image::Image(const unsigned char* data, unsigned int dataLength, ImageType type) 
    {
        load(data, dataLength, type);
    }
    
    Image& Image::operator=(const Image& other)
    {
        filePath = other.filePath;
        width = other.width;
        height = other.height;
        channelCount = other.channelCount;
        size = other.size;
        std::copy(other.data, other.data + other.size - 1, data);
        return *this;
    }
    
    Image& Image::operator=(Image&& other)
    {
        data = other.data;
        filePath = other.filePath;
        width = other.width;
        height = other.height;
        channelCount = other.channelCount;
        size = other.size;

        other.data = nullptr;
        other.filePath = "";
        other.width = other.height = other.channelCount = 0;
        other.size = 0u;

        return *this;
    }
    
    Image::~Image()
    {
        unload();
    }
    
    void Image::load(std::string resPath)
    {
        unload();
        std::string filePath = ResourceManager::createResourcePath(resPath);
        stbi_set_flip_vertically_on_load(1);
        type = parseType(filePath);
        data = stbi_load(filePath.c_str(), &width, &height, &channelCount, getChannelCountOfType(type));
        size = width * height * channelCount; 
        this->filePath = filePath;
        modified = false;
        if(!data) throw PRIM_EXCEPTION("Couldn't load image with path '" + filePath + "'. Probably a file extension problem.");
    }
    
    void Image::load(const unsigned char* data, unsigned int dataLength, ImageType type) 
    {
        unload();
        stbi_set_flip_vertically_on_load(1);
        this->type = type;
        this->data = stbi_load_from_memory(data, dataLength, &width, &height, &channelCount, getChannelCountOfType(type));
        size = width * height * channelCount; 
        this->filePath = "";
        modified = false;
        if(!data) throw PRIM_EXCEPTION("Couldn't load image with path '" + filePath + "'. Probably a file extension problem.");
    }
    
    void Image::unload()
    {
        if(data)
        {
            stbi_image_free(data);
            filePath = "";
            width = height = channelCount = 0;
            size = 0u;
            data = nullptr;
        }
    }
}