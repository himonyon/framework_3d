#include "../../../framework.h"
#include "../../../environment.h"

using namespace MyFrameWork;

HDC DirectInput::hdc = 0;
LPDIRECTINPUT8 DirectInput::pInput = 0;
LPDIRECTINPUTDEVICE8 DirectInput::pKeyDevice = 0;
LPDIRECTINPUTDEVICE8 DirectInput::pMouseDevice = 0;
LPDIRECTINPUTDEVICE8 DirectInput::pJoystickDevice = 0;

BYTE DirectInput::diKeyboard[256]; //入力されたキー
BYTE DirectInput::oldKeyboard[256]; //入力されたキー

DIMOUSESTATE2 DirectInput::diMouse; //入力されたマウス

DIJOYSTATE2 DirectInput::diJoy = {0};
DIJOYSTATE2 DirectInput::oldJoy = {0};
int	DirectInput::JoystickAxisRange = 1000;
float DirectInput::JoystickAxisRangeI = 1.0f / (float)JoystickAxisRange;

bool DirectInput::InitInput(void* hWnd) {
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
	if (FAILED(pJoystickDevice->EnumObjects(DirectInput::EnumObjectsCallback, NULL, DIDFT_ALL))) {
		return FALSE;
	}

	//デバイスを所得する
	pKeyDevice->Acquire();
	pJoystickDevice->Acquire();

	diJoy.rgdwPOV[0] = 1;
	oldJoy.rgdwPOV[0] = 1;
	return true;
}

void DirectInput::DestroyInput() {
	if (pKeyDevice) pKeyDevice->Unacquire();
	SAFE_RELEASE(pKeyDevice);

	if (pMouseDevice)	pMouseDevice->Unacquire();
	SAFE_RELEASE(pMouseDevice)

	
	if (pJoystickDevice)	pJoystickDevice->Unacquire();
	SAFE_RELEASE(pJoystickDevice);

	SAFE_RELEASE(pInput);
}

BOOL CALLBACK DirectInput::EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext) {
	//コントローラーの有効検知
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
BOOL CALLBACK DirectInput::EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext) {
	HRESULT hr;

	//列挙されたジョイスティックへのインターフェイスを取得
	hr = pInput->CreateDevice(pdidInstance->guidInstance, &pJoystickDevice, NULL);
	if (FAILED(hr))
	{
		return DIENUM_CONTINUE;
	}

	return DIENUM_STOP;
}

void DirectInput::KeyManager() {
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

	Mouse::SetMouseStatus();
}