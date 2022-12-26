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

        for(unsigned char ch = 32; ch < 126; ++ch)
        {
            FT_GlyphSlot glyph = renderGlyph(ch);   
            Texture* texture = Texture::create(glyph->bitmap.buffer, glyph->bitmap.width, glyph->bitmap.rows, ImageType::bitmap);
            textureMap[ch] = Glyph{ glm::vec2(glyph->metrics.width, glyph->metrics.height) / emSizeFloat, 
                glm::vec2(glyph->metrics.horiBearingX, glyph->metrics.horiBearingY - glyph->metrics.height) / emSizeFloat, 
                glyph->advance.x / emSizeFloat, 
                texture };
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
        FT_CHECK_ERROR(FT_Set_Char_Size(ftFace, 0, size, 0, 0));
        generateTextures();
    }
    
    int Font::getEmSize() const
    {
        return ftFace->size->metrics.height;
    }
    
    const Glyph* Font::getGlyph(unsigned char ch) const
    {
        return &textureMap.at(ch);
    }
    
    FT_GlyphSlot Font::renderGlyph(char ch) 
    {
        FT_CHECK_ERROR(FT_Load_Char(ftFace, ch, FT_LOAD_RENDER));
        return ftFace->glyph;
    }
}