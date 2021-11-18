#include "../../../framework.h"
#include "../../../environment.h"

bool Joystick::IsValid() {
	return DirectInput::IsJoystickEnable();
}
bool Joystick::On(int button) {
	return (DirectInput::GetCurrentJoystick().rgbButtons[button] != 0);
}
bool Joystick::Trg(int button) {
	return (DirectInput::GetCurrentJoystick().rgbButtons[button] != 0 && DirectInput::GetOldJoystick().rgbButtons[button] == 0);
}
bool Joystick::Rel(int button) {
	return (DirectInput::GetCurrentJoystick().rgbButtons[button] == 0 && DirectInput::GetOldJoystick().rgbButtons[button] != 0);
}
float Joystick::GetLX() {
	float lx = DirectInput::GetCurrentJoystick().lX * DirectInput::GetJoyAxisRange();
	if (lx < 0.1f && lx > -0.1f) return 0;
	return lx;
}
float Joystick::GetLY() {
	float ly = DirectInput::GetCurrentJoystick().lY * DirectInput::GetJoyAxisRange();
	if (ly < 0.1f && ly > -0.1f) return 0;
	return ly;
}
float Joystick::GetRX() {
	float rx = DirectInput::GetCurrentJoystick().lZ * DirectInput::GetJoyAxisRange();
	if (rx < 0.1f && rx > -0.1f) return 0;
	return rx;
}
float Joystick::GetRY() {
	float ry = DirectInput::GetCurrentJoystick().lRz * DirectInput::GetJoyAxisRange();
	if (ry < 0.1f && ry > -0.1f) return 0;
	return ry;
}
bool Joystick::PovOn(int pov) {
	return DirectInput::GetCurrentJoystick().rgdwPOV[0] == pov;
}
bool Joystick::PovTrg(int pov) {
	return DirectInput::GetCurrentJoystick().rgdwPOV[0] == pov && DirectInput::GetOldJoystick().rgdwPOV[0] != pov;
}
bool Joystick::PovRel(int pov) {
	return DirectInput::GetCurrentJoystick().rgdwPOV[0] != pov && DirectInput::GetOldJoystick().rgdwPOV[0] == pov;
}