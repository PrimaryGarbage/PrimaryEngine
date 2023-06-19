#include "label.hpp"
#include "graphics/renderer.hpp"
#include "globals.hpp"
#include "graphics/primitives.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "imgui.h"
#include <sstream>

namespace prim
{
    Label::Label(): Label("NewLabel") 
    {}
    
    Label::Label(std::string name): Control(name), glyphMesh(Primitives::createGlyphMesh()),
        backgroundMesh(Primitives::createSquareMesh(1.0f)), text(" ")
    {
        backgroundMesh.compositions.front().shader = Shader::getDefaultShader(DefaultShader::controlBackground);
    }
    
    void Label::draw(Renderer& renderer) 
    {
        NODE_DRAW
        (
            glm::vec2 globalPosition = getGlobalPosition();
            glm::vec2 globalSize = getSize();
            const glm::mat4& rendererViewMat = renderer.getViewMat();
            renderer.setViewMat(glm::mat4(1.0f));
            glyphMesh.compositions.front().shader->setUniform4f("u_color", textColor.r, textColor.g, textColor.b, textColor.a);

            // render background
            glm::mat4 modelMat(1.0f);
            modelMat = glm::translate(modelMat, glm::vec3(globalPosition.x, globalPosition.y, transform.zIndex));
            modelMat = glm::rotate(modelMat, getGlobalRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
            modelMat = glm::scale(modelMat, glm::vec3(globalSize.x * stringInfo.emSize.x + padding.x * 2.0f, globalSize.y * (stringInfo.emSize.y + stringInfo.emMaxDescend) + padding.y * 2.0f, 1.0f));
            backgroundMesh.compositions.front().shader->setUniform4f("u_color", backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
            backgroundMesh.compositions.front().shader->setUniform2f("u_resolution", stringInfo.pxSize);
            backgroundMesh.compositions.front().shader->setUniform1f("u_borderRadius", borderRadius);
            backgroundMesh.compositions.front().shader->setUniform1i("u_sampleTexture", 0);
            renderer.setModelMat(std::move(modelMat));
            renderer.drawMesh(backgroundMesh);

            float offset = 0.0f;
            for(const char& ch : text)
            {
                glm::mat4 modelMat(1.0f);
                const Glyph* glyph = font.getGlyph(ch);
                const glm::vec2 glyphOffset = glyph->emOffset * globalSize;
                const glm::vec3 pivotTranslation = glm::vec3(offset + glyphOffset.x + padding.x, glyphOffset.y + padding.y + stringInfo.emMaxDescend * globalSize.y, 0.0f);
                modelMat = glm::translate(modelMat, glm::vec3(globalPosition.x, globalPosition.y, transform.zIndex));
                modelMat = glm::rotate(modelMat, getGlobalRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
                modelMat = glm::translate(modelMat, pivotTranslation);
                modelMat = glm::scale(modelMat, glm::vec3(globalSize.x * glyph->emSize.x, globalSize.y * glyph->emSize.y, 1.0f));
                offset += globalSize.x * glyph->emAdvanceX;
                renderer.setModelMat(std::move(modelMat));
                glyphMesh.compositions.front().texture = glyph->texture;
                renderer.drawMesh(glyphMesh);
            }

            renderer.setViewMat(rendererViewMat);
        )
    }

    void Label::setText(std::string text)
    {
        this->text = text.empty() ? " " : text;
        this->stringInfo = font.calculateStringInfo(text);
    }

    std::string Label::serialize(bool withChildren) const 
    {
        std::stringstream ss;

        ss << Control::serialize(false);

        ss << Utils::createKeyValuePair(StateValues::text, text) << std::endl;
        ss << Utils::createKeyValuePair(StateValues::textColor, Utils::serializeVec4(textColor)) << std::endl;
        ss << Utils::createKeyValuePair(StateValues::backgroundColor, Utils::serializeVec4(backgroundColor)) << std::endl;
        ss << Utils::createKeyValuePair(StateValues::padding, Utils::serializeVec2(padding)) << std::endl;
        ss << Utils::createKeyValuePair(StateValues::borderRadius, std::to_string(borderRadius)) << std::endl;

        if(withChildren) ss << serializeChildren();

        return ss.str();
    }
    
    void Label::restore(NodeValues& nodeValues) 
    {
        Control::restore(nodeValues);   

        text = nodeValues[StateValues::text];
        textColor = Utils::deserializeVec4(nodeValues[StateValues::textColor]);
        backgroundColor = Utils::deserializeVec4(nodeValues[StateValues::backgroundColor]);
        padding = Utils::deserializeVec2(nodeValues[StateValues::padding]);
        borderRadius = std::stof(nodeValues[StateValues::borderRadius]);
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
        ImGui::DragFloat("Border Radius", &borderRadius, 0.1f, 0.0f, 100.0f);
    }
    
} // namespace prim
