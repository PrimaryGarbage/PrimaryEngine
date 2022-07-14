#ifndef __UI_HPP__
#define __UI_HPP__

class GLFWwindow;

namespace prim
{

    class UI
    {
    public:
        static void init(GLFWwindow* window);
        static void draw();
    };
}

#endif // __UI_HPP__