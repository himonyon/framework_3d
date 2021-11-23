/*-----------------------------------------------------------

	DirectInputクラス
		入力関連全般の基本設定

-------------------------------------------------------------*/
#pragma once

class DirectInput {
private:
	static HDC hdc;
	static LPDIRECTINPUT8 pInput;
	static LPDIRECTINPUTDEVICE8 pKeyDevice;
	static LPDIRECTINPUTDEVICE8 pMouseDevice;
	static LPDIRECTINPUTDEVICE8 pJoystickDevice;

	//キーボード
	static BYTE	diKeyboard[256]; //入力されたキー
	static BYTE	oldKeyboard[256]; //入力されたキー

	//マウス
	static DIMOUSESTATE2 diMouse; //入力されたキー

	//ジョイスティック
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

	//入力状態の管理
	static void KeyManager();
	
	//ジョイスティックの有効判定
	static bool IsJoystickEnable() { return pJoystickDevice != NULL; }

	//Getter,Setter
	static BYTE* GetCurrentKeyboard() { return diKeyboard; }
	static BYTE* GetOldKeyboard() { return oldKeyboard; }
	static DIMOUSESTATE2& GetMouseState() { return diMouse; }
	static DIJOYSTATE2& GetCurrentJoystick() { return diJoy; }
	static DIJOYSTATE2& GetOldJoystick() { return oldJoy; }
	static float& GetJoyAxisRange() { return JoystickAxisRangeI; }
};