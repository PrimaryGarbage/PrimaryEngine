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

        // delete later
        const static float size = 15.0f;

        for(const char& ch : text)
        {
            glm::mat4 modelMat(1.0f);
            const Glyph* glyph = font.getGlyph(ch);
            modelMat = glm::translate(modelMat, glm::vec3(globalPosition.x + offset, globalPosition.y, transform.zIndex));
            modelMat = glm::rotate(modelMat, getGlobalRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
            modelMat = glm::scale(modelMat, glm::vec3(globalSize.x * glyph->width, globalSize.y * glyph->height, 1.0f));
            //modelMat = glm::translate(modelMat, -Utils::toVec3(getPivot()));
            offset += glyph->width + globalSize.x;
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
