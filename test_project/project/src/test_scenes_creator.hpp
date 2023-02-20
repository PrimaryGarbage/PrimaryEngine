#ifndef __TEST_SCENES_CREATOR_HPP__
#define __TEST_SCENES_CREATOR_HPP__

#include "primary_engine.hpp"

namespace prim
{
    class Node;

    class TestScenesCreator
    {
    public:
        static inline Node* createScene1()
        {
            Node* scene = new Node("TestScene1");
            Sprite* background = new Sprite("TestBackground", "textures/abstract_stairs.png");
            background->setSize(1920.0f, 1080.0f);
            Sprite* sprite1 = new Sprite("TestSprite1");
            sprite1->setCenterPivot();
            Sprite* sprite2 = new Sprite("TestSprite2");
            sprite2->setCenterPivot();
            sprite2->move(200.0f, 200.0f);
            ActorCamera2D* actorCamera = new ActorCamera2D("TestActorCamera", sprite1->getNodePath());
            actorCamera->setStiffness(0.3f);

            scene->addChild(background);
            scene->addChild(sprite1);
            scene->addChild(sprite2);
            scene->addChild(actorCamera);

            return scene;
        }

        static inline Node* createScene2()
        {
            Node* scene = new Node("TestScene2");
            Sprite* background = new Sprite("TestBackground", "textures/abstract_stairs.png");
            background->setSize(1920.0f, 1080.0f);
            Sprite* sprite1 = new Sprite("TestSprite1");
            sprite1->setCenterPivot();
            Sprite* sprite2 = new Sprite("TestSprite2");
            sprite2->setCenterPivot();
            sprite2->move(200.0f, 200.0f);
            ActorCamera2D* actorCamera = new ActorCamera2D("TestActorCamera", sprite1->getNodePath());
            actorCamera->setStiffness(0.3f);

            scene->addChild(background);
            scene->addChild(sprite1);
            sprite1->addChild(sprite2);
            scene->addChild(actorCamera);

            return scene;
        }

        static inline Node* createScene3()
        {
            Node* scene = new Node("TestScene3");
            Sprite* background = new Sprite("TestBackground", "textures/abstract_stairs.png");
            background->setSize(1920.0f, 1080.0f);
            Sprite* sprite1 = new Sprite("TestSprite1");
            sprite1->setCenterPivot();
            Sprite* sprite2 = new Sprite("TestSprite2");
            sprite2->setCenterPivot();
            sprite2->move(200.0f, 200.0f);
            ActorCamera2D* actorCamera = new ActorCamera2D("TestActorCamera", sprite1->getNodePath());
            actorCamera->setStiffness(0.3f);
            Label* uiLabel = new Label("UILabel");
            Label* uiChildLabel = new Label("UIChildLabel");
            uiLabel->setText("Imagine this is a UI element");
            uiLabel->setBackgroundColor({ 0.0f, 0.6f, 0.9f, 1.0f});
            uiChildLabel->setText("And this is a UI element child");
            uiLabel->setAnchor({ 0.0f, 1.0f });
            uiLabel->setSize({ 80.0f, 80.0f });
            uiLabel->move({480.0f, -130.0f});
            uiChildLabel->setPivot({ 1.0f, 0.0f });
            uiChildLabel->move(180.0f, -85.0f);
            uiChildLabel->setSize(60.0f, 60.0f);
            uiChildLabel->setTextColor({0.1f, 1.0f, 0.1f, 1.0f});

            scene->addChild(background);
            scene->addChild(sprite1);
            sprite1->addChild(sprite2);
            scene->addChild(actorCamera);
            scene->addChild(uiLabel);
            uiLabel->addChild(uiChildLabel);

            return scene;
        }

        static inline Node* createLabelScene()
        {
            Node* scene = new Node("TestScene3");
            Label* label = new Label("TestLabel");
            label->setText("Test text");
            scene->addChild(label);

            return scene;
        }
    };
}


#endif // __TEST_SCENES_CREATOR_HPP__