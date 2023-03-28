#ifndef __SPRITE_HPP__
#define __SPRITE_HPP__

#include "drawable.hpp"
#include "graphics/mesh.hpp"
#include "graphics/image.hpp"

namespace prim
{

    enum class ImageType;

    class Sprite : public Drawable
    {
        NODE_FIXTURE(Sprite)
    protected:
        
        struct StateValues: public Node2D::StateValues
        {
            inline static const char* width = "width";
            inline static const char* height = "height";
            inline static const char* zIndex = "zIndex";
            inline static const char* imagePath = "imagePath";
        };

        inline static const float defaultSize = 100.0f;
        Mesh planeMesh;
        Shp<Image> image;
        float width;
        float height;
        float zIndex = 0.0f;
        glm::vec4 tint = glm::vec4(1.0f);
    public:
        Sprite();
        Sprite(std::string name);
        Sprite(std::string name, std::string imagePath);
        Sprite(const Sprite& other);
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
        float getRelativeWidth() const;
        float getRelativeHeight() const;
        inline glm::vec2 getSize() const { return glm::vec2(width, height); }
        inline float getZIndex() const { return zIndex; }
        inline const Image* getImage() const { return &*image; }

        virtual std::string serialize(bool withChildren = true) const override;
        virtual void restore(NodeValues& nodeValues) override;
        virtual void renderFields(SceneEditor* sceneEditor) override;

    };

}

#endif // __SPRITE_HPP__