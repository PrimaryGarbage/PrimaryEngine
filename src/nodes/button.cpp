#include "button.hpp"
#include "graphics/primitives.hpp"
#include "imgui.h"
#include "GLM/gtc/matrix_transform.hpp"
#include "resource_manager.hpp"
#include "ImGuiFileDialog.h"
#include "misc/rectangle.hpp"

namespace prim
{
    Button::Button() : Button("NewLabel")
    {}

    Button::Button(std::string name) : Control(name), glyphMesh(Primitives::createGlyphMesh())
    {
        Shader* defaultShader = Shader::getDefaultShader(DefaultShader::controlBackground);
        for (int i = 0; i < static_cast<int>(ControlState::__count); ++i)
        {
            controlStateValues[i].backgroundMesh = Primitives::createSquareMesh(1.0f);
            controlStateValues[i].backgroundMesh.compositions.front().shader = defaultShader;
        }
    }

    void Button::setTexture(std::string path, ControlState state)
    {
        controlStateValues[static_cast<int>(state)].backgroundMesh.compositions[0].texture = Texture::create(ResourceManager::createResourcePath(path));
    }

    void Button::start()
    {
        startChildren();
    }

    void Button::update(float deltaTime)
    {
        updateChildren(deltaTime);

        Rectangle rect(getGlobalPosition(), getSize(), getPivot(), getRotation());
        glm::vec2 mousePos = Input::getCursorPos();
        Globals::mainRenderer->drawRectangle(rect.getPosition(), rect.getSize(), Utils::Color::Red);
        // Logger::inst().logInfo(Utils::serializeVec2(mousePos));
        // if(rect.inside(mousePos))
        // {
        //     Logger::inst().logInfo("INSIDE!!!");
        // }
    }

