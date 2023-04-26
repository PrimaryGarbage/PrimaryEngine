#ifndef __NODE_HPP__
#define __NODE_HPP__

#include <vector>
#include <stack>
#include "GLM/glm.hpp"
#include "node_path.hpp"
#include "utils.hpp"

#define NODE_FIXTURE(NODE_NAME) \
    private: inline static const NodeRegistration<NODE_NAME> nodeRegistration = NodeRegistration<NODE_NAME>(#NODE_NAME); \
    public: virtual inline const char* type() const override { return #NODE_NAME; }


namespace prim
{
    class Renderer;
    class SceneEditor;

    class Node
    {
    private:
        static inline std::unordered_map<std::string, Node* (*)()> nodeTypeMap;
        static constexpr unsigned int maxId = 100000;
        inline static bool idPool[maxId]{};
        inline static std::stack<unsigned int> freeIds;

        void updateNodePath();

        // helper function. Serves as a generic node factory
        template<class T>
        inline static Node* constructNode()
        {
            return new T();
        }

    protected:

        template<class T>
        class NodeRegistration
        {
        public:
            NodeRegistration(const char* type)
            {
                nodeTypeMap.insert(std::make_pair(type, &constructNode<T>));
            }
        };

        private: inline static const NodeRegistration<Node> nodeRegistration = NodeRegistration<Node>("Node"); \
        public: virtual inline const char* type() const { return "Node"; }

        struct StateValues
        {
            inline static const char* type = "type";
            inline static const char* name = "name";
            inline static const char* childrenStart = "children_start";
            inline static const char* childrenEnd = "children_end";
        };

        const unsigned int id;
        std::string name;
        Node* parent = nullptr;
        std::vector<Node*> children;
        NodePath nodePath;

        static unsigned int getUniqueId();
        void freeUniqueId(unsigned int id);

    public:
        struct SerializationSymbols
        {
            inline static const char* header = "Node";
            inline static const char* type = "type";
            inline static const char* childrenStart = "children_start";
            inline static const char* childrenEnd = "children_end";
        };

        Node();
        Node(std::string name);
        virtual ~Node();

        static Node* createNode(std::string type);
        static std::vector<std::string> getAllNodeTypes();

        inline uint getId() const { return id; }

        virtual void start();
        virtual void update(float deltaTime);
        virtual void draw(Renderer& renderer);

        virtual glm::vec2 getPosition() const;
        virtual float getRotation() const;
        virtual glm::vec2 getScale() const;
        virtual glm::vec2 getPivot() const;
        virtual glm::vec2 getGlobalPosition() const;
        virtual float getGlobalRotation() const;
        virtual glm::vec2 getGlobalScale() const;
        virtual glm::vec2 forward() const;
        virtual glm::vec2 backward() const;
        virtual glm::vec2 left() const;
        virtual glm::vec2 right() const;
        virtual void setPosition(glm::vec2 v);
        virtual void setRotation(float angle);
        virtual void setScale(float s);
        virtual void setScale(glm::vec2 s);
        virtual void setPivot(glm::vec2 pivot);
        virtual void setGlobalPosition(glm::vec2 v);
        virtual void setGlobalRotation(float angle);
        virtual void setGlobalScale(float s);
        virtual void setGlobalScale(glm::vec2 s);

        virtual void addChild(Node* node);
        virtual void addChildren(const std::vector<Node*>& children);
        virtual void insertBefore(Node* node);
        virtual void insertAfter(Node* node);
        virtual void addSibling(Node* node);
        virtual void removeChild(Node* node);
        virtual bool hasChild(Node* node);
        virtual void orphanize();
        virtual void startChildren();
        virtual void drawChildren(Renderer& renderer);
        virtual void updateChildren(float deltaTime);
        virtual const std::vector<Node*>& getChildren() const;
        virtual const Node* getParent() const;

        static Node* deserialize(const std::string& nodeStr);
        virtual std::string serialize(bool withChildren = true) const;
        std::string serializeChildren() const;
        virtual void restore(NodeValues& nodeValues);

        NodePath getNodePath() const;
        std::string getName() const;
        void setName(std::string name);
        Node* findChild(std::string name) const;
        virtual void renderFields(SceneEditor* sceneEditor);
        Node* clone() const;

        template<class T>
        T* findChild(std::string name) const;

    };

}

#include "node.tpp"

#endif // __NODE_HPP__