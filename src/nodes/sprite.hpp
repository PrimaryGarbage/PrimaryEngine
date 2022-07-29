#ifndef __SPRITE_HPP__
#define __SPRITE_HPP__

#include "node2d.hpp"
#include "mesh.hpp"
#include "image.hpp"

namespace prim
{

    enum class ImageType;

    class Sprite : public Node2D
    {
    protected:
        inline static const float defaultSize = 100.0f;
        Mesh planeMesh;
        Image image;
        float width;
        float height;
        float relativeWidth;
        float relativeHeight;
        float zIndex = 0.0f;
    public:
        inline static const std::string typeName = "Sprite";

        Sprite(std::string name);
        Sprite(std::string name, std::string imagePath);
        Sprite(std::unordered_map<std::string, std::string>& fieldValues);
        virtual ~Sprite();

        virtual void start() override;
        virtual void update(float deltaTime) override;
        virtual void draw(Renderer& renderer) override;

        void setCenterPivot();

        void setSize(float width, float height);
        void setWidth(float width);
        void setHeight(float height);
        void setImage(std::string path);
        void setZIndex(float value);
        inline float getWidth() const { return width; }
        inline float getHeight() const { return height; }
        inline glm::vec2 getSize() const { return glm::vec2(width, height); }
        inline float getZIndex() const { return zIndex; }
        inline const Image* getImage() const { return &image; }

        virtual std::string serialize() const override;
        virtual inline const std::string getTypeName() const override { return typeName; }
        virtual void renderFields() override;

    };

}

#endif // __SPRITE_HPP__