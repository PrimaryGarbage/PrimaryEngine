#include "label.hpp"
#include "renderer.hpp"
#include "globals.hpp"

namespace prim
{
    Label::Label(): Control()
    {
    }
    
    Label::Label(std::string name): Control(name)
    {
    }

    void Label::start() 
    {
    }
    
    void Label::update(float deltaTime) 
    {
    }
    
    void Label::draw(Renderer& renderer) 
    {
    }
    
    std::string Label::serialize(bool withChildren) const 
    {
        std::stringstream ss;

        ss << Control::serialize(false);

        ss << Utils::createKeyValuePair(StateFields::text, text) << std::endl;

        if(withChildren) ss << serializeChildren();

        return ss.str();
    }
    
    void Label::deserialize(FieldValues& fieldValues) 
    {
        Control::deserialize(fieldValues);   

        text = fieldValues[StateFields::text];
    }
    
    void Label::renderFields() 
    {
        Control::renderFields();
        
        static char textBuf[textBufferSize];
        if(ImGui::InputTextMultiline("Text", textBuf, textBufferSize))
        {
            setText(textBuf);
        }
    }

    
} // namespace prim
