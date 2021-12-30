#include "../../../framework.h"
#include "../../../environment.h"

float Joystick::stickEnableRange = 0.1f; //�W���C�X�e�B�b�N�̃u���̒l�����O����͈�
float Joystick::stickFlagRange = 0.7f; //�W���C�X�e�B�b�N�̃{�^���Ƃ��Ă̔���͈�

bool Joystick::IsValid() {
	return DirectInput::IsJoystickEnable();
}
bool Joystick::On(int button) {
	//�L�[�{�[�h�ƕ��p����̂ɃW���C�{�^����+300�ɐݒ肵�Ă��邽��-300�������ė��p
	button -= 300;
	if (button > 127 || button < 0) return false;
	return (DirectInput::GetCurrentJoystick().rgbButtons[button] != 0);
}
bool Joystick::Trg(int button) {
	button -= 300;
	if (button > 127 || button < 0) return false;
	return (DirectInput::GetCurrentJoystick().rgbButtons[button] != 0 && DirectInput::GetOldJoystick().rgbButtons[button] == 0);
}
bool Joystick::Rel(int button) {
	button -= 300;
	if (button > 127 || button < 0) return false;
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
	//�\���L�[��Ǝl�p�{�^�����O�Ŕ��A��ʂ��邽�ߏ\���L�[��+10000�ݒ肵�Ă��邽��-����
	pov -= 10000;
	return DirectInput::GetCurrentJoystick().rgdwPOV[0] == pov;
}
bool Joystick::PovTrg(int pov) {
	pov -= 10000;
	return DirectInput::GetCurrentJoystick().rgdwPOV[0] == pov && DirectInput::GetOldJoystick().rgdwPOV[0] != pov;
}
bool Joystick::PovRel(int pov) {
	pov -= 10000;
	return DirectInput::GetCurrentJoystick().rgdwPOV[0] != pov && DirectInput::GetOldJoystick().rgdwPOV[0] == pov;
}
bool Joystick::StickTrg(int dir) {
	float cr;
	float ol;
	if (dir == JOY_LSTICKL) {
		cr = DirectInput::GetCurrentJoystick().lX * DirectInput::GetJoyAxisRange();
		ol = DirectInput::GetOldJoystick().lX * DirectInput::GetJoyAxisRange();
		return (cr < -stickFlagRange) && (ol > -stickFlagRange);
	}
	else if (dir == JOY_LSTICKR) {
		cr = DirectInput::GetCurrentJoystick().lX * DirectInput::GetJoyAxisRange();
		ol = DirectInput::GetOldJoystick().lX * DirectInput::GetJoyAxisRange();
		return (cr > stickFlagRange) && (ol < stickFlagRange);
	}
	else if (dir == JOY_LSTICKU) {
		cr = DirectInput::GetCurrentJoystick().lY * DirectInput::GetJoyAxisRange();
		ol = DirectInput::GetOldJoystick().lY * DirectInput::GetJoyAxisRange();
		return (cr < -stickFlagRange) && (ol > -stickFlagRange);
	}
	else if (dir == JOY_LSTICKD) {
		cr = DirectInput::GetCurrentJoystick().lY * DirectInput::GetJoyAxisRange();
		ol = DirectInput::GetOldJoystick().lY * DirectInput::GetJoyAxisRange();
		return (cr > stickFlagRange) && (ol < stickFlagRange);
	}
	else if (dir == JOY_RSTICKL) {
		cr = DirectInput::GetCurrentJoystick().lZ * DirectInput::GetJoyAxisRange();
		ol = DirectInput::GetOldJoystick().lZ * DirectInput::GetJoyAxisRange();
		return (cr < -stickFlagRange) && (ol > -stickFlagRange);
	}
	else if (dir == JOY_RSTICKR) {
		cr = DirectInput::GetCurrentJoystick().lZ * DirectInput::GetJoyAxisRange();
		ol = DirectInput::GetOldJoystick().lZ * DirectInput::GetJoyAxisRange();
		return (cr > stickFlagRange) && (ol < stickFlagRange);
	}
	else if (dir == JOY_RSTICKU) {
		cr = DirectInput::GetCurrentJoystick().lRz * DirectInput::GetJoyAxisRange();
		ol = DirectInput::GetOldJoystick().lRz * DirectInput::GetJoyAxisRange();
		return (cr < -stickFlagRange) && (ol > -stickFlagRange);
	}
	else if (dir == JOY_RSTICKD) {
		cr = DirectInput::GetCurrentJoystick().lRz * DirectInput::GetJoyAxisRange();
		ol = DirectInput::GetOldJoystick().lRz * DirectInput::GetJoyAxisRange();
		return (cr > stickFlagRange) && (ol < stickFlagRange);
	}

	return false;
}
bool Joystick::StickOn(int dir) {
	float cr;
	float ol;
	if (dir == JOY_LSTICKL) {
		cr = DirectInput::GetCurrentJoystick().lX * DirectInput::GetJoyAxisRange();
		ol = DirectInput::GetOldJoystick().lX * DirectInput::GetJoyAxisRange();
		return cr < -stickFlagRange;
	}
	else if (dir == JOY_LSTICKR) {
		cr = DirectInput::GetCurrentJoystick().lX * DirectInput::GetJoyAxisRange();
		ol = DirectInput::GetOldJoystick().lX * DirectInput::GetJoyAxisRange();
		return cr > stickFlagRange;
	}
	else if (dir == JOY_LSTICKU) {
		cr = DirectInput::GetCurrentJoystick().lY * DirectInput::GetJoyAxisRange();
		ol = DirectInput::GetOldJoystick().lY * DirectInput::GetJoyAxisRange();
		return cr < -stickFlagRange;
	}
	else if (dir == JOY_LSTICKD) {
		cr = DirectInput::GetCurrentJoystick().lY * DirectInput::GetJoyAxisRange();
		ol = DirectInput::GetOldJoystick().lY * DirectInput::GetJoyAxisRange();
		return cr > stickFlagRange;
	}
	else if (dir == JOY_RSTICKL) {
		cr = DirectInput::GetCurrentJoystick().lZ * DirectInput::GetJoyAxisRange();
		ol = DirectInput::GetOldJoystick().lZ * DirectInput::GetJoyAxisRange();
		return cr < -stickFlagRange;
	}
	else if (dir == JOY_RSTICKR) {
		cr = DirectInput::GetCurrentJoystick().lZ * DirectInput::GetJoyAxisRange();
		ol = DirectInput::GetOldJoystick().lZ * DirectInput::GetJoyAxisRange();
		return cr > stickFlagRange;
	}
	else if (dir == JOY_RSTICKU) {
		cr = DirectInput::GetCurrentJoystick().lRz * DirectInput::GetJoyAxisRange();
		ol = DirectInput::GetOldJoystick().lRz * DirectInput::GetJoyAxisRange();
		return cr < -stickFlagRange;
	}
	else if (dir == JOY_RSTICKD) {
		cr = DirectInput::GetCurrentJoystick().lRz * DirectInput::GetJoyAxisRange();
		ol = DirectInput::GetOldJoystick().lRz * DirectInput::GetJoyAxisRange();
		return cr > stickFlagRange;
	}

	return false;
}
bool Joystick::StickRel(int dir) {
	float cr;
	float ol;
	if (dir == JOY_LSTICKL) {
		cr = DirectInput::GetCurrentJoystick().lX * DirectInput::GetJoyAxisRange();
		ol = DirectInput::GetOldJoystick().lX * DirectInput::GetJoyAxisRange();
		return (cr > -stickFlagRange) && (ol < -stickFlagRange);
	}
	else if (dir == JOY_LSTICKR) {
		cr = DirectInput::GetCurrentJoystick().lX * DirectInput::GetJoyAxisRange();
		ol = DirectInput::GetOldJoystick().lX * DirectInput::GetJoyAxisRange();
		return (cr < stickFlagRange) && (ol > stickFlagRange);
	}
	else if (dir == JOY_LSTICKU) {
		cr = DirectInput::GetCurrentJoystick().lY * DirectInput::GetJoyAxisRange();
		ol = DirectInput::GetOldJoystick().lY * DirectInput::GetJoyAxisRange();
		return (cr > -stickFlagRange) && (ol < -stickFlagRange);
	}
	else if (dir == JOY_LSTICKD) {
		cr = DirectInput::GetCurrentJoystick().lY * DirectInput::GetJoyAxisRange();
		ol = DirectInput::GetOldJoystick().lY * DirectInput::GetJoyAxisRange();
		return (cr < stickFlagRange) && (ol > stickFlagRange);
	}
	else if (dir == JOY_RSTICKL) {
		cr = DirectInput::GetCurrentJoystick().lZ * DirectInput::GetJoyAxisRange();
		ol = DirectInput::GetOldJoystick().lZ * DirectInput::GetJoyAxisRange();
		return (cr > -stickFlagRange) && (ol < -stickFlagRange);
	}
	else if (dir == JOY_RSTICKR) {
		cr = DirectInput::GetCurrentJoystick().lZ * DirectInput::GetJoyAxisRange();
		ol = DirectInput::GetOldJoystick().lZ * DirectInput::GetJoyAxisRange();
		return (cr < stickFlagRange) && (ol > stickFlagRange);
	}
	else if (dir == JOY_RSTICKU) {
		cr = DirectInput::GetCurrentJoystick().lRz * DirectInput::GetJoyAxisRange();
		ol = DirectInput::GetOldJoystick().lRz * DirectInput::GetJoyAxisRange();
		return (cr > -stickFlagRange) && (ol < -stickFlagRange);
	}
	else if (dir == JOY_RSTICKD) {
		cr = DirectInput::GetCurrentJoystick().lRz * DirectInput::GetJoyAxisRange();
		ol = DirectInput::GetOldJoystick().lRz * DirectInput::GetJoyAxisRange();
		return (cr < stickFlagRange) && (ol > stickFlagRange);
	}

	return false;
}