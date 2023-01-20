#ifndef __SPRITE_HPP__
#define __SPRITE_HPP__

#include "drawable.hpp"
#include "src/graphics/mesh.hpp"
#include "src/graphics/image.hpp"

namespace prim
{

    enum class ImageType;

    class Sprite : public Drawable
    {
        NODE_FIXTURE(Sprite)
    protected:
        
        struct StateFields: public Node2D::StateFields
        {
            inline static const char* width = "width";
            inline static const char* height = "height";
            inline static const char* zIndex = "zIndex";
            inline static const char* imagePath = "imagePath";
        };

        inline static const float defaultSize = 100.0f;
        Mesh planeMesh;
        shptr<Image> image;
        float width;
        float height;
        float relativeWidth;
        float relativeHeight;
        float zIndex = 0.0f;
    public:
        Sprite();
        Sprite(std::string name);
        Sprite(std::string name, std::string imagePath);
        Sprite(const Sprite& other) = default;
        virtual ~Sprite();

        virtual void start() override;
        virtual void update(float deltaTime) override;
        virtual void draw(Renderer& renderer) override;

        void setCenterPivot();
        void setCornerPivot();

        void setSize(float width, float height);
        void setWidth(float width);
        void setHeight(float height);
        void setImage(std::string path);
        void setZIndex(float value);
        inline float getWidth() const { return width; }
        inline float getHeight() const { return height; }
        inline glm::vec2 getSize() const { return glm::vec2(width, height); }
        inline float getZIndex() const { return zIndex; }
        inline const Image* getImage() const { return &*image; }

        virtual std::string serialize(bool withChildren = true) const override;
        virtual void deserialize(FieldValues& fieldValues) override;
        virtual void renderFields(SceneEditor* sceneEditor) override;
        virtual void unbind() override;

    };

}

#endif // __SPRITE_HPP__