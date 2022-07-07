#ifndef __INPUT_HPP__
#define __INPUT_HPP__

#include "GLFW/glfw3.h"
#include "prim_exception.hpp"
#include "input_util.hpp"
#include <vector>

class Input final
{
private:
	static const int keysCount = 350;
	static PressInfo keys[keysCount];
	static MouseInfo mouse;
	static std::vector<Gamepad> gamepads;
	static std::vector<Action> actions;
	static std::vector<Axis> axes;
	static std::string charInput;	// utf8 text

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods); 
	static void char_callback(GLFWwindow* window, unsigned int codepoint);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void joystick_callback(int jid, int event);
	static void createDefaultActionsAndAxes();
	static void insertCodepointIntoString(unsigned int codepoint, std::string& str);
	static void registerAllGamepads();
public:
	static void init(GLFWwindow* window);
	static void update();
	
	static bool isPressed(const Key key);
	static bool isPressed(const MouseButton button);
	static bool isPressed(const GamepadButton button, const int gamepadId = 0);
	static bool isPressed(const std::string actionName);
	static bool isJustPressed(const Key key);
	static bool isJustPressed(const MouseButton button);
	static bool isJustPressed(const GamepadButton button, const int gamepadId = 0);
	static bool isJustPressed(const std::string actionName);
	static bool isJustReleased(const Key key);
	static bool isJustReleased(const MouseButton button);
	static bool isJustReleased(const GamepadButton button, const int gamepadId = 0);
	static bool isJustReleased(const std::string actionName);

	static float getAxis(const GamepadAxis axis, const int gamepadId = 0);
	static float getAxis(const std::pair<Key, Key> keys);
	static float getAxis(const std::pair<MouseButton, MouseButton> buttons);
	static float getAxis(const std::pair<GamepadButton, GamepadButton> buttons);
	static float getAxis(const std::string axisName);

	static void addAction(const std::string name, std::initializer_list<ActionCause> actionCauses);
	static void addAxis(const std::string name, std::initializer_list<AxisCause> axisCauses);
	static void removeAction(const std::string name);
	static void removeAxis(const std::string name);

	static std::string getChars();
};

#endif // __INPUT_HPP__