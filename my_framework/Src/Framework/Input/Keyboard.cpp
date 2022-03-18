#include "../../../framework.h"
#include "../../../environment.h"

using namespace MyFrameWork;

int Keyboard::GetTrgKeyType() {
	for (int i = 0x01; i < 0xdf; i++) {
		if (Trg(i)) return i;
	}
	return 0;
}
int Keyboard::GetOnKeyType() {
	for (int i = 0x01; i < 0xdf; i++) {
		if (On(i)) return i;
	}
	return 0;
}
int Keyboard::GetRelKeyType() {
	for (int i = 0x01; i < 0xdf; i++) {
		if (Rel(i)) return i;
	}
	return 0;
}
bool Keyboard::On(int key) {
	if (key > 223 || key < 0) return false;
	BYTE* cur = DirectInput::GetCurrentKeyboard();
	return (cur[key] != 0);
}
bool Keyboard::Trg(int key) {
	if (key > 223 || key < 0) return false;
	BYTE* cur = DirectInput::GetCurrentKeyboard();
	BYTE* old = DirectInput::GetOldKeyboard();
	return (cur[key] != 0 && old[key] == 0);
}
bool Keyboard::Rel(int key) {
	if (key > 223 || key < 0) return false;
	BYTE* cur = DirectInput::GetCurrentKeyboard();
	BYTE* old = DirectInput::GetOldKeyboard();
	return (cur[key] == 0 && old[key] != 0);
}