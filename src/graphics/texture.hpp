#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__

#include <string>
#include <unordered_map>
#include <vector>
#include "typedefs.hpp"

namespace prim
{

class Image;
enum class ImageType;

class Texture
{
private:
    inline static const unsigned int maxTextureSlots = 48u;
    inline static unsigned int textureMap[maxTextureSlots] {0u};
    inline static unsigned int boundTextureSlot = maxTextureSlots + 1;
    inline static std::unordered_map<std::string, Unp<Texture>> textureCache;
    inline static std::vector<Unp<Texture>> modifiedImageTextureCache;
    inline static Texture* defaultTexture = nullptr;

    unsigned int gl_id = 0u;
    int width;
    int height;
    int channelCount;

    void loadIntoGpu(const unsigned char* data, int widht, int height, ImageType type);
    void unload();

    Texture(const std::string path);
    Texture(const Image& image);
    Texture(const unsigned char* imageData, unsigned int length, ImageType type);
    Texture(const unsigned char* data, unsigned int width, unsigned int height, ImageType type);
public:
    ~Texture();

    static Texture* create(std::string resPath);
    static Texture* create(const Image& image);
    static Texture* create(const unsigned char* imageData, unsigned int length, ImageType type);
    static Texture* create(const unsigned char* data, unsigned int width, unsigned int height, ImageType type);
    static Texture* getDefaultTexture();
    static void terminate();

    void bind(unsigned int slot = 0) const;
    void unbind() const;

    inline int getWidth() const { return width; }
    inline int getHeight() const { return height; }
};


}


#endif // __TEXTURE_HPP__