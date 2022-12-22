#ifndef __IMAGE_HPP__
#define __IMAGE_HPP__

#include <string>
#include "image_utils.hpp"

namespace prim
{

    class Image
    {
    private:
        unsigned char* data = nullptr;
        std::string filePath;
        int width;
        int height;
        int channelCount;
        unsigned int size;
        ImageType type;
        bool modified = false;  // set this variable after any image modification (e.g. setPixel())

        static ImageType parseType(std::string path);

    public:
        Image() = default;
        Image(std::string filePath);
        Image(const unsigned char* data, unsigned int dataLength, ImageType type);
        Image(const Image& other);
        Image(Image&& other);
        Image& operator=(const Image& other);
        Image& operator=(Image&& other);
        ~Image();

        void load(std::string resPath);
        void load(const unsigned char* data, unsigned int dataLength, ImageType type);
        void unload();

        inline unsigned char* getData() const { return data; }
        inline int getWidth() const { return width; }
        inline int getHeight() const { return height; }
        inline int getChannelCount() const { return channelCount; }
        inline unsigned int getSize() const { return size; }
        inline std::string getFilePath() const { return filePath; }
        inline ImageType getType() const { return type; }
        inline bool empty() const { return !data; }
        inline bool wasModified() const { return modified; }

        static int getChannelCountOfType(ImageType type);
    };

}


#endif // __IMAGE_HPP__