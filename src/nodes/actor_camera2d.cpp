#include "actor_camera2d.hpp"
#include "gtx/rotate_vector.hpp"
#include "utils.hpp"
#include "globals.hpp"
#include "node_utils.hpp"
#include "imgui.h"
#include "utils.hpp"

namespace prim
{
    ActorCamera2D::ActorCamera2D(std::string name, const NodePath& target)
        : ActorCamera2D(name, -1.0f, 1.0f, target)
    {
    }

    ActorCamera2D::ActorCamera2D(std::string name, float zNear, float zFar, const NodePath& target)
        : Camera2D(name, zNear, zFar), targetPath(target)
    {
    }

    ActorCamera2D::ActorCamera2D(FieldValues& fieldValues)
        : Camera2D(fieldValues), stiffness(std::stof(fieldValues[StateFields::stiffness])),
        initialOffset(Utils::deserializeVec2(fieldValues[StateFields::initialOffset])),
        targetPath(fieldValues[StateFields::targetPath])
    {
    }

    ActorCamera2D::~ActorCamera2D()
    {
    }

    void ActorCamera2D::start()
    {
        startChildren();

            target = dynamic_cast<Node2D*>(Globals::app->getNode(targetPath));
        if (!target) return;
        //initialOffset = getGlobalPosition() - target->getGlobalPosition();
    }

    void ActorCamera2D::update(float deltaTime)
    {
        updateChildren(deltaTime);

        if (target)
        {
            auto followTargetFunction = [this]() {
                glm::vec2 position;
                if (rotateWithTarget)
                {
                    float targetAngle = target->getGlobalRotation();
                    position = glm::mix(getGlobalPosition(), target->getGlobalPosition() + glm::rotate(initialOffset, targetAngle), stiffness);
                    setGlobalRotation(Utils::lerpAngle(getGlobalRotation(), targetAngle, stiffness));
                }
                else
                    position = glm::mix(getGlobalPosition(), target->getGlobalPosition() + initialOffset, stiffness);

                setGlobalPosition(position);
            };

            Globals::app->deferFunctionExecution(followTargetFunction);
        }
    }

    void ActorCamera2D::draw(Renderer& renderer)
    {
        drawChildren(renderer);
    }

    void ActorCamera2D::setTarget(const NodePath& target)
    {
        targetPath = target;
        this->target = dynamic_cast<Node2D*>(Globals::app->getNode(targetPath));
    }

    void ActorCamera2D::setStiffness(float value)
    {
        stiffness = Utils::clamp(value, 0.0f, 1.0f);
    }

    std::string ActorCamera2D::serialize(bool withChildren) const
    {
        std::stringstream ss;

        ss << Camera2D::serialize(false);

        ss << Utils::createKeyValuePair(StateFields::targetPath, targetPath.string()) << std::endl;
        ss << Utils::createKeyValuePair(StateFields::initialOffset, Utils::serializeVec2(initialOffset)) << std::endl;
        ss << Utils::createKeyValuePair(StateFields::stiffness, std::to_string(stiffness)) << std::endl;
        ss << Utils::createKeyValuePair(StateFields::rotateWithTarget, std::to_string((int)rotateWithTarget)) << std::endl;

        if(withChildren) ss << serializeChildren();

        return ss.str();
    }

    void ActorCamera2D::renderFields()
    {
        Camera2D::renderFields();

        ImGui::Checkbox("Rotate with target", &rotateWithTarget);

        ImGui::PushStyleColor(ImGuiCol_Text, target ? Utils::Color::White : Utils::Color::Red);
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

        static float initialOffsetBuffer[2];
        initialOffsetBuffer[0] = initialOffset.x;
        initialOffsetBuffer[1] = initialOffset.y;
        if (ImGui::DragFloat2("Initial Offset", initialOffsetBuffer))
        {
            initialOffset = glm::vec2(initialOffsetBuffer[0], initialOffsetBuffer[1]);
        }

        ImGui::DragFloat("Stiffness", &stiffness, 0.01f, 0.0f, 1.0f);
    }

}