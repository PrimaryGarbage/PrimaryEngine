#include "image.hpp"
#include <filesystem>
#include <cassert>
#include <unordered_map>
#include "stb_image.h"
#include "../prim_exception.hpp"

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
            default:
                return 4;
        }
    }
    
    ImageType Image::parseType(std::string path)
    {
        const static std::unordered_map<std::string, int> extensionMap = {
            { ".jpeg", 0 },
            { ".jpg", 1 },
            { ".png", 2 },
        };

        assert(std::filesystem::exists(std::filesystem::path(path)));
        std::filesystem::path filePath(path);
        std::string extension = filePath.extension().string(); 
        switch(extensionMap.at(extension))
        {
            case 0:
            case 1:
                return ImageType::jpeg;
            case 2:
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
    
    void Image::load(std::string filePath)
    {
        unload();
        assert(std::filesystem::exists(filePath));
        stbi_set_flip_vertically_on_load(1);
        data = stbi_load(filePath.c_str(), &width, &height, &channelCount, getChannelCountOfType(parseType(filePath)));
        size = width * height * channelCount; 
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
        }
    }
}