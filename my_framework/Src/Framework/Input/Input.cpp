#include "../../../framework.h"
#include "../../../environment.h"

using namespace MyFrameWork;

eDeviceType Input::currentDevice = eDeviceType::Both;
std::function<bool(std::vector<int>*& inputs)> Input::funcTrg = Input::BothDevTrg;
std::function<bool(std::vector<int>*& inputs)> Input::funcOn = Input::BothDevOn;
std::function<bool(std::vector<int>*& inputs)> Input::funcRel = Input::BothDevRel;
std::function<float(std::vector<int>*& inputs)> Input::funcDX = Input::BothDevDX;
std::function<float(std::vector<int>*& inputs)> Input::funcDY = Input::BothDevDY;


float Input::GetDX(std::vector<int>*& inputs) {
	return funcDX(inputs);
}
float Input::GetDY(std::vector<int>*& inputs) {
	return funcDY(inputs);
}
bool Input::Trg(std::vector<int>*& inputs) {
	return funcTrg(inputs);
}
bool Input::On(std::vector<int>*& inputs) {
	return funcOn(inputs);
}
bool Input::Rel(std::vector<int>*& inputs) {
	return funcRel(inputs);
}

//処理部分
bool Input::BothDevTrg(std::vector<int>*& inputs) {
	for (auto& i : *inputs) {
		if (Keyboard::Trg(i)) return true;
		if (Mouse::Trg(i)) return true;
		if (Joystick::Trg(i)) return true;
		if (Joystick::PovTrg(i)) return true;
		if (Joystick::StickTrg(i)) return true;
	}
	return false;
}
bool Input::BothDevOn(std::vector<int>*& inputs) {
	for (auto& i : *inputs) {
		if (Keyboard::On(i)) return true;
		if (Mouse::On(i)) return true;
		if (Joystick::On(i)) return true;
		if (Joystick::PovOn(i)) return true;
		if (Joystick::StickOn(i)) return true;
	}
	return false;
}
bool Input::BothDevRel(std::vector<int>*& inputs) {
	for (auto& i : *inputs) {
		if (Keyboard::Rel(i)) return true;
		if (Mouse::Rel(i)) return true;
		if (Joystick::Rel(i)) return true;
		if (Joystick::PovRel(i)) return true;
		if (Joystick::StickRel(i)) return true;
	}
	return false;
}

bool Input::KeyboardDevTrg(std::vector<int>*& inputs) {
	for (auto& i : *inputs) {
		if (Keyboard::Trg(i)) return true;
		if (Mouse::Trg(i)) return true;
	}
	return false;
}
bool Input::KeyboardDevOn(std::vector<int>*& inputs) {
	for (auto& i : *inputs) {
		if (Keyboard::On(i)) return true;
		if (Mouse::On(i)) return true;
	}
	return false;
}
bool Input::KeyboardDevRel(std::vector<int>*& inputs) {
	for (auto& i : *inputs) {
		if (Keyboard::Rel(i)) return true;
		if (Mouse::Rel(i)) return true;
	}
	return false;
}

bool Input::JoystickDevTrg(std::vector<int>*& inputs) {
	for (auto& i : *inputs) {
		if (Joystick::Trg(i)) return true;
		if (Joystick::PovTrg(i)) return true;
		if (Joystick::StickTrg(i)) return true;
	}
	return false;
}
bool Input::JoystickDevOn(std::vector<int>*& inputs) {
	for (auto& i : *inputs) {
		if (Joystick::On(i)) return true;
		if (Joystick::PovOn(i)) return true;
		if (Joystick::StickOn(i)) return true;
	}
	return false;
}
bool Input::JoystickDevRel(std::vector<int>*& inputs) {
	for (auto& i : *inputs) {
		if (Joystick::Rel(i)) return true;
		if (Joystick::PovRel(i)) return true;
		if (Joystick::StickRel(i)) return true;
	}
	return false;
}

