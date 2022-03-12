#include "../../../framework.h"
#include "../../../environment.h"

using namespace MyFrameWork;

int	Mouse::x = 0;
int	Mouse::y = 0;
int	Mouse::on = 0;
int	Mouse::trg = 0;
int	Mouse::rel = 0;

//�}�E�X
int Mouse::GetX() {
	return x;
}
int Mouse::GetY() {
	return y;
}
int Mouse::GetR() {
	return DirectInput::GetMouseState().lZ;
}
int Mouse::GetDX() {
	return DirectInput::GetMouseState().lX;
}
int Mouse::GetDY() {
	return DirectInput::GetMouseState().lY;
}

//�L�[�{�[�h�ƃW���C�X�e�B�b�N�ƕ��p���邽�߃{�^����+500�ɐݒ肵�Ă��邽��-500�������ė��p
bool Mouse::On(int mouse) {
	mouse -= 500;
	if (mouse < 0) return false;
	return (mouse & on);
}
bool Mouse::Trg(int mouse) {
	mouse -= 500;
	if (mouse < 0) return false;
	if (mouse < 0) mouse = 0;
	return (mouse & trg);
}
bool Mouse::Rel(int mouse) {
	mouse -= 500;
	if (mouse < 0) return false;
	return (mouse & rel);
}

void Mouse::SetMouseStatus() {
	//�}�E�X
	int Old = on;
	on = 0;
	if (DirectInput::GetMouseState().rgbButtons[0])on |= 0x01;
	if (DirectInput::GetMouseState().rgbButtons[1])on |= 0x04;
	if (DirectInput::GetMouseState().rgbButtons[2])on |= 0x02;
	trg = (on ^ Old) & on;
	rel = (on ^ Old) & Old;
}

void Mouse::SetMousePosition(int _x, int _y) {
	x = _x;
	y = _y;
}