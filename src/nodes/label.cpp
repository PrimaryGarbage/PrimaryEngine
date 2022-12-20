#include "label.hpp"
#include "renderer.hpp"
#include "globals.hpp"
#include "primitives.hpp"
#include "gtc/matrix_transform.hpp"

namespace prim
{
    Label::Label(): Control(), glyphMesh(Primitives::createGlyphMesh())
    {
    }
    
    Label::Label(std::string name): Control(name), glyphMesh(Primitives::createGlyphMesh())
    {
    }

    void Label::start() 
    {
        startChildren();
    }
    
    void Label::update(float deltaTime) 
    {
        updateChildren(deltaTime);
    }
    
    void Label::draw(Renderer& renderer) 
    {
        drawChildren(renderer);

        glm::vec2 globalPosition = getGlobalPosition();
        glm::vec2 globalSize = getSize();
        float offset = 0.0f;

        for(const char& ch : text)
        {
            glm::mat4 modelMat(1.0f);
            const Glyph* glyph = font.getGlyph(ch);
            const glm::vec3 pivotTranslation = glm::vec3(offset, 0.0f, 0.0f);
            const glm::vec2 glyphOffset = glyph->offset * globalSize;
            modelMat = glm::translate(modelMat, glm::vec3(globalPosition.x + offset + glyphOffset.x, globalPosition.y + glyphOffset.y, transform.zIndex));
            modelMat = glm::translate(modelMat, -pivotTranslation);
            modelMat = glm::rotate(modelMat, getGlobalRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
            modelMat = glm::translate(modelMat, pivotTranslation);
            modelMat = glm::scale(modelMat, glm::vec3(globalSize.x * glyph->size.x, globalSize.y * glyph->size.y, 1.0f));
            offset += globalSize.x * glyph->advanceX;
            renderer.setModelMat(std::move(modelMat));
            glyphMesh.compositions.front().texture = glyph->texture;
            renderer.drawMesh(glyphMesh);
        }
    }
    
    std::string Label::serialize(bool withChildren) const 
    {
        std::stringstream ss;

        ss << Control::serialize(false);

        ss << Utils::createKeyValuePair(StateFields::text, text) << std::endl;

        if(withChildren) ss << serializeChildren();

        return ss.str();
    }
    
    void Label::deserialize(FieldValues& fieldValues) 
    {
        Control::deserialize(fieldValues);   

        text = fieldValues[StateFields::text];
    }
    
    void Label::renderFields() 
    {
        Control::renderFields();
        
        static char textBuf[textBufferSize];
        if(ImGui::InputTextMultiline("Text", textBuf, textBufferSize))
        {
            setText(textBuf);
        }
    }

    
} // namespace prim
