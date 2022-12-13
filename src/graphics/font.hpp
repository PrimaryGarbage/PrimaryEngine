#ifndef __FONT_HPP__
#define __FONT_HPP__

#include <string>
#include "ft2build.h"
#include "typedefs.hpp"
#include FT_FREETYPE_H

typedef FT_GlyphSlot Glyph;

namespace prim
{
    class Font
    {
    private:
        FT_Library ftLibrary = NULL;
        FT_Face ftFace = NULL;
    public:
        Font();
        Font(std::string filename);
        ~Font();

        void load(std::string filename);
        void load(uchar* data, uint size);
        void setSize(float size);
        float getSize() const;
        Glyph renderGlyph(char ch);
    };
} // namespace prim


#endif // __FONT_HPP__