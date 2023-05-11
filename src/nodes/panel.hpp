#ifndef __PANEL_HPP__
#define __PANEL_HPP__

#include "control.hpp"
#include "graphics/mesh.hpp"

namespace prim
{
    class Panel : public Control
    {
        NODE_FIXTURE(Panel);
    
    protected:
        struct StateValues : public Control::StateValues
        {
            inline static const char* backgroundColor = "backgroundColor";
            inline static const char* borderRadius = "borderRadius";
            inline static const char* imagePath = "imagePath";
        };
    
    private:
        Mesh backgroundMesh;
        std::string imagePath;
        glm::vec4 backgroundColor{0.0f, 0.0f, 0.0f, 1.0f};
        float borderRadius{};
        bool useTexture{};

    public:
        using Control::Control;
        Panel();
        Panel(std::string name);

        inline void setBackgroundColor(glm::vec4 color) { this->backgroundColor = color; }
        inline void setBorderRadius(float radius) { this->borderRadius = radius; }

        virtual void draw(Renderer& renderer) override;

        virtual std::string serialize(bool withChildren = true) const override;
        virtual void restore(NodeValues& nodeValues) override;
        virtual void renderFields(SceneEditor* sceneEditor) override;

        void setTexture(std::string path);
        void removeTexture();
    };
}

#endif // __PANEL_HPP__