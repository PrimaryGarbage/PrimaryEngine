#ifndef __FONT_HPP__
#define __FONT_HPP__

#include <string>
#include "ft2build.h"
#include "typedefs.hpp"
#include "texture.hpp"
#include "glm.hpp"
#include FT_FREETYPE_H

namespace prim
{

    // all numeric values here are relative (0.0 <= x <= 1.0)
    struct Glyph
    {
        glm::vec2 size;
        glm::vec2 offset;
        float advanceX;
        Texture* texture;
    };

    class Font
    {
    private:
        static const unsigned int defaultEmSize = 3000u;

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
        void setEmSize(int size);
        int getEmSize() const;
        const Glyph* getGlyph(unsigned char ch) const;
        float calculateWidth(const std::string& str) const;
    };
} // namespace prim


#endif // __FONT_HPP__