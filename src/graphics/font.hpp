#ifndef __FONT_HPP__
#define __FONT_HPP__

#include <string>
#include "ft2build.h"
#include "typedefs.hpp"
#include "texture.hpp"
#include FT_FREETYPE_H

namespace prim
{

    struct Glyph
    {
        long width;
        long height;
        Texture* texture;

        ~Glyph() { delete texture; }
    };

    class Font
    {
    private:
        static const unsigned int defaultSize = 50u;

        FT_Library ftLibrary = NULL;
        FT_Face ftFace = NULL;
        std::unordered_map<unsigned char, Glyph> textureMap;

        void generateTextures();
        FT_GlyphSlot renderGlyph(char ch);
    public:
        Font();
        Font(std::string filename);
        ~Font();

        void load(std::string filename);
        void load(uchar* data, uint size);
        void setSize(float size);
        float getSize() const;
        const Glyph* getGlyph(unsigned char ch) const;
    };
} // namespace prim


#endif // __FONT_HPP__