#ifndef __SCENE_EDITOR_HPP__
#define __SCENE_EDITOR_HPP__

#include "imgui.h"
#include <vector>
#include <string>

#define INPUT_STRING_MAX_LENGTH 50

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

    class SceneEditor
    {
    private:
        const float rightPanelWidth = 200.0f;
        const ImVec2 fileExplorerMinSize = { 640.0f, 480.0f };
        const ImVec2 popupMinSize = { 250.0f, 170.0f };

        Renderer* renderer;
        ImGuiIO* io;
        std::vector<std::string> printLines;
        std::vector<DragFloat> dragFloats;
        Node* selectedNode = nullptr;
        Node* nodeToAddTo = nullptr;
        bool initialized = false;
        
        void drawRightPanel();
        void drawLeftPanel();
        void drawNodeInTree(Node* node);
        void drawSelectedNodeFraming();
        void drawLoadSceneButton();
        void drawSaveSceneButton();
        void drawNodeTreeContextMenu(Node* node, bool hovered);
        void drawCreateNodeMenu();

    public:
        static inline const char* dragNodePayloadType = "NODE";

        SceneEditor() = default;
        ~SceneEditor();
        void init(Renderer* renderer);
        void draw();
        void print(std::string str);
        void addDragFloat(const char* label, float* f, float speed = 0.1f);
        inline void setSelectedNode(Node* node) { selectedNode = node; }
        inline Node* getSelectedNode() const { return selectedNode; }
    };
}

#endif // __SCENE_EDITOR_HPP__