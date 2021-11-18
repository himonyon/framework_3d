#include "../../../framework.h"
#include "../../../environment.h"


BYTE Keyboard::GetTrgKeyType() {
	for (int i = 0x01; i < 0xdf; i++) {
		if (Trg(i)) return i;
	}
	return 0;
}
BYTE Keyboard::GetOnKeyType() {
	for (int i = 0x01; i < 0xdf; i++) {
		if (On(i)) return i;
	}
	return 0;
}
BYTE Keyboard::GetRelKeyType() {
	for (int i = 0x01; i < 0xdf; i++) {
		if (Rel(i)) return i;
	}
	return 0;
}
bool Keyboard::On(BYTE key) {
	BYTE* cur = DirectInput::GetCurrentKeyboard();
	return (cur[key] != 0);
}
bool Keyboard::Trg(BYTE key) {
	BYTE* cur = DirectInput::GetCurrentKeyboard();
	BYTE* old = DirectInput::GetOldKeyboard();
	return (cur[key] != 0 && old[key] == 0);
}
bool Keyboard::Rel(BYTE key) {
	BYTE* cur = DirectInput::GetCurrentKeyboard();
	BYTE* old = DirectInput::GetOldKeyboard();
	return (cur[key] == 0 && old[key] != 0);
}