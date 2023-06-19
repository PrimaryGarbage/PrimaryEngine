#include "actor_camera2d.hpp"
#include "GLM/gtx/rotate_vector.hpp"
#include "utils.hpp"
#include "globals.hpp"
#include "imgui.h"
#include "utils.hpp"
#include "node_utils.hpp"
#include "math.h"

namespace prim
{
    ActorCamera2D::ActorCamera2D(): ActorCamera2D(generateNodeName(this))
    {
    }

    ActorCamera2D::ActorCamera2D(std::string name): Camera2D(name)
    {
    }

    ActorCamera2D::ActorCamera2D(std::string name, const NodePath& target)
        : ActorCamera2D(name, -1.0f, 1.0f, target)
    {
    }

    ActorCamera2D::ActorCamera2D(std::string name, float zNear, float zFar, const NodePath& target)
        : Camera2D(name, zNear, zFar), targetPath(target)
    {
    }

    void ActorCamera2D::start()
    {
        NODE_START
        (
            if(!targetPath.empty())
                target = dynamic_cast<Node2D*>(Globals::app->getNode(targetPath));
        )
    }

    void ActorCamera2D::update(float deltaTime)
    {
        NODE_UPDATE
        (
            if (target)
            {
                auto followTargetFunction = [this]() {
                    glm::vec2 position;
                    float modifiedStiffness = std::pow(stiffness, 4);
                    if (rotateWithTarget)
                    {
                        float targetAngle = target->getGlobalRotation();
                        position = glm::mix(getGlobalPosition(), target->getGlobalPosition() + glm::rotate(initialOffset, targetAngle), modifiedStiffness);
                        setGlobalRotation(Utils::lerpAngle(getGlobalRotation(), targetAngle, modifiedStiffness));
                    }
                    else
                        position = glm::mix(getGlobalPosition(), target->getGlobalPosition() + initialOffset, modifiedStiffness);

                    setGlobalPosition(position);
                };

                Globals::app->deferFunctionExecution(followTargetFunction);
            }
        )
    }

    void ActorCamera2D::draw(Renderer& renderer)
    {
        NODE_DRAW()
    }

    void ActorCamera2D::setTarget(const NodePath& target)
    {
        targetPath = target;
        this->target = dynamic_cast<Node2D*>(Globals::app->getNode(targetPath));
    }

    void ActorCamera2D::setTarget(const Node2D* node)
    {
        this->target = node;
    }

    void ActorCamera2D::setStiffness(float value)
    {
        stiffness = Utils::clamp(value, 0.0f, 1.0f);
    }

    std::string ActorCamera2D::serialize(bool withChildren) const
    {
        std::stringstream ss;

        ss << Camera2D::serialize(false);

        ss << Utils::createKeyValuePair(StateValues::targetPath, targetPath.string()) << std::endl;
        ss << Utils::createKeyValuePair(StateValues::initialOffset, Utils::serializeVec2(initialOffset)) << std::endl;
        ss << Utils::createKeyValuePair(StateValues::stiffness, std::to_string(stiffness)) << std::endl;
        ss << Utils::createKeyValuePair(StateValues::rotateWithTarget, std::to_string((int)rotateWithTarget)) << std::endl;

        if(withChildren) ss << serializeChildren();

        return ss.str();
    }
    
    void ActorCamera2D::restore(NodeValues& nodeValues) 
    {
        Camera2D::restore(nodeValues);

        stiffness = std::stof(nodeValues[StateValues::stiffness]);
        initialOffset = Utils::deserializeVec2(nodeValues[StateValues::initialOffset]);
        targetPath = nodeValues[StateValues::targetPath];
    }

    void ActorCamera2D::renderFields(SceneEditor* sceneEditor)
    {
        Camera2D::renderFields(sceneEditor);

        ImGui::Checkbox("Rotate with target", &rotateWithTarget);

        ImGui::PushStyleColor(ImGuiCol_Text, Utils::castVec4<ImVec4>(target ? Utils::Color::White : Utils::Color::Red));
        ImGui::LabelText("Target", targetPath.string().c_str());
        ImGui::PopStyleColor();

        if (ImGui::BeginDragDropTarget())
        {
            const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(SceneEditor::dragNodePayloadType);
            if (payload)
            {
                Node* node = *static_cast<Node**>(payload->Data);
                Node2D* node2d = dynamic_cast<Node2D*>(node);
                if (node2d) setTarget(node2d->getNodePath());
                ImGui::EndDragDropTarget();
            }
        }

        ImGui::DragFloat2("Initial Offset", &initialOffset.x);
        ImGui::DragFloat("Stiffness", &stiffness, 0.01f, 0.0f, 1.0f);
    }

}