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

        static int getChannelCountOfType(ImageType type);
        static ImageType parseType(std::string path);

    public:
        Image() = default;
        Image(std::string filePath);
        Image(const Image& other);
        Image(Image&& other);
        Image& operator=(const Image& other);
        Image& operator=(Image&& other);
        ~Image();

        void load(std::string filePath);
        void unload();

        inline unsigned char* getData() const { return data; }
        inline int getWidth() const { return width; }
        inline int getHeight() const { return height; }
        inline int getChannelCount() const { return channelCount; }
        inline unsigned int getSize() const { return size; }
        inline std::string getFilePath() const { return filePath; }
        inline ImageType getType() const { return type; }
        inline bool empty() const { return !data; }
    };

}


#endif // __IMAGE_HPP__