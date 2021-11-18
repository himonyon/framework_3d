#include "../../../framework.h"
#include "../../../environment.h"

std::function<bool(int(&inputs)[2])> Input::funcTrg = Input::BothDevTrg;
std::function<bool(int(&inputs)[2])> Input::funcOn = Input::BothDevOn;
std::function<bool(int(&inputs)[2])> Input::funcRel = Input::BothDevRel;


bool Input::Trg(int(&inputs)[2]) {
	bool flag = false;
	return funcTrg(inputs);
}
bool Input::On(int(&inputs)[2]) {
	return funcOn(inputs);
}
bool Input::Rel(int(&inputs)[2]) {
	return funcRel(inputs);
}

//処理部分
bool Input::BothDevTrg(int(&inputs)[2]) {
	if (Keyboard::Trg(inputs[0])) return true;
	if (Joystick::Trg(inputs[1])) return true;
	return false;
}
bool Input::BothDevOn(int(&inputs)[2]) {
	if (Keyboard::On(inputs[0])) return true;
	if (Joystick::On(inputs[1])) return true;
	return false;
}
bool Input::BothDevRel(int(&inputs)[2]) {
	if (Keyboard::Rel(inputs[0])) return true;
	if (Joystick::Rel(inputs[1])) return true;
	return false;
}

bool Input::KeyboardDevTrg(int(&inputs)[2]) {
	if (Keyboard::Trg(inputs[0])) return true;
	return false;
}
bool Input::KeyboardDevOn(int(&inputs)[2]) {
	if (Keyboard::On(inputs[0])) return true;
	return false;
}
bool Input::KeyboardDevRel(int(&inputs)[2]) {
	if (Keyboard::Rel(inputs[0])) return true;
	return false;
}

bool Input::JoystickDevTrg(int(&inputs)[2]) {
	if (Joystick::Trg(inputs[1])) return true;
	return false;
}
bool Input::JoystickDevOn(int(&inputs)[2]) {
	if (Joystick::On(inputs[1])) return true;
	return false;
}
bool Input::JoystickDevRel(int(&inputs)[2]) {
	if (Joystick::Rel(inputs[1])) return true;
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