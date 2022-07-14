#ifndef __UI_HPP__
#define __UI_HPP__

class GLFWwindow;

namespace prim
{

    class UI
    {
        GLFWwindow* window;
    public:
        void init(GLFWwindow* window);
        void draw();
    };
}

#endif // __UI_HPP__