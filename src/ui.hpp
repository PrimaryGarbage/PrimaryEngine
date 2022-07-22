#ifndef __UI_HPP__
#define __UI_HPP__

#include "imgui.h"
#include <vector>
#include <string>


namespace prim
{

    class Renderer;
    class Node;

    struct DragFloat
    {
        const char* label;
        float* f;
        float speed;
    };

    class UI
    {
    private:
        const float rightPanelWidth = 200.0f;

        Renderer* renderer;
        ImGuiIO* io;
        std::vector<std::string> printLines;
        std::vector<DragFloat> dragFloats;
        Node* selectedNode = nullptr;
        
        void drawRightPanel();
        void drawLeftPanel();
        void drawNodeInTree(Node* node);

    public:
        static inline const char* dragNodePayloadType = "NODE";

        UI() = default;
        ~UI();
        void init(Renderer* renderer);
        void draw();
        void print(std::string str);
        void addDragFloat(const char* label, float* f, float speed = 0.1f);
    };
}

#endif // __UI_HPP__