#include "label.hpp"
#include "graphics/renderer.hpp"
#include "globals.hpp"
#include "graphics/primitives.hpp"
#include "GLM/gtc/matrix_transform.hpp"

namespace prim
{
    Label::Label(): Control(), glyphMesh(Primitives::createGlyphMesh()), 
        backgroundMesh(Primitives::createSquareMesh(1.0f))
    {
        backgroundMesh.compositions.front().shader = Shader::getDefaultShader(DefaultShader::controlBackground);
    }
    
    Label::Label(std::string name): Control(name), glyphMesh(Primitives::createGlyphMesh()),
        backgroundMesh(Primitives::createSquareMesh(1.0f))
    {
        backgroundMesh.compositions.front().shader = Shader::getDefaultShader(DefaultShader::controlBackground);
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
        backgroundMesh.compositions.front().shader->setUniform4f("u_color", backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);

        const glm::mat4& rendererViewMat = renderer.getViewMat();
        renderer.setViewMat(glm::mat4(1.0f));

        // render background
        glm::mat4 modelMat(1.0f);
        StringFontInfo stringInfo = font.calculateStringInfo(text);
        modelMat = glm::translate(modelMat, glm::vec3(globalPosition.x - padding.x, globalPosition.y - globalSize.y * stringInfo.emMaxDescend - padding.y, transform.zIndex));
        modelMat = glm::rotate(modelMat, getGlobalRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMat = glm::scale(modelMat, glm::vec3(globalSize.x * stringInfo.emSize.x + padding.x * 2.0f, globalSize.y * (stringInfo.emSize.y + stringInfo.emMaxDescend) + padding.y * 2.0f, 1.0f));
        backgroundMesh.compositions.front().shader->setUniform1f("u_borderRadius", borderRadius);
        renderer.setModelMat(std::move(modelMat));
        renderer.drawMesh(backgroundMesh);

        for(const char& ch : text)
        {
            glm::mat4 modelMat(1.0f);
            const Glyph* glyph = font.getGlyph(ch);
            const glm::vec3 pivotTranslation = glm::vec3(offset, 0.0f, transform.zIndex);
            const glm::vec2 glyphOffset = glyph->emOffset * globalSize;
            modelMat = glm::translate(modelMat, glm::vec3(globalPosition.x + offset + glyphOffset.x, globalPosition.y + glyphOffset.y, transform.zIndex));
            modelMat = glm::translate(modelMat, -pivotTranslation);
            modelMat = glm::rotate(modelMat, getGlobalRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
            modelMat = glm::translate(modelMat, pivotTranslation);
            modelMat = glm::scale(modelMat, glm::vec3(globalSize.x * glyph->emSize.x, globalSize.y * glyph->emSize.y, 1.0f));
            offset += globalSize.x * glyph->emAdvanceX;
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
        ss << Utils::createKeyValuePair(StateFields::backgroundColor, Utils::serializeVec4(backgroundColor)) << std::endl;
        ss << Utils::createKeyValuePair(StateFields::padding, Utils::serializeVec2(padding)) << std::endl;
        ss << Utils::createKeyValuePair(StateFields::borderRadius, std::to_string(borderRadius)) << std::endl;

        if(withChildren) ss << serializeChildren();

        return ss.str();
    }
    
    void Label::deserialize(FieldValues& fieldValues) 
    {
        Control::deserialize(fieldValues);   

        text = fieldValues[StateFields::text];
        textColor = Utils::deserializeVec4(fieldValues[StateFields::textColor]);
        backgroundColor = Utils::deserializeVec4(fieldValues[StateFields::backgroundColor]);
        padding = Utils::deserializeVec2(fieldValues[StateFields::padding]);
        borderRadius = std::stof(fieldValues[StateFields::borderRadius]);
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
        ImGui::DragFloat2("Padding", &padding.x, 0.1f, 0.0f, std::numeric_limits<float>::max());
        ImGui::ColorEdit4("Text Color", &textColor.x);
        ImGui::ColorEdit4("Background Color", &backgroundColor.x);
        ImGui::DragFloat("Border Radius", &borderRadius, 0.01f, 0.0f, 5.0f);
    }
    
} // namespace prim
