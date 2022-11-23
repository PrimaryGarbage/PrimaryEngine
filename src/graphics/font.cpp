#include "font.hpp"
#include "prim_exception.hpp"
#include "resource_manager.hpp"

#define FT_CHECK_ERROR(error) if(error) throw PRIM_EXCEPTION("Freetype raised an exception: error code " + std::to_string(error))

namespace prim
{
    Font::Font() 
    {
        FT_CHECK_ERROR(FT_Init_FreeType(&ftLibrary));
        FT_CHECK_ERROR(FT_New_Face(ftLibrary, ResourceManager::getDefaultFontFilePath().c_str(), 0, &ftFace));
    }
    
    Font::Font(std::string filename) 
    {
        FT_CHECK_ERROR(FT_Init_FreeType(&ftLibrary));
        load(std::move(filename));
    }
    
    void Font::load(std::string filename) 
    {
        FT_CHECK_ERROR(FT_New_Face(ftLibrary, ResourceManager::getFontFilePath(filename).c_str(), 0, &ftFace));
    }
    
    void Font::load(uchar* data, uint size) 
    {
        FT_CHECK_ERROR(FT_New_Memory_Face(ftLibrary, data, size, 0, &ftFace));
    }
    
    void Font::setSize(float size) 
    {
        FT_CHECK_ERROR(FT_Set_Char_Size(ftFace, 0, size, 0, 0));
    }
    
    float Font::getSize() const
    {
        return ftFace->size->metrics.height;
    }
    
    Glyph Font::renderGlyph(char ch) 
    {
        FT_CHECK_ERROR(FT_Load_Char(ftFace, ch, FT_LOAD_RENDER));
        return ftFace->glyph;
    }
}