#ifndef __UI_HPP__
#define __UI_HPP__

#include "imgui.h"
#include <vector>
#include <string>


namespace prim
{

    class Renderer;

    struct DragFloat
    {
        const char* label;
        float* f;
        float speed;
    };

    class UI
    {
        const float rightPanelWidth = 200.0f;

        Renderer* renderer;
        ImGuiIO* io;
        std::vector<std::string> printLines;
        std::vector<DragFloat> dragFloats;
        
        void drawRightPanel();

    public:
        UI() = default;
        ~UI();
        void init(Renderer* renderer);
        void draw();
        void print(std::string str);
        void addDragFloat(const char* label, float* f, float speed = 0.1f);
    };
}

#endif // __UI_HPP__