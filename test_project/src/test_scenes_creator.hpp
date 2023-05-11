#ifndef __TEST_SCENES_CREATOR_HPP__
#define __TEST_SCENES_CREATOR_HPP__

#include "primary_engine.hpp"
#include <vector>
#include <iostream>

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
            ActorCamera2D* actorCamera = new ActorCamera2D("TestActorCamera");
            actorCamera->setStiffness(0.3f);
            Button* button = new Button("Button");
            button->setText("Click");
            button->move({600.0f, 600.0f});
            Label* uiLabel = new Label("UILabel");
            Label* uiChildLabel = new Label("UIChildLabel");
            Panel* uiPanel = new Panel("UIPanel");
            Control* control = new Control("Control");
            control->setAnchor({0.0f, 1.0f});
            control->addChild(uiPanel);
            uiLabel->setText("Imagine this is a UI element");
            uiLabel->setBackgroundColor({ 0.0f, 0.6f, 0.9f, 0.1f});
            uiChildLabel->setText("And this is a UI element child");
            uiLabel->setAnchor({ 0.0f, 1.0f });
            uiLabel->setSize({ 80.0f, 80.0f });
            uiLabel->move({-90.0f, 500.0f});
            uiChildLabel->setPivot({ 1.0f, 0.0f });
            uiChildLabel->move(180.0f, -85.0f);
            uiChildLabel->setSize(60.0f, 60.0f);
            uiChildLabel->setTextColor({0.1f, 1.0f, 0.1f, 1.0f});
            uiPanel->setBackgroundColor({0.7f, 0.7f, 0.9f, 0.5f});
            uiPanel->setSize({800.0f, 600.0f});
            uiPanel->move({150.0f, -600.0f});

            scene->addChild(background);
            scene->addChild(sprite1);
            sprite1->addChild(sprite2);
            scene->addChild(actorCamera);
            uiLabel->addChild(uiChildLabel);
            uiPanel->addChild(uiLabel);
            scene->addChild(control);
            actorCamera->setTarget(sprite1->getNodePath());
            scene->addChild(button);

            button->buttonPressedEvent.subscribe("testPressed", [](){ std::cout << "Button pressed!" << std::endl; });
            button->buttonReleasedEvent.subscribe("testReleased", [](){ std::cout << "Button released!" << std::endl; });

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