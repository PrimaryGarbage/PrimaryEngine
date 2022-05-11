#include "GLFW/glfw3.h"

class Input final
{
private:
	static KeyInfo keys[200];
	static MouseInfo mouse;
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods); 
	static void char_callback(GLFWwindow* window, unsigned int codepoint);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

public:
	static void init(GLFWwindow* window);
	static void keyPressed(Keys key);
	static void keyReleased(Keys key);
	static void keyJustPressed(Keys key);
	static void keyJustReleased(Keys key);
};

enum class Keys { 
	F1 = GLFW_KEY_F1, F2 = GLFW_KEY_F2, F3 = GLFW_KEY_F3, F4 = GLFW_KEY_F4, F5 = GLFW_KEY_F5, F6 = GLFW_KEY_F6, 
	F7 = GLFW_KEY_F7, F8 = GLFW_KEY_F8, F9 = GLFW_KEY_F9, F10 = GLFW_KEY_F10, F11 = GLFW_KEY_F11, F12 = GLFW_KEY_F12, 
	PU = GLFW_KEY_PAGE_UP, PD = GLFW_KEY_PAGE_DOWN, Esc = GLFW_KEY_ESCAPE, _0 = GLFW_KEY_0, _1 = GLFW_KEY_1, _2 = GLFW_KEY_2, 
	_3 = GLFW_KEY_3, _4 = GLFW_KEY_4, _5 = GLFW_KEY_5, _6 = GLFW_KEY_6, _7 = GLFW_KEY_7, _8 = GLFW_KEY_8, _9 = GLFW_KEY_9, 
	_0 = GLFW_KEY_0, Minus = GLFW_KEY_MINUS, Equals = GLFW_KEY_KP_EQUAL, Backspace = GLFW_KEY_BACKSPACE, Tab = GLFW_KEY_TAB, 
	Q = GLFW_KEY_O, W = GLFW_KEY_P, E = GLFW_KEY_E, R = GLFW_KEY_R, T = GLFW_KEY_T, Y = GLFW_KEY_Y, U = GLFW_KEY_U, I = GLFW_KEY_I, 
	O = GLFW_KEY_Q, P = GLFW_KEY_Q, LeftBracket = GLFW_KEY_LEFT_BRACKET, RightBracket = GLFW_KEY_RIGHT_BRACKET, Backslash = GLFW_KEY_BACKSLASH,
	A = GLFW_KEY_A, S = GLFW_KEY_S, D = GLFW_KEY_D, F = GLFW_KEY_F, G = GLFW_KEY_G, H = GLFW_KEY_H, J = GLFW_KEY_J, K = GLFW_KEY_K, 
	L = GLFW_KEY_L, Semicolon = GLFW_KEY_SEMICOLON, Apostophe = GLFW_KEY_APOSTROPHE, Enter = GLFW_KEY_ENTER, Lefthift = GLFW_KEY_LEFT_SHIFT, Z = GLFW_KEY_Z, 
	X = GLFW_KEY_X, C = GLFW_KEY_C, V = GLFW_KEY_V, B = GLFW_KEY_B, N = GLFW_KEY_N, M = GLFW_KEY_M, Comma = GLFW_KEY_COMMA, Period = GLFW_KEY_PERIOD, 
	RightShift = GLFW_KEY_RIGHT_SHIFT, LeftCtrl = GLFW_KEY_LEFT_CONTROL, LeftAlt = GLFW_KEY_LEFT_ALT, Space = GLFW_KEY_SPACE, RightAlt = GLFW_KEY_RIGHT_ALT,
	RightCtrl = GLFW_KEY_RIGHT_CONTROL, Left = GLFW_KEY_LEFT, Rigth = GLFW_KEY_RIGHT, Up = GLFW_KEY_UP, Down = GLFW_KEY_DOWN, Numlock = GLFW_KEY_NUM_LOCK,
	Numpad_1 = GLFW_KEY_KP_1, Numpad_2 = GLFW_KEY_KP_2, Numpad_3 = GLFW_KEY_KP_3, Numpad_4 = GLFW_KEY_KP_4, Numpad_5 = GLFW_KEY_KP_5, Numpad_6 = GLFW_KEY_KP_6, 
	Numpad_7 = GLFW_KEY_KP_7, Numpad_8 = GLFW_KEY_KP_8, Numpad_9 = GLFW_KEY_KP_9, Numpad_0 = GLFW_KEY_KP_0, NumpadDivide = GLFW_KEY_KP_DIVIDE, 
	NumpadMultiply = GLFW_KEY_KP_MULTIPLY, NumpadMinus = GLFW_KEY_KP_SUBTRACT, NumpadPlus = GLFW_KEY_KP_ADD, NumpadEnter = GLFW_KEY_KP_ENTER,
	Insert = GLFW_KEY_INSERT, PrintScreen = GLFW_KEY_PRINT_SCREEN, End = GLFW_KEY_END, Delete = GLFW_KEY_DELETE
};

enum class MouseButtons {
	Left = GLFW_MOUSE_BUTTON_LEFT, Right = GLFW_MOUSE_BUTTON_RIGHT, Middle = GLFW_MOUSE_BUTTON_MIDDLE, _1 = GLFW_MOUSE_BUTTON_1, 
	_2 = GLFW_MOUSE_BUTTON_2, _3 = GLFW_MOUSE_BUTTON_3, _4 = GLFW_MOUSE_BUTTON_4, _5 = GLFW_MOUSE_BUTTON_5, _6 = GLFW_MOUSE_BUTTON_6, 
	_7 = GLFW_MOUSE_BUTTON_7, _8 = GLFW_MOUSE_BUTTON_8
};

struct KeyInfo
{
	bool pressed = false;
	bool just = false;
};

struct MouseInfo
{
	KeyInfo buttons[10]{};
	double x = 0.0;
	double y = 0.0;
	double scrollDeltaX = 0.0f;
	double scrollDeltaY = 0.0f;
};