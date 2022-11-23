#ifndef __LABEL_HPP__
#define __LABEL_HPP__

#include "control.hpp"

namespace prim
{

    class Label: public Control
    {
        NODE_FIXTURE(Label)
    
    protected:
        struct StateFields : public Control::StateFields
        {
            inline static const char* text = "text";
        };

    private:
        const static unsigned int textBufferSize = 1000u;

        std::string text;
    public:
        using Control::Control;
        Label();
        Label(std::string name);
        virtual ~Label() {};

        inline std::string_view getText() const { return text; }
        inline void setText(std::string text) { this->text = text; }

        virtual void start() override;
        virtual void update(float deltaTime) override;
        virtual void draw(Renderer& renderer) override;

        virtual std::string serialize(bool withChildren = true) const override;
        virtual void deserialize(FieldValues& fieldValues) override;
        virtual void renderFields() override;
    };
    
} // namespace prim


#endif // __LABEL_HPP__