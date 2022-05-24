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
			auto gamepad = std::find_if(gamepads.begin(), gamepads.end(), [jid](const Gamepad& gp) -> bool { return gp.id == jid; });
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

bool Input::isPressed(const Key key)
{
	return keys[static_cast<int>(key)].pressed;
}

bool Input::isPressed(const MouseButton button)
{
	return mouse.buttons[static_cast<int>(button)].pressed;
}

bool Input::isPressed(const GamepadButton button, const int gamepadId)
{
	if(gamepads.size() < gamepadId + 1) throw PRIM_EXCEPTION("Requested gamepad doesn't exist! GamepadID = " + gamepadId);
	return gamepads[gamepadId].buttons[static_cast<int>(button)].pressed;
}

bool Input::isPressed(const std::string actionName)
{
	auto action = std::find_if(actions.begin(), actions.end(), [&actionName](const Action& a) { return actionName == a.name; });
	if(action == actions.end()) throw PRIM_EXCEPTION("Input action wasn't found! Action name: " + actionName);
	bool result = false;
	static auto pressedVisitor = [](const auto& actionCause) -> bool { return isPressed(actionCause); };
	for(const ActionCause& a : action->associatedButtons)
	{
		result = result || std::visit(pressedVisitor, a);
		if(result) break;
	}

	return result;
}

bool Input::isJustPressed(const Key key)
{
	return keys[static_cast<int>(key)].pressed && keys[static_cast<int>(key)].just;
}

bool Input::isJustPressed(const MouseButton button)
{
	return mouse.buttons[static_cast<int>(button)].pressed && mouse.buttons[static_cast<int>(button)].just;
}

bool Input::isJustPressed(const GamepadButton button, const int gamepadId)
{
	if(gamepads.size() < gamepadId + 1) throw PRIM_EXCEPTION("Requested gamepad doesn't exist! GamepadID = " + gamepadId);
	return gamepads[gamepadId].buttons[static_cast<int>(button)].pressed && gamepads[gamepadId].buttons[static_cast<int>(button)].just;
}

bool Input::isJustPressed(const std::string actionName)
{
	auto action = std::find_if(actions.begin(), actions.end(), [&actionName](const Action& a) { return actionName == a.name; });
	if(action == actions.end()) throw PRIM_EXCEPTION("Input action wasn't found! Action name: " + actionName);
	bool result = false;
	static auto pressedVisitor = [](const auto& actionCause) -> bool { return isJustPressed(actionCause); };
	for(const ActionCause& a : action->associatedButtons)
	{
		result = result || std::visit(pressedVisitor, a);
		if(result) break;
	}

	return result;
}

bool Input::isJustReleased(const Key key)
{
	return !keys[static_cast<int>(key)].pressed && keys[static_cast<int>(key)].just;
}

bool Input::isJustReleased(const MouseButton button)
{
	return !mouse.buttons[static_cast<int>(button)].pressed && mouse.buttons[static_cast<int>(button)].just;
}

bool Input::isJustReleased(const GamepadButton button, const int gamepadId)
{
	if(gamepads.size() < gamepadId + 1) throw PRIM_EXCEPTION("Requested gamepad doesn't exist! GamepadID = " + gamepadId);
	return !gamepads[gamepadId].buttons[static_cast<int>(button)].pressed && gamepads[gamepadId].buttons[static_cast<int>(button)].just;
}

bool Input::isJustReleased(const std::string actionName)
{
	auto action = std::find_if(actions.begin(), actions.end(), [&actionName](const Action& a) { return actionName == a.name; });
	if(action == actions.end()) throw PRIM_EXCEPTION("Input action wasn't found! Action name: " + actionName);
	bool result = false;
	static auto pressedVisitor = [](const auto& actionCause) -> bool { return isJustReleased(actionCause); };
	for(const ActionCause& a : action->associatedButtons)
	{
		result = result || std::visit(pressedVisitor, a);
		if(result) break;
	}

	return result;
}

float Input::getAxis(const GamepadAxis axis, const int gamepadId)
{
	if(gamepadId > gamepads.size() - 1) throw PRIM_EXCEPTION("Inexistent gamepadId! GamepadId = " + gamepadId);
	return gamepads[gamepadId].axes[static_cast<int>(axis)];
}

float Input::getAxis(const std::pair<Key, Key> keys)
{
	return static_cast<float>(isPressed(keys.first)) - static_cast<float>(isPressed(keys.second));
}

float Input::getAxis(const std::pair<MouseButton, MouseButton> buttons)
{
	return static_cast<float>(isPressed(buttons.first)) - static_cast<float>(isPressed(buttons.second));
}

float Input::getAxis(const std::pair<GamepadButton, GamepadButton> buttons)
{
	return static_cast<float>(isPressed(buttons.first)) - static_cast<float>(isPressed(buttons.second));
}

float Input::getAxis(const std::string axisName)
{
	auto axis = std::find_if(axes.begin(), axes.end(), [&axisName](const Axis& a) { return axisName == a.name; });
	if(axis == axes.end()) throw PRIM_EXCEPTION("Input axis wasn't found! Axis name: " + axisName);
	float result = 0.0f;
	static auto axisVisitor = [](const auto& axisCause) -> bool { return getAxis(axisCause); };
	for(const AxisCause& a : axis->associatedAxes)
	{
		result += std::visit(axisVisitor, a);
		if(result) break;
	}

	return result;
}

void Input::addAction(const std::string name, std::initializer_list<ActionCause> actionCauses)
{
	auto actionSearch = std::find_if(actions.begin(), actions.end(), [&name](const Action& a) { return a.name == name; });
	if(actionSearch != actions.end())
	{
		// throw PRIM_EXCEPTION("Action with name '" + name + "' aleady exists!");
		actionSearch->associatedButtons.clear();
		actionSearch->associatedButtons.insert(actionSearch->associatedButtons.begin(), actionCauses);
	}
	else
	{
		actions.push_back({ name.c_str(), actionCauses });
	} 
}

void Input::addAxis(const std::string name, std::initializer_list<AxisCause> axisCauses)
{
	auto axisSearch = std::find_if(axes.begin(), axes.end(), [&name](const Axis& a) { return a.name == name; });
	if(axisSearch != axes.end())
	{
		// throw PRIM_EXCEPTION("Axis with name '" + name + "' aleady exists!");
		axisSearch->associatedAxes.clear();
		axisSearch->associatedAxes.insert(axisSearch->associatedAxes.begin(), axisCauses);
	}
	else
	{
		axes.push_back({ name.c_str(), axisCauses });
	} 
}

void Input::removeAction(const std::string name)
{
	auto actionSearch = std::find_if(actions.begin(), actions.end(), [&name](const Action& a) { return a.name == name; });
	if(actionSearch != actions.end())
	{
		actions.erase(actionSearch);
	}
	else
	{
		// throw PRIM_EXCEPTION("Action with name '" + name + "' wasn't found!");
	} 
}

void Input::removeAxis(const std::string name)
{
	auto axisSearch = std::find_if(axes.begin(), axes.end(), [&name](const Axis& a) { return a.name == name; });
	if(axisSearch != axes.end())
	{
		axes.erase(axisSearch);
	}
	else
	{
		// throw PRIM_EXCEPTION("Axis with name '" + name + "' wasn't found!");
	} 
}