#ifndef __LABEL_HPP__
#define __LABEL_HPP__

#include "control.hpp"
#include "graphics/font.hpp"
#include "graphics/mesh.hpp"

namespace prim
{

    class Label: public Control
    {
        NODE_FIXTURE(Label)
    
    protected:
        struct StateFields : public Control::StateFields
        {
            inline static const char* text = "text";
            inline static const char* textColor = "textColor";
            inline static const char* backgroundColor = "backgoundColor";
            inline static const char* padding = "padding";
        };

    private:
        const static unsigned int textBufferSize = 1000u;

        std::string text;
        Font font;
        Mesh glyphMesh;
        Mesh backgroundMesh;
        glm::vec4 textColor = glm::vec4(1.0f);
        glm::vec4 backgroundColor {0.0f, 0.0f, 0.0f, 1.0f};
        glm::vec2 padding { 1.0f, 1.0f };
    public:
        using Control::Control;
        Label();
        Label(std::string name);
        virtual ~Label() {};

        inline std::string_view getText() const { return text; }
        inline glm::vec4 getTextColor() const { return textColor; }

        inline void setText(std::string text) { this->text = text; }
        inline void setTextColor(glm::vec4 color) { this->textColor = color; }
        inline void setBackgroundColor(glm::vec4 color) { this->backgroundColor = color; }

        virtual void start() override;
        virtual void update(float deltaTime) override;
        virtual void draw(Renderer& renderer) override;

        virtual std::string serialize(bool withChildren = true) const override;
        virtual void deserialize(FieldValues& fieldValues) override;
        virtual void renderFields(SceneEditor* sceneEditor) override;
    };
    
} // namespace prim


#endif // __LABEL_HPP__