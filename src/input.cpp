#include "input.hpp"
#include <algorithm>

PressInfo Input::keys[200]{};
MouseInfo Input::mouse;
std::vector<Gamepad> Input::gamepads;
std::vector<Action> Input::actions;
std::vector<Axis> Input::axes;

void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	keys[key].just = action != keys[key].pressed;
	keys[key].pressed = action == GLFW_PRESS;
}

void Input::char_callback(GLFWwindow* window, unsigned int codepoint)
{
	// TO IMPLEMENT!
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

void Input::createDefaultActionsAndAxes()
{
	// TO IMPLEMENT!
}

void Input::init(GLFWwindow* window)
{
	glfwSetKeyCallback(window, key_callback);
	glfwSetCharCallback(window, char_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);

	createDefaultActionsAndAxes();
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

bool Input::isPressed(Key key)
{
	return keys[static_cast<int>(key)].pressed;
}

bool Input::isPressed(MouseButton button)
{
	return mouse.buttons[static_cast<int>(button)].pressed;
}

bool Input::isPressed(int gamepadId, GamepadButton button)
{
	return gamepads[gamepadId].buttons[static_cast<int>(button)].pressed;
}

bool Input::isPressed(std::string actionName)
{
	auto action = std::find_if(actions.begin(), actions.end(), [&actionName](const Action& a) { return actionName == a.name; });
	if(action == actions.end()) throw PRIM_EXCEPTION("Input action wasn't found! Action name: " + actionName);
	bool result = false;
	static auto pressedVisitor = [](const auto& actionCause) -> bool { isPressed(actionCause); };
	for(const ActionCause& a : action->associatedButtons)
	{
		result = result || std::visit(pressedVisitor, a);
	}
}

bool Input::isJustPressed(Key key)
{
	return keys[static_cast<int>(key)].pressed && keys[static_cast<int>(key)].just;
}

bool Input::isJustPressed(MouseButton button)
{
	return mouse.buttons[static_cast<int>(button)].pressed && mouse.buttons[static_cast<int>(button)].just;
}

bool Input::isJustPressed(int gamepadId, GamepadButton button)
{
	return gamepads[gamepadId].buttons[static_cast<int>(button)].pressed && gamepads[gamepadId].buttons[static_cast<int>(button)].just;
}

bool Input::isJustPressed(std::string actionName)
{
	// TO IMPLEMENT!
}

bool Input::isJustReleased(Key key)
{
	return !keys[static_cast<int>(key)].pressed && keys[static_cast<int>(key)].just;
}

bool Input::isJustReleased(MouseButton button)
{
	return !mouse.buttons[static_cast<int>(button)].pressed && mouse.buttons[static_cast<int>(button)].just;
}

bool Input::isJustReleased(int gamepadId, GamepadButton button)
{
	return !gamepads[gamepadId].buttons[static_cast<int>(button)].pressed && gamepads[gamepadId].buttons[static_cast<int>(button)].just;
}

bool Input::isJustReleased(std::string actionName)
{
	// TO IMPLEMENT!
}

float Input::getGamepadAxis(int gamepadId, GamepadAxis axis)
{
	return gamepads[gamepadId].axes[static_cast<int>(axis)];
}

float Input::getAxis(std::string axisName)
{
	// TO IMPLEMENT!
}

void Input::addAction(std::string name, std::initializer_list<ActionCause> actionCauses)
{
	// TO IMPLEMENT!
}

void Input::addAxis(std::string name, std::initializer_list<AxisCause> axisCauses)
{
	// TO IMPLEMENT!
}

void Input::removeAction(std::string name)
{
	// TO IMPLEMENT!
}

void Input::removeAxis(std::string name)
{
	// TO IMPLEMENT!
}