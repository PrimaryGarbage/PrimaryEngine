#ifndef __TEST_SCENES_CREATOR_HPP__
#define __TEST_SCENES_CREATOR_HPP__

#include "node_utils.hpp"

namespace prim
{
    class Node;

    class TestScenesCreator
    {
    public:
        static inline Node* createScene1()
        {
            Node* scene = new Node("TestScene1");
            Sprite* background = new Sprite("TestBackground", "./res/textures/abstract_stairs.png");
            background->setSize(1920.0f, 1080.0f);
            Sprite* sprite1 = new Sprite("TestSprite1", "./res/textures/TestTexture.png");
            sprite1->setCenterPivot();
            Sprite* sprite2 = new Sprite("TestSprite2", "./res/textures/TestTexture.png");
            sprite2->setCenterPivot();
            sprite2->move(200.0f, 200.0f);
            ActorCamera2D* actorCamera = new ActorCamera2D("TestActorCamera", sprite1->getNodePath());
            actorCamera->setStiffness(0.01f);

            scene->addChild(background);
            scene->addChild(sprite1);
            scene->addChild(sprite2);
            scene->addChild(actorCamera);

            return scene;
        }

        static inline Node* createScene2()
        {
            Node* scene = new Node("TestScene2");
            Sprite* background = new Sprite("TestBackground", "./res/textures/abstract_stairs.png");
            background->setSize(1920.0f, 1080.0f);
            Sprite* sprite1 = new Sprite("TestSprite1", "./res/textures/TestTexture.png");
            sprite1->setCenterPivot();
            Sprite* sprite2 = new Sprite("TestSprite2", "./res/textures/TestTexture.png");
            sprite2->setCenterPivot();
            sprite2->move(200.0f, 200.0f);
            ActorCamera2D* actorCamera = new ActorCamera2D("TestActorCamera", sprite1->getNodePath());
            actorCamera->setStiffness(0.01f);

            scene->addChild(background);
            scene->addChild(sprite1);
            sprite1->addChild(sprite2);
            scene->addChild(actorCamera);

            return scene;
        }
    };
}


#endif // __TEST_SCENES_CREATOR_HPP__