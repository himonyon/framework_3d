#include "../../environment.h"

HDC Input::hdc = 0;
LPDIRECTINPUT8 Input::pInput = 0;
LPDIRECTINPUTDEVICE8 Input::pKeyDevice = 0;
LPDIRECTINPUTDEVICE8 Input::pMouseDevice = 0;
LPDIRECTINPUTDEVICE8 Input::pJoystickDevice = 0;

BYTE Input::diKeyboard[256]; //入力されたキー
BYTE Input::oldKeyboard[256]; //入力されたキー

DIMOUSESTATE2 Input::diMouse; //入力されたマウス
int	Input::mouseX = 0;
int	Input::mouseY = 0;
int	Input::mouseOn = 0;
int	Input::mouseTrg = 0;
int	Input::mouseRel = 0;

DIJOYSTATE2 Input::diJoy = {0};
DIJOYSTATE2 Input::oldJoy = {0};
int	Input::JoystickAxisRange = 1000;
float Input::JoystickAxisRangeI = 1.0f / (float)JoystickAxisRange;

bool Input::InitInput(void* hWnd) {
	//DirectInputオブジェクトの作成
	if (FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8,
		(VOID**)&pInput, NULL))) {
		return false;
	}

	//キーボードデバイスオブジェクトの作成
	if (FAILED(pInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, NULL))) {
		return false;
	}
	//デバイスをキーボードに設定
	if (FAILED(pKeyDevice->SetDataFormat(&c_dfDIKeyboard))) {
		return false;
	}

	//マウスデバイスオブジェクトの作成
	if (FAILED(pInput->CreateDevice(GUID_SysMouse, &pMouseDevice, NULL))) {
		return false;
	}
	//デバイスをマウスに設定
	if (FAILED(pMouseDevice->SetDataFormat(&c_dfDIMouse2))) {
		return false;
	}

	//ジョイスティック
	//デバイスの列挙
	HRESULT hr;
	hr = pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY);
	if (FAILED(hr))
	{
		return FALSE;
	}
	if (!pJoystickDevice) {
		return FALSE;
	}
	if (FAILED(pJoystickDevice->SetDataFormat(&c_dfDIJoystick2))) {
		return FALSE;
	}

	//協調レベルの設定
	if (FAILED(pKeyDevice->SetCooperativeLevel((HWND)hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND))) {
		return false;
	}
	if (FAILED(pJoystickDevice->SetCooperativeLevel((HWND)hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND))) {
		return false;
	}

	//コントローラーを列挙して設定
	if (FAILED(pJoystickDevice->EnumObjects(Input::EnumObjectsCallback, NULL, DIDFT_ALL))) {
		return FALSE;
	}

	//デバイスを所得する
	pKeyDevice->Acquire();
	pJoystickDevice->Acquire();

	diJoy.rgdwPOV[0] = 1;
	oldJoy.rgdwPOV[0] = 1;
	return true;
}

void Input::DestroyInput() {
	if (pKeyDevice) pKeyDevice->Unacquire();
	SAFE_RELEASE(pKeyDevice);

	if (pMouseDevice)	pMouseDevice->Unacquire();
	SAFE_RELEASE(pMouseDevice)

	
	if (pJoystickDevice)	pJoystickDevice->Unacquire();
	SAFE_RELEASE(pJoystickDevice);

	SAFE_RELEASE(pInput);
}

BOOL CALLBACK Input::EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext) {
	if (pdidoi->dwType & DIDFT_AXIS) {
		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprg.diph.dwHow = DIPH_BYID;
		diprg.diph.dwObj = pdidoi->dwType;
		diprg.lMin = -JoystickAxisRange;
		diprg.lMax = +JoystickAxisRange;

		if (FAILED(pJoystickDevice->SetProperty(DIPROP_RANGE, &diprg.diph))) {
			return DIENUM_STOP;
		}
	}
	return DIENUM_CONTINUE;
}
BOOL CALLBACK Input::EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext) {
	HRESULT hr;

	//列挙されたジョイスティックへのインターフェイスを取得
	hr = pInput->CreateDevice(pdidInstance->guidInstance, &pJoystickDevice, NULL);
	if (FAILED(hr))
	{
		return DIENUM_CONTINUE;
	}

	return DIENUM_STOP;
}

