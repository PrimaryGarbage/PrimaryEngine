#ifndef __INPUT_UTIL_HPP__
#define __INPUT_UTIL_HPP__

#include <vector>
#include <variant>

enum class Key { 
	f1 = GLFW_KEY_F1, f2 = GLFW_KEY_F2, f3 = GLFW_KEY_F3, f4 = GLFW_KEY_F4, f5 = GLFW_KEY_F5, f6 = GLFW_KEY_F6, 
	f7 = GLFW_KEY_F7, f8 = GLFW_KEY_F8, f9 = GLFW_KEY_F9, f10 = GLFW_KEY_F10, f11 = GLFW_KEY_F11, f12 = GLFW_KEY_F12, 
	pageUp = GLFW_KEY_PAGE_UP, pageDown = GLFW_KEY_PAGE_DOWN, esc = GLFW_KEY_ESCAPE, _0 = GLFW_KEY_0, _1 = GLFW_KEY_1, _2 = GLFW_KEY_2, 
	_3 = GLFW_KEY_3, _4 = GLFW_KEY_4, _5 = GLFW_KEY_5, _6 = GLFW_KEY_6, _7 = GLFW_KEY_7, _8 = GLFW_KEY_8, _9 = GLFW_KEY_9, 
	minus = GLFW_KEY_MINUS, equals = GLFW_KEY_KP_EQUAL, backspace = GLFW_KEY_BACKSPACE, tab = GLFW_KEY_TAB, 
	q = GLFW_KEY_Q, w = GLFW_KEY_W, e = GLFW_KEY_E, r = GLFW_KEY_R, t = GLFW_KEY_T, y = GLFW_KEY_Y, u = GLFW_KEY_U, i = GLFW_KEY_I, 
	o = GLFW_KEY_O, p = GLFW_KEY_P, leftBracket = GLFW_KEY_LEFT_BRACKET, rightBracket = GLFW_KEY_RIGHT_BRACKET, backslash = GLFW_KEY_BACKSLASH,
	a = GLFW_KEY_A, s = GLFW_KEY_S, d = GLFW_KEY_D, f = GLFW_KEY_F, g = GLFW_KEY_G, h = GLFW_KEY_H, j = GLFW_KEY_J, k = GLFW_KEY_K, 
	l = GLFW_KEY_L, semicolon = GLFW_KEY_SEMICOLON, apostophe = GLFW_KEY_APOSTROPHE, enter = GLFW_KEY_ENTER, leftShift = GLFW_KEY_LEFT_SHIFT, z = GLFW_KEY_Z, 
	x = GLFW_KEY_X, c = GLFW_KEY_C, v = GLFW_KEY_V, b = GLFW_KEY_B, n = GLFW_KEY_N, m = GLFW_KEY_M, comma = GLFW_KEY_COMMA, period = GLFW_KEY_PERIOD, 
	rightShift = GLFW_KEY_RIGHT_SHIFT, leftCtrl = GLFW_KEY_LEFT_CONTROL, leftAlt = GLFW_KEY_LEFT_ALT, space = GLFW_KEY_SPACE, rightAlt = GLFW_KEY_RIGHT_ALT,
	rightCtrl = GLFW_KEY_RIGHT_CONTROL, left = GLFW_KEY_LEFT, right = GLFW_KEY_RIGHT, up = GLFW_KEY_UP, down = GLFW_KEY_DOWN, numlock = GLFW_KEY_NUM_LOCK,
	numpad_1 = GLFW_KEY_KP_1, numpad_2 = GLFW_KEY_KP_2, numpad_3 = GLFW_KEY_KP_3, numpad_4 = GLFW_KEY_KP_4, numpad_5 = GLFW_KEY_KP_5, numpad_6 = GLFW_KEY_KP_6, 
	numpad_7 = GLFW_KEY_KP_7, numpad_8 = GLFW_KEY_KP_8, numpad_9 = GLFW_KEY_KP_9, numpad_0 = GLFW_KEY_KP_0, numpadDivide = GLFW_KEY_KP_DIVIDE, 
	numpadMultiply = GLFW_KEY_KP_MULTIPLY, numpadMinus = GLFW_KEY_KP_SUBTRACT, numpadPlus = GLFW_KEY_KP_ADD, numpadEnter = GLFW_KEY_KP_ENTER,
	insert = GLFW_KEY_INSERT, printScreen = GLFW_KEY_PRINT_SCREEN, end = GLFW_KEY_END, del = GLFW_KEY_DELETE
};

