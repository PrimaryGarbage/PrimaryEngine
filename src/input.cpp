#include "input.hpp"

KeyInfo Input::keys[200]{};
MouseInfo Input::mouse;

void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	keys[key].just = action != keys[key].pressed;
	keys[key].pressed = action == GLFW_PRESS;
}

void Input::char_callback(GLFWwindow* window, unsigned int codepoint)
{
	
}

void Input::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	mouse.x = xpos;
	mouse.y = ypos;
}

void Input::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	mouse.buttons[button].just = action != mouse.buttons[button].pressed;
	mouse.buttons[button].pressed = action == GLFW_PRESS;
}

void Input::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	mouse.scrollDeltaX = xoffset;
	mouse.scrollDeltaY = yoffset;
}


void Input::init(GLFWwindow* window)
{
	glfwSetKeyCallback(window, key_callback);
	glfwSetCharCallback(window, char_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
}