float Input::BothDevDX(std::vector<int>*& inputs) {
	int btnVal = 0;
	float maxVal = 0;
	for (auto& i : *inputs) {
		btnVal = i < 0 ? -1 : 1;
		if (i == JOY_LSTICK) maxVal = Joystick::GetLX();
		if (i == JOY_RSTICK) maxVal = Joystick::GetRX();
		if (i == MOUSE_DXY)  maxVal = (float)Mouse::GetDX();
		if (Keyboard::On(abs(i))) maxVal = (float)btnVal;
		if (Joystick::On(abs(i)))  maxVal = (float)btnVal;
		if (Joystick::PovOn(abs(i))) maxVal = (float)btnVal;
	}
	return maxVal;
}
float Input::KeyboardDevDX(std::vector<int>*& inputs) {
	int btnVal = 0;
	float maxVal = 0;
	for (auto& i : *inputs) {
		btnVal = i < 0 ? -1 : 1;
		if (i == MOUSE_DXY) maxVal = (float)Mouse::GetDX();
		if (Keyboard::On(abs(i))) maxVal = (float)btnVal;
	}
	return maxVal;
}
float Input::JoystickDevDX(std::vector<int>*& inputs) {
	int btnVal = 0;
	float maxVal = 0;
	for (auto& i : *inputs) {
		btnVal = i < 0 ? -1 : 1;
		if (i == JOY_LSTICK) maxVal = Joystick::GetLX();
		if (i == JOY_RSTICK)  maxVal = Joystick::GetRX();
		if (Joystick::On(abs(i))) maxVal = (float)btnVal;
		if (Joystick::PovOn(abs(i))) maxVal = (float)btnVal;
	}
	return maxVal;
}

float Input::BothDevDY(std::vector<int>*& inputs) {
	int btnVal = 0;
	float maxVal = 0;
	for (auto& i : *inputs) {
		btnVal = i < 0 ? -1 : 1;
		if (i == JOY_LSTICK) maxVal = Joystick::GetLY();
		if (i == JOY_RSTICK) maxVal = Joystick::GetRY();
		if (i == MOUSE_DXY)  maxVal = (float)Mouse::GetDY();
		if (Keyboard::On(abs(i))) maxVal = (float)btnVal;
		if (Joystick::On(abs(i)))  maxVal = (float)btnVal;
		if (Joystick::PovOn(abs(i))) maxVal = (float)btnVal;
	}
	return maxVal;
}
float Input::KeyboardDevDY(std::vector<int>*& inputs) {
	int btnVal = 0;
	float maxVal = 0;
	for (auto& i : *inputs) {
		btnVal = i < 0 ? -1 : 1;
		if (i == MOUSE_DXY) maxVal = (float)Mouse::GetDY();
		if (Keyboard::On(abs(i))) maxVal = (float)btnVal;
	}
	return maxVal;
}
float Input::JoystickDevDY(std::vector<int>*& inputs) {
	int btnVal = 0;
	float maxVal = 0;
	for (auto& i : *inputs) {
		btnVal = i < 0 ? -1 : 1;
		if (i == JOY_LSTICK) maxVal = Joystick::GetLY();
		if (i == JOY_RSTICK) maxVal = Joystick::GetRY();
		if (Joystick::On(abs(i)))  maxVal = (float)btnVal;
		if (Joystick::PovOn(abs(i))) maxVal = (float)btnVal;
	}
	return maxVal;
}

bool Input::IsMouseMove() {
	if (currentDevice == eDeviceType::Joystick) return false;
	if (Mouse::GetDX() == 0 && Mouse::GetDY() == 0) return false;
	return true;
}

//デバイスの切り替え
void Input::SetKeyboardDevice() {
	currentDevice = eDeviceType::Keyboard;
	funcDX = KeyboardDevDX;
	funcDY = KeyboardDevDY;
	funcTrg = KeyboardDevTrg;
	funcOn = KeyboardDevOn;
	funcRel = KeyboardDevRel;
}
void Input::SetJoystickDevice() {
	currentDevice = eDeviceType::Joystick;
	funcDX = JoystickDevDX;
	funcDY = JoystickDevDY;
	funcTrg = JoystickDevTrg;
	funcOn = JoystickDevOn;
	funcRel = JoystickDevRel;
}
void Input::SetBothDevice() {
	currentDevice = eDeviceType::Both;
	funcDX = BothDevDX;
	funcDY = BothDevDY;
	funcTrg = BothDevTrg;
	funcOn = BothDevOn;
	funcRel = BothDevRel;
}