#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__

#include <string>

namespace prim
{

class Image;
enum class ImageType;

class Texture
{
private:
    unsigned int gl_id;
    int width;
    int height;
    int channelCount;

    void loadIntoGpu(unsigned char* data, int widht, int height, ImageType type);

public:
    Texture() = default;
    Texture(const std::string path);
    Texture(const Image& image);
    Texture(Texture&& other);
    Texture& operator=(Texture&& other);
    ~Texture();

    void load(std::string filePath);
    void load(const Image& image);
    void bind(unsigned int slot = 0) const;
    void unbind() const;
    void unload();

    inline int getWidth() const { return width; }
    inline int getHeight() const { return height; }
};


}


#endif // __TEXTURE_HPP__