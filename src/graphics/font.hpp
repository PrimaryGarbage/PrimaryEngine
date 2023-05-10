#ifndef __FONT_HPP__
#define __FONT_HPP__

#include <string>
#include "ft2build.h"
#include "typedefs.hpp"
#include "texture.hpp"
#include "GLM/glm.hpp"
#include FT_FREETYPE_H

namespace prim
{

    struct Glyph
    {
        // relative (em) metrics are relative to the EM

        glm::vec2 pxSize;
        glm::vec2 emSize;
        glm::vec2 emOffset;
        float emAdvanceX;
        float pxAdvanceX;
        float pxAscend;
        float pxDescend;
        Texture* texture;
    };
    
    struct StringFontInfo
    {
        glm::vec2 pxSize;
        float pxMaxAscend;
        float pxMaxDescend;

        glm::vec2 emSize;
        float emMaxAscend;
        float emMaxDescend;
    };

    class Font
    {
    private:
        static const unsigned int defaultEmSize = 100u;

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
        int getEmSizeInPixels() const;
        const Glyph* getGlyph(unsigned char ch) const;

        StringFontInfo calculateStringInfo(const std::string& str) const;
    };
} // namespace prim


#endif // __FONT_HPP__