enum class MouseButton {
	left = GLFW_MOUSE_BUTTON_LEFT, right = GLFW_MOUSE_BUTTON_RIGHT, middle = GLFW_MOUSE_BUTTON_MIDDLE, _1 = GLFW_MOUSE_BUTTON_1, _2 = GLFW_MOUSE_BUTTON_2,
	_3 = GLFW_MOUSE_BUTTON_3, _4 = GLFW_MOUSE_BUTTON_4, _5 = GLFW_MOUSE_BUTTON_5, _6 = GLFW_MOUSE_BUTTON_6, _7 = GLFW_MOUSE_BUTTON_7, _8 = GLFW_MOUSE_BUTTON_8
};

enum class GamepadButton {
	a = GLFW_GAMEPAD_BUTTON_A, b = GLFW_GAMEPAD_BUTTON_B, x = GLFW_GAMEPAD_BUTTON_X, y = GLFW_GAMEPAD_BUTTON_Y, leftBumper = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,
	rightBumper = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER, back = GLFW_GAMEPAD_BUTTON_BACK, start = GLFW_GAMEPAD_BUTTON_START, guide = GLFW_GAMEPAD_BUTTON_GUIDE, 
	leftThumb = GLFW_GAMEPAD_BUTTON_LEFT_THUMB, rightThumb = GLFW_GAMEPAD_BUTTON_RIGHT_THUMB, dpadUp = GLFW_GAMEPAD_BUTTON_DPAD_UP, dpadRight = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,
	dpadDown = GLFW_GAMEPAD_BUTTON_DPAD_DOWN, dpadLeft = GLFW_GAMEPAD_BUTTON_DPAD_LEFT, buttonCount
};
 
enum class GamepadAxis {
	LeftX = GLFW_GAMEPAD_AXIS_LEFT_X, LeftY = GLFW_GAMEPAD_AXIS_LEFT_Y, RightX = GLFW_GAMEPAD_AXIS_RIGHT_X, RightY = GLFW_GAMEPAD_AXIS_RIGHT_Y,
	LeftTrigger = GLFW_GAMEPAD_AXIS_LEFT_TRIGGER, RightTrigger = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER, axisCount = 6
};

struct PressInfo
{	
	bool pressed = false;
	bool just = false;
};

struct MouseInfo
{
	static const int buttonCount = 11;

	PressInfo buttons[buttonCount]{};
	double x = 0.0;
	double y = 0.0;
	double scrollDeltaX = 0.0f;
	double scrollDeltaY = 0.0f;
};

struct Gamepad
{
	static const int buttonCount = static_cast<int>(GamepadButton::buttonCount);
	static const int axisCount = static_cast<int>(GamepadAxis::axisCount);

	int id;
	std::string name;
	PressInfo buttons[buttonCount];
	float axes[axisCount] {};

	Gamepad(int id): id(id), name(glfwGetGamepadName(id)) {}
}; 

using ActionCause = std::variant<Key, MouseButton, GamepadButton>;
using AxisCause = std::variant<GamepadAxis, std::pair<Key, Key>, std::pair<MouseButton, MouseButton>, std::pair<GamepadButton, GamepadButton>>;

struct Action
{
	std::string name;
	std::vector<ActionCause> associatedButtons;
};

struct Axis
{
	std::string name;
	std::vector<AxisCause> associatedAxes;
};

#endif // __INPUT_UTIL_HPP__