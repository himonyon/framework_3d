#include "../../../framework.h"
#include "../../../environment.h"

/// <summary>
/// ユーザー設定インプット情報の初期化
/// </summary>
int InputConfig::Decide[DeviceNum] = { DIK_SPACE, JOY_CIRCLE };
int InputConfig::Cansel[DeviceNum] = { DIK_BACKSPACE, JOY_CROSS };