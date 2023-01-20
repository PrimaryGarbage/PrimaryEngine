#include "label.hpp"
#include "../graphics/renderer.hpp"
#include "globals.hpp"
#include "../graphics/primitives.hpp"
#include "gtc/matrix_transform.hpp"

namespace prim
{
    Label::Label(): Control(), glyphMesh(Primitives::createGlyphMesh()), 
        backgroundMesh(Primitives::createSquareMesh(1.0f))
    {
    }
    
    Label::Label(std::string name): Control(name), glyphMesh(Primitives::createGlyphMesh()),
        backgroundMesh(Primitives::createSquareMesh(1.0f))
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

        glyphMesh.compositions.front().shader->setUniform4f("u_color", textColor.r, textColor.g, textColor.b, textColor.a);
        backgroundMesh.compositions.front().shader->setUniform4f("u_color", backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);

        const glm::mat4& rendererViewMat = renderer.getViewMat();
        renderer.setViewMat(glm::mat4(1.0f));

        // render background
        glm::mat4 modelMat(1.0f);
        float width = font.calculateWidth(text);
        modelMat = glm::translate(modelMat, glm::vec3(globalPosition.x, globalPosition.y, transform.zIndex));
        modelMat = glm::rotate(modelMat, getGlobalRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
        // here: background size is wrong
        modelMat = glm::scale(modelMat, glm::vec3(globalSize.x * width, globalSize.y, 1.0f));
        renderer.setModelMat(std::move(modelMat));
        renderer.drawMesh(backgroundMesh);

        for(const char& ch : text)
        {
            glm::mat4 modelMat(1.0f);
            const Glyph* glyph = font.getGlyph(ch);
            const glm::vec3 pivotTranslation = glm::vec3(offset, 0.0f, transform.zIndex);
            const glm::vec2 glyphOffset = glyph->offset * globalSize;
            modelMat = glm::translate(modelMat, glm::vec3(globalPosition.x + offset + glyphOffset.x, globalPosition.y + glyphOffset.y, transform.zIndex));
            modelMat = glm::translate(modelMat, -pivotTranslation);
            modelMat = glm::rotate(modelMat, getGlobalRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
            modelMat = glm::translate(modelMat, pivotTranslation);
            modelMat = glm::scale(modelMat, glm::vec3(globalSize.x * glyph->size.x, globalSize.y * glyph->size.y, 1.0f));
            // modelMat = glm::translate(modelMat, glm::vec3(globalPosition.x + offset + glyphOffset.x, globalPosition.y + glyphOffset.y, transform.zIndex));
            // modelMat = glm::scale(modelMat, glm::vec3(globalSize.x * glyph->size.x, globalSize.y * glyph->size.y, 1.0f));
            offset += globalSize.x * glyph->advanceX;
            renderer.setModelMat(std::move(modelMat));
            glyphMesh.compositions.front().texture = glyph->texture;
            renderer.drawMesh(glyphMesh);
        }

        renderer.setViewMat(rendererViewMat);
    }

    std::string Label::serialize(bool withChildren) const 
    {
        std::stringstream ss;

        ss << Control::serialize(false);

        ss << Utils::createKeyValuePair(StateFields::text, text) << std::endl;

        ss << Utils::createKeyValuePair(StateFields::textColor, Utils::serializeVec4(textColor)) << std::endl;

        if(withChildren) ss << serializeChildren();

        return ss.str();
    }
    
    void Label::deserialize(FieldValues& fieldValues) 
    {
        Control::deserialize(fieldValues);   

        text = fieldValues[StateFields::text];
        textColor = Utils::deserializeVec4(fieldValues[StateFields::textColor]);
    }
    
    void Label::renderFields(SceneEditor* sceneEditor) 
    {
        Control::renderFields(sceneEditor);
        
        static char textBuf[textBufferSize];
        std::copy(text.begin(), text.end(), textBuf);

        if(ImGui::InputTextMultiline("Text", textBuf, textBufferSize))
        {
            setText(textBuf);
        }
        ImGui::ColorEdit4("Text Color", &textColor.x);
        ImGui::ColorEdit4("Background Color", &backgroundColor.x);
    }
    
} // namespace prim
