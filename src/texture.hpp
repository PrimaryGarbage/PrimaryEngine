#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__

#include <string>

namespace prim
{

class Texture
{
private:
    unsigned int gl_id;
    std::string filePath;
    unsigned char* localBuffer;
    int width;
    int height;
    int bitsPerPixel;
public:
    Texture(const std::string path);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unbind() const;

    inline int getWidth() const { return width; }
    inline int getHeight() const { return height; }
};


}


#endif // __TEXTURE_HPP__