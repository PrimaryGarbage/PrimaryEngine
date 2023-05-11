#ifndef __BUTTON_HPP__
#define __BUTTON_HPP__

#include "control.hpp"
#include "graphics/font.hpp"

namespace prim
{
    class Button : public Control
    {
        NODE_FIXTURE(Button)

    private:
        struct ButtonState
        {
            std::string text;
            std::string imagePath;
            Mesh backgroundMesh;
            glm::vec4 textColor = glm::vec4(1.0f);
            glm::vec4 backgroundColor {0.0f, 0.0f, 0.0f, 1.0f};
            StringFontInfo stringInfo;
            bool useTexture{};
        };

    protected:
        struct StateValues : public Control::StateValues
        {
            inline static const char* text = "text";
            inline static const char* textColor = "textColor";
            inline static const char* backgroundColor = "backgoundColor";
            inline static const char* padding = "padding";
            inline static const char* borderRadius = "borderRadius";
            inline static const char* imagePath = "imagePath";
        };

        void updateControlState();

    private:
        const static unsigned int textBufferSize = 300u;

        ButtonState controlStateValues[static_cast<int>(ControlState::__count)];
        ControlState state{ControlState::Idle};
        Font font;
        Mesh glyphMesh;
        glm::vec2 padding = glm::vec2(1.0f);
        float borderRadius = 0.0f;
    public:
        using Control::Control;
        Button();
        Button(std::string name);

        Event<> buttonPressedEvent;
        Event<> buttonReleasedEvent;

        inline std::string_view getText() const { return controlStateValues[static_cast<int>(state)].text; }
        inline glm::vec4 getTextColor() const { return controlStateValues[static_cast<int>(state)].textColor; }
        inline glm::vec4 getBackgroudColor() const { return controlStateValues[static_cast<int>(state)].backgroundColor; }
        inline glm::vec2 getPadding() const { return padding; }
        inline float getBorderRadius() const { return borderRadius; }
        inline bool isPressed() const { return state == ControlState::Active; }
        inline bool isHovered() const { return state == ControlState::Selected; }

        void setText(std::string text, ControlState state);
        void setTexture(std::string path, ControlState state);
        void removeTexture(ControlState state);
        void setTextColor(glm::vec4 color, ControlState state);
        void setBackgroundColor(glm::vec4 color, ControlState state);

        void setText(std::string text);
        void setTexture(std::string path);
        void removeTexture();
        void setTextColor(glm::vec4 color);
        void setBackgroundColor(glm::vec4 color);

        inline void setPadding(glm::vec2 padding) { padding = padding; }
        inline void setBorderRadius(float radius) { this->borderRadius = radius; }

        virtual void uiUpdate(float deltaTime) override;
        virtual void draw(Renderer& renderer) override;

        virtual std::string serialize(bool withChildren = true) const override;
        virtual void restore(NodeValues& nodeValues) override;
        virtual void renderFields(SceneEditor* sceneEditor) override;
    };
}

#endif // __BUTTON_HPP__