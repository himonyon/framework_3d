#pragma once

class DirectInput {
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

	//���͏�Ԃ̊Ǘ�
	static void KeyManager();
	
	//�W���C�X�e�B�b�N�̗L������
	static bool IsJoystickEnable() { return pJoystickDevice != NULL; }

	//Getter,Setter
	static BYTE* GetCurrentKeyboard() { return diKeyboard; }
	static BYTE* GetOldKeyboard() { return oldKeyboard; }
	static DIMOUSESTATE2& GetMouseState() { return diMouse; }
	static DIJOYSTATE2& GetCurrentJoystick() { return diJoy; }
	static DIJOYSTATE2& GetOldJoystick() { return oldJoy; }
	static float& GetJoyAxisRange() { return JoystickAxisRangeI; }
};