    void Button::draw(Renderer& renderer)
    {
        ButtonState& currentState = controlStateValues[static_cast<int>(state)];
        Mesh& backgroundMesh = currentState.backgroundMesh;
        std::string& text = currentState.text;
        glm::vec4& textColor = currentState.textColor;
        glm::vec4& backgroundColor = currentState.backgroundColor;
        glm::vec2& padding = currentState.padding;

        glm::vec2 globalPosition = getGlobalPosition();
        glm::vec2 globalSize = getSize();
        const glm::mat4& rendererViewMat = renderer.getViewMat();
        renderer.setViewMat(glm::mat4(1.0f));
        glyphMesh.compositions.front().shader->setUniform4f("u_color", textColor.r, textColor.g, textColor.b, textColor.a);

        // render background
        glm::mat4 modelMat(1.0f);
        StringFontInfo stringInfo = font.calculateStringInfo(text);
        modelMat = glm::translate(modelMat, glm::vec3(globalPosition.x, globalPosition.y, transform.zIndex));
        modelMat = glm::rotate(modelMat, getGlobalRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMat = glm::scale(modelMat, glm::vec3(globalSize.x * stringInfo.emSize.x + padding.x * 2.0f, globalSize.y * (stringInfo.emSize.y + stringInfo.emMaxDescend) + padding.y * 2.0f, 1.0f));
        backgroundMesh.compositions.front().shader->setUniform4f("u_color", backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
        backgroundMesh.compositions.front().shader->setUniform2f("u_resolution", stringInfo.pxSize);
        backgroundMesh.compositions.front().shader->setUniform1f("u_borderRadius", borderRadius);
        renderer.setModelMat(std::move(modelMat));
        renderer.drawMesh(backgroundMesh);

        // render text
        float offset = 0.0f;
        for (const char& ch : text)
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

        drawChildren(renderer);
    }

    std::string Button::serialize(bool withChildren) const
    {
        std::stringstream ss;

        ss << Control::serialize(false);

        for (int i = 0; i < static_cast<int>(ControlState::__count); ++i)
        {
            const ButtonState& st = controlStateValues[i];
            std::string statePostfix = std::to_string(i);

            ss << Utils::createKeyValuePair(StateValues::text + statePostfix, st.text) << std::endl;
            ss << Utils::createKeyValuePair(StateValues::textColor + statePostfix, Utils::serializeVec4(st.textColor)) << std::endl;
            ss << Utils::createKeyValuePair(StateValues::backgroundColor + statePostfix, Utils::serializeVec4(st.backgroundColor)) << std::endl;
            ss << Utils::createKeyValuePair(StateValues::imagePath + statePostfix, st.imagePath) << std::endl;
            ss << Utils::createKeyValuePair(StateValues::padding + statePostfix, Utils::serializeVec2(st.padding)) << std::endl;
        }

        ss << Utils::createKeyValuePair(StateValues::borderRadius, std::to_string(borderRadius)) << std::endl;

        if (withChildren) ss << serializeChildren();

        return ss.str();
    }

    void Button::restore(NodeValues& nodeValues)
    {
        Control::restore(nodeValues);

        for (int i = 0; i < static_cast<int>(ControlState::__count); ++i)
        {
            ButtonState& st = controlStateValues[i];
            std::string statePostfix = std::to_string(i);

            st.text = nodeValues[StateValues::text + statePostfix];
            st.textColor = Utils::deserializeVec4(nodeValues[StateValues::textColor + statePostfix]);
            st.backgroundColor = Utils::deserializeVec4(nodeValues[StateValues::backgroundColor + statePostfix]);
            st.imagePath = nodeValues[StateValues::imagePath + statePostfix];
            st.padding = Utils::deserializeVec2(nodeValues[StateValues::padding + statePostfix]);
        }

        borderRadius = std::stof(nodeValues[StateValues::borderRadius]);

        if (!controlStateValues[0].imagePath.empty())
            setTexture(controlStateValues[0].imagePath);
    }

    void Button::renderFields(SceneEditor* sceneEditor)
    {
        Control::renderFields(sceneEditor);

        static char textBuffers[static_cast<int>(ControlState::__count)][textBufferSize];
        const std::string& text = controlStateValues[static_cast<int>(state)].text;
        std::copy(text.begin(), text.end(), textBuffers[static_cast<int>(state)]);

        for (int i = 0; i < static_cast<int>(ControlState::__count); ++i)
        {
            ButtonState& st = controlStateValues[i];
            if (ImGui::CollapsingHeader(ControlState_str[i]))
            {
                if (ImGui::InputText("Text", textBuffers[i], textBufferSize))
                {
                    st.text = textBuffers[i];
                }
                ImGui::DragFloat2("Padding", &st.padding.x, 0.1f, 0.0f, std::numeric_limits<float>::max());
                ImGui::ColorEdit4("Text Color", &st.textColor.x);
                ImGui::ColorEdit4("Background Color", &st.backgroundColor.x);
                ImGui::DragFloat("Border Radius", &borderRadius, 0.1f, 0.0f, 100.0f);

                ImGui::LabelText("Image", st.imagePath.c_str());
                ImGui::SameLine();

                if (ImGui::Button("...")) // change image button
                {
                    ImGuiFileDialog::Instance()->OpenDialog("ChooseImageKey", "Open Image",
                        "Image files (*.png *.jpg *.jpeg){.png,.jpg,.jpeg}", ResourceManager::getResourceDirPathAbsolute(),
                        1, nullptr, ImGuiFileDialogFlags_Modal);
                }

                if (ImGuiFileDialog::Instance()->Display("ChooseImageKey"))
                {
                    if (ImGuiFileDialog::Instance()->IsOk())
                    {
                        std::string path = Utils::splitString(ImGuiFileDialog::Instance()->GetFilePathName(), ResourceManager::resDirName + ResourceManager::separator()).back();
                        setTexture(ResourceManager::createResourcePath(path), static_cast<ControlState>(i));
                    }

                    ImGuiFileDialog::Instance()->Close();
                }
            }
        }
    }

} // namespace prim