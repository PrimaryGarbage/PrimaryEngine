#ifndef __CONTROL_HPP__
#define __CONTROL_HPP__

#include "node.hpp"
#include "control_transform.hpp"

namespace prim
{

    class Control : public Node
    {
        NODE_FIXTURE(Control)

    protected:
        struct StateFields : public Node::StateFields
        {
            inline static const char* position = "position";
            inline static const char* rotation = "rotation";
            inline static const char* size = "size";
            inline static const char* pivot = "pivot";
        };

        ControlTransform transform;
        const char* defaultFontName = "Roboto-Regular.ttf";

        static std::string getPathToFontFile(std::string filename);

    public:
        Control();
        Control(std::string name);
        virtual ~Control() {};

        void move(glm::vec2 v);
        void move(float x, float y);
        void rotate(float angle);

        virtual glm::vec2 getPosition() const override final;
        virtual float getRotation() const override final;
        virtual glm::vec2 getSize() const;
        virtual glm::vec2 getPivot() const override final;

        virtual glm::vec2 getGlobalPosition() const override final;
        virtual float getGlobalRotation() const override final;
        virtual glm::vec2 getGlobalScale() const override final;

        virtual void setPosition(glm::vec2 v) override final;
        virtual void setRotation(float angle) override final;
        virtual void setSize(glm::vec2 s);
        virtual void setSize(float width, float height);
        virtual void setWidth(float width);
        virtual void setHeight(float height);
        virtual void setPivot(glm::vec2 pivot) override final;

        virtual void setGlobalPosition(glm::vec2 v) override final;
        virtual void setGlobalRotation(float angle) override final;

        virtual std::string serialize(bool withChildren = true) const override;
        virtual void deserialize(FieldValues& fieldValues) override;
        virtual void renderFields() override;
    };
}

#endif // __CONTROL_HPP__