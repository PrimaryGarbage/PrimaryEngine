#include "input.hpp"

KeyInfo Input::keys[200]{};
MouseInfo Input::mouse;
std::vector<Gamepad> Input::gamepads;

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

void Input::joystick_callback(int jid, int event)
{
	if (event == GLFW_CONNECTED)
    {
		if(glfwJoystickIsGamepad(jid))
		{
			auto gamepad = std::find(gamepads.begin(), gamepads.end(), [jid](const Gamepad& gp) -> bool { return gp.id == jid; });
			if(gamepad != gamepads.end()) return;		// gamepad is already registered
			gamepads.push_back(Gamepad(jid));
		}
    }
    else if (event == GLFW_DISCONNECTED)
    {
		if(glfwJoystickIsGamepad(jid))
		{
			auto gamepad = std::find_if(gamepads.begin(), gamepads.end(), [jid](const Gamepad& gp) -> bool { return gp.id == jid; });
			if(gamepad == gamepads.end()) return;		// gamepad is already unregistered
			gamepads.erase(gamepad);
		}
    }
}

void Input::init(GLFWwindow* window)
{
	glfwSetKeyCallback(window, key_callback);
	glfwSetCharCallback(window, char_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
}

// should be called from mainLoop
void Input::update()
{
	for(Gamepad& gamepad : gamepads)
	{
		GLFWgamepadstate state;
		glfwGetGamepadState(gamepad.id, &state);
		for(int i = 0; i < Gamepad::buttonCount; ++i)
		{
			gamepad.buttons[i].just = state.buttons[i] != gamepad.buttons[i].pressed;
			gamepad.buttons[i].pressed = state.buttons[i] == GLFW_PRESS;
		}
		for(int i = 0; i < Gamepad::axisCount; ++i)
		{
			gamepad.axes[i] = state.axes[i];
		}
	}
}

bool Input::keyPressed(Key key)
{
	return keys[static_cast<int>(key)].pressed;
}

bool Input::keyJustPressed(Key key)
{
	return keys[static_cast<int>(key)].pressed && keys[static_cast<int>(key)].just;
}

bool Input::keyJustReleased(Key key)
{
	return !keys[static_cast<int>(key)].pressed && keys[static_cast<int>(key)].just;
}

bool Input::gamepadButtonPressed(int gamepadId, GamepadButton button)
{
	return gamepads[gamepadId].buttons[static_cast<int>(button)].pressed;
}

bool Input::gamepadButtonJustPressed(int gamepadId, GamepadButton button)
{
	return gamepads[gamepadId].buttons[static_cast<int>(button)].pressed && gamepads[gamepadId].buttons[static_cast<int>(button)].just;
}

bool Input::gamepadButtonJustReleased(int gamepadId, GamepadButton button)
{
	return !gamepads[gamepadId].buttons[static_cast<int>(button)].pressed && gamepads[gamepadId].buttons[static_cast<int>(button)].just;
}

float Input::gamepadAxis(int gamepadId, GamepadAxis axis)
{
	return gamepads[gamepadId].axes[static_cast<int>(axis)];
}