void Input::SetMousePosition(int x, int y) {
	mouseX = x;
	mouseY = y;
}

void Input::KeyManager() {
	HRESULT hr;

	//マウス
	hr = pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &diMouse);
	if (FAILED(hr))
	{
		hr = pMouseDevice->Acquire();
	}

	//キーボード
	memcpy(oldKeyboard, diKeyboard, sizeof(oldKeyboard));	//前フレームのキー状態を保存
	hr = pKeyDevice->GetDeviceState(sizeof(diKeyboard), &diKeyboard);
	if (FAILED(hr)) {
		pKeyDevice->Acquire();
	}

	//ジョイスティック
	if (pJoystickDevice != NULL) {
		memcpy(&oldJoy, &diJoy, sizeof(DIJOYSTATE2));
		hr = pJoystickDevice->Poll();
		if (FAILED(hr)) {
			pJoystickDevice->Acquire();
		}
		hr = pJoystickDevice->GetDeviceState(sizeof(DIJOYSTATE2), &diJoy);
	}

	//マウス
	int Old = mouseOn;
	mouseOn = 0;
	if (diMouse.rgbButtons[0])mouseOn |= 0x01;
	if (diMouse.rgbButtons[1])mouseOn |= 0x04;
	if (diMouse.rgbButtons[2])mouseOn |= 0x02;
	mouseTrg = (mouseOn ^ Old) & mouseOn;
	mouseRel = (mouseOn ^ Old) & Old;
}

//キ－ボード
bool Input::KeyOn(BYTE key) {
    return (diKeyboard[key]!=0);
}
bool Input::KeyTrg(BYTE key) {
	return (diKeyboard[key]!=0 && oldKeyboard[key]==0);
}
bool Input::KeyRel(BYTE key) {
	return (diKeyboard[key]==0 && oldKeyboard[key]!=0);
}
//マウス
int Input::GetMouseX() {
	return mouseX;
}
int Input::GetMouseY() {
	return mouseY;
}
int Input::GetMouseR() {
	return diMouse.lZ;
}
int Input::GetMouseDX() {
	return diMouse.lX;
}
int Input::GetMouseDY() {
	return diMouse.lY;
}
bool Input::MouseOn(int mouse) {
	return (mouse&mouseOn);
}
bool Input::MouseTrg(int mouse) {
	return (mouse & mouseTrg);
}
bool Input::MouseRel(int mouse) {
	return (mouse & mouseRel);
}
//ジョイスティック
bool Input::JoyButtonOn(int button) {
	return (diJoy.rgbButtons[button]!=0);
}
bool Input::JoyButtonTrg(int button) {
	return (diJoy.rgbButtons[button]!=0 && oldJoy.rgbButtons[button]==0);
}
bool Input::JoyButtonRel(int button) {
	return (diJoy.rgbButtons[button]==0 && diJoy.rgbButtons[button]!=0);
}
float Input::GetJoyLX() {
	return diJoy.lX * JoystickAxisRangeI;
}
float Input::GetJoyLY() {
	return diJoy.lY * JoystickAxisRangeI;
}
float Input::GetJoyRX() {
	return diJoy.lZ * JoystickAxisRangeI;
}
float Input::GetJoyRY() {
	return diJoy.lRz * JoystickAxisRangeI;
}
bool Input::JoyPovOn(int pov) {
	return diJoy.rgdwPOV[0]==pov;
}
bool Input::JoyPovTrg(int pov) {
	return diJoy.rgdwPOV[0]==pov && oldJoy.rgdwPOV[0]!=pov;
}
bool Input::JoyPovRel(int pov) {
	return diJoy.rgdwPOV[0]!=pov && oldJoy.rgdwPOV[0]==pov;
}