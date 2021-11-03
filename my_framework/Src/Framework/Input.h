#pragma once

class Input {
private:
	static HDC hdc;
	static LPDIRECTINPUT8 pInput;
	static LPDIRECTINPUTDEVICE8 pKeyDevice;
	static LPDIRECTINPUTDEVICE8 pMouseDevice;
	static LPDIRECTINPUTDEVICE8 pJoystickDevice;

	//�L�[�{�[�h
	static BYTE	diKeyboard[256]; //���͂��ꂽ�L�[
	static BYTE	oldKeyboard[256]; //���͂��ꂽ�L�[

	//�}�E�X
	static DIMOUSESTATE2 diMouse; //���͂��ꂽ�L�[
	static int	mouseX;
	static int	mouseY;
	static int	mouseOn;
	static int	mouseTrg;
	static int	mouseRel;

	//�W���C�X�e�B�b�N
	static DIJOYSTATE2 diJoy;
	static DIJOYSTATE2 oldJoy;
	static int	JoystickAxisRange;
	static float JoystickAxisRangeI;
	
private:
	static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);
	static BOOL CALLBACK EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);

public:
	static bool InitInput(void* hWnd);
	static void DestroyInput();
	static void KeyManager();

	//�L�[�{�[�h(DIK)
	static bool KeyOn(BYTE key);	
	static bool KeyTrg(BYTE key);
	static bool KeyRel(BYTE key);

	//�}�E�X(L(1) R(4) R(2))
	static void SetMousePosition(int x, int y);

	static bool MouseOn(int mouse);
	static bool MouseTrg(int mouse);
	static bool MouseRel(int mouse);

	static int GetMouseX();
	static int GetMouseY();
	static int GetMouseR();
	static int GetMouseDX();
	static int GetMouseDY();

	//�W���C�X�e�B�b�N(0~)
	static bool JoyButtonOn(int button);
	static bool JoyButtonTrg(int button);
	static bool JoyButtonRel(int button);
	static float GetJoyLX();
	static float GetJoyLY();
	static float GetJoyRX();
	static float GetJoyRY();
	static bool JoyPovOn(int pov);
	static bool JoyPovTrg(int pov);
	static bool JoyPovRel(int pov);
};