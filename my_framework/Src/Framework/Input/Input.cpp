#include "../../../framework.h"
#include "../../../environment.h"

std::function<bool(std::vector<int>& inputs)> Input::funcTrg = Input::BothDevTrg;
std::function<bool(std::vector<int>& inputs)> Input::funcOn = Input::BothDevOn;
std::function<bool(std::vector<int>& inputs)> Input::funcRel = Input::BothDevRel;


bool Input::Trg(std::vector<int>& inputs) {
	bool flag = false;
	return funcTrg(inputs);
}
bool Input::On(std::vector<int>& inputs) {
	return funcOn(inputs);
}
bool Input::Rel(std::vector<int>& inputs) {
	return funcRel(inputs);
}

//処理部分
bool Input::BothDevTrg(std::vector<int>& inputs) {
	for (auto& i : inputs) {
		if (Keyboard::Trg(i)) return true;
		if (Joystick::Trg(i)) return true;
		if (Joystick::PovTrg(i)) return true;
		if (Joystick::StickTrg(i)) return true;
	}
	return false;
}
bool Input::BothDevOn(std::vector<int>& inputs) {
	for (auto& i : inputs) {
		if (Keyboard::On(i)) return true;
		if (Joystick::On(i)) return true;
		if (Joystick::PovOn(i)) return true;
		if (Joystick::StickOn(i)) return true;
	}
	return false;
}
bool Input::BothDevRel(std::vector<int>& inputs) {
	for (auto& i : inputs) {
		if (Keyboard::Rel(i)) return true;
		if (Joystick::Rel(i)) return true;
		if (Joystick::PovRel(i)) return true;
		if (Joystick::StickRel(i)) return true;
	}
	return false;
}

bool Input::KeyboardDevTrg(std::vector<int>& inputs) {
	for (auto& i : inputs) {
		if (Keyboard::Trg(i)) return true;
	}
	return false;
}
bool Input::KeyboardDevOn(std::vector<int>& inputs) {
	for (auto& i : inputs) {
		if (Keyboard::On(i)) return true;
	}
	return false;
}
bool Input::KeyboardDevRel(std::vector<int>& inputs) {
	for (auto& i : inputs) {
		if (Keyboard::Rel(i)) return true;
	}
	return false;
}

bool Input::JoystickDevTrg(std::vector<int>& inputs) {
	for (auto& i : inputs) {
		if (Joystick::Trg(i)) return true;
		if (Joystick::PovTrg(i)) return true;
	}
	return false;
}
bool Input::JoystickDevOn(std::vector<int>& inputs) {
	for (auto& i : inputs) {
		if (Joystick::On(i)) return true;
		if (Joystick::PovOn(i)) return true;
	}
	return false;
}
bool Input::JoystickDevRel(std::vector<int>& inputs) {
	for (auto& i : inputs) {
		if (Joystick::Rel(i)) return true;
		if (Joystick::PovRel(i)) return true;
	}
	return false;
}

//デバイスの切り替え
void Input::SetKeyboardDevice() {
	funcTrg = KeyboardDevTrg;
	funcOn = KeyboardDevOn;
	funcRel = KeyboardDevRel;
}
void Input::SetJoystickDevice() {
	funcTrg = JoystickDevTrg;
	funcOn = JoystickDevOn;
	funcRel = JoystickDevRel;
}
void Input::SetBothDevice() {
	funcTrg = BothDevTrg;
	funcOn = BothDevOn;
	funcRel = BothDevRel;
}