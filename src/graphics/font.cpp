#include "font.hpp"
#include "prim_exception.hpp"
#include "resource_manager.hpp"
#include "image_utils.hpp"
#include "globals.hpp"
#include "default_font_data.hpp"

#define FT_CHECK_ERROR(error) if(error) throw PRIM_EXCEPTION("Freetype raised an exception: error code " + std::to_string(error))

namespace prim
{
    Font::Font() 
    {
        FT_CHECK_ERROR(FT_Init_FreeType(&ftLibrary));
        FT_CHECK_ERROR(FT_New_Memory_Face(ftLibrary, defaultFontData, defaultFontDataLength, 0, &ftFace));
        setEmSize(defaultEmSize);
        generateTextures();
        Globals::logger->logInfo("Loaded default font. //TODO: Need to fix this multiple font loading");
    }
    
    Font::Font(std::string filePath) 
    {
        FT_CHECK_ERROR(FT_Init_FreeType(&ftLibrary));
        setEmSize(defaultEmSize);
        load(std::move(filePath));
    }
    
    Font::~Font() 
    {
        FT_Done_Face(ftFace);
        FT_Done_FreeType(ftLibrary);
    }

    void Font::generateTextures() 
    {
        textureMap.clear();
        float emSizeFloat = static_cast<float>(getEmSize());

        for(unsigned char ch = 0; ch < 126; ++ch)
        {
            FT_GlyphSlot glyph = renderGlyph(ch);   
            Texture* texture = Texture::create(glyph->bitmap.buffer, glyph->bitmap.width, glyph->bitmap.rows, ImageType::bitmap);
            textureMap[ch] = Glyph { 
                glm::vec2(glyph->metrics.width >> 6, glyph->metrics.height >> 6), // pxSize
                glm::vec2(glyph->metrics.width, glyph->metrics.height) / emSizeFloat, // emSize
                glm::vec2(glyph->metrics.horiBearingX, glyph->metrics.horiBearingY - glyph->metrics.height) / emSizeFloat, // emOffset
                glyph->advance.x / emSizeFloat, // emAdvance
                static_cast<float>(glyph->advance.x >> 6), // pxAdvance
                static_cast<float>(glyph->metrics.horiBearingY >> 6), // pxAscend
                static_cast<float>((glyph->metrics.height - glyph->metrics.horiBearingY) >> 6), // pxDescend
                texture }; // texture
        }
    }
    
    void Font::load(std::string filePath) 
    {
        FT_CHECK_ERROR(FT_New_Face(ftLibrary, ResourceManager::createResourcePath(filePath).c_str(), 0, &ftFace));
        generateTextures();
    }
    
    void Font::load(uchar* data, uint size) 
    {
        FT_CHECK_ERROR(FT_New_Memory_Face(ftLibrary, data, size, 0, &ftFace));
        generateTextures();
    }
    
    void Font::setEmSize(int size) 
    {
        FT_CHECK_ERROR(FT_Set_Pixel_Sizes(ftFace, 0, size));
        generateTextures();
    }
    
    int Font::getEmSize() const
    {
        return ftFace->size->metrics.height;
    }

    int Font::getEmSizeInPixels() const
    {
        return ftFace->size->metrics.height >> 6;
    }
    
    const Glyph* Font::getGlyph(unsigned char ch) const
    {
        if(textureMap.count(ch) == 0) 
            throw PRIM_EXCEPTION("Trying to access font glyph that doesn't exist!");

        return &textureMap.at(ch);
    }
    
    StringFontInfo Font::calculateStringInfo(const std::string& str) const
    {
        StringFontInfo info;
        int emSize = getEmSizeInPixels();

        for(const char& ch : str)
        {
            const Glyph* glyph = getGlyph(ch);

            info.pxSize.x += glyph->pxAdvanceX;

            if(info.pxSize.y < glyph->pxSize.y) 
                info.pxSize.y = glyph->pxSize.y;

            if(info.pxMaxAscend < glyph->pxAscend)
                info.pxMaxAscend = glyph->pxAscend;

            if(info.pxMaxDescend < glyph->pxDescend)
                info.pxMaxDescend = glyph->pxDescend;
        }

        info.emSize.x = info.pxSize.x / emSize;
        info.emSize.y = info.pxSize.y / emSize;
        info.emMaxAscend = info.pxMaxAscend / emSize;
        info.emMaxDescend = info.pxMaxDescend / emSize;

        return info;
    }
    
    FT_GlyphSlot Font::renderGlyph(char ch) 
    {
        FT_CHECK_ERROR(FT_Load_Char(ftFace, ch, FT_LOAD_RENDER));
        return ftFace->glyph;
    }
}