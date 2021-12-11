#define _WIN32_DCOM
#define WINDOW_WIDTH 1280 //ウィンドウ幅
#define WINDOW_HEIGHT 750 //ウィンドウ高さ#define WINDOW_WIDTH 1280 //ウィンドウ幅
#define SCREEN_WIDTH (WINDOW_WIDTH) //スクリーン幅
#define SCREEN_HEIGHT (WINDOW_HEIGHT) //スクリーン高さ
#define SCREEN_WIDTH_CENTER (SCREEN_WIDTH / 2) //スクリーン幅
#define SCREEN_HEIGHT_CENTER (SCREEN_HEIGHT / 2) //スクリーン高さ


#define DIRECTINPUT_VERSION 0x0800

//マクロ
#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}


//共通ヘッダー
#include <d3d11.h>
#include <d2d1.h>
#include <dwrite.h>
#include <d3dcompiler.h>
#include <xaudio2.h>
#include <dinput.h>

#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <unordered_map>
#include <locale.h>
#include <mmsystem.h>

#include <DirectXMath.h>
#include "WICTextureLoader11.h"

//ライブラリとネームスペース
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib") 
#pragma comment(lib,"d3dCompiler.lib")

using namespace DirectX;

//Utility
#include "Src/Framework/GraphicsUtility.h"
#include "Src//Utility/noDel_ptr.h"
#include "Src//Utility/UtilFunc.h"


//フレームワークのヘッダー
//スプライト
#include "Src/Framework/Sprite/Sprite.h"
#include "Src/Framework/Sprite/SpriteManager.h"

//メッシュ
#include "Src//Framework//Mesh/Mesh.h"
#include "Src//Framework//Mesh/MeshManager.h"

//Objectクラス
#include "Src/Framework/Object/Object.h"

//メッセージシステム
#include "Src//Framework/MessageSystem/MessageSystem.h"

//コンポーネント
#include "Src/Framework/Component/Component.h"

//コンポーネント群
#include "Src/Framework/Component/Transform/Transform.h"
#include "Src/Framework/Component/Collider/Collider2D.h"
#include "Src/Framework/Component/Physics/Physics2D.h"
#include "Src/Framework/Component/Renderer/MeshRenderer.h"
#include "Src/Framework/Component/Renderer/SpriteRenderer.h"
#include "Src/Framework/Component/Behaviour/Behaviour.h"
#include "Src/Framework/Component/Font/Font.h"

//スプライトアニメーション
#include "Src/Framework/Animation/SpriteAnimation.h"

//アニメーターコンポーネント
#include "Src/Framework/Component/Animator/Animator.h"

//ゲームオブジェクト
#include "Src/Framework/Object/GameObject.h"

//オブジェクト、コンポーネント管理クラス
#include "Src//Framework/Object/GameObjectManager.h"





//タイマー
#include "Src//Framework/Timer/Timer.h"

//インプット
#include "Src/Framework/Input/DirectInput.h"
#include "Src//Framework//Input/Keyboard.h"
#include "Src//Framework//Input/Mouse.h"
#include "Src//Framework//Input/Joystick.h"
#include "Src//Framework//Input/Input.h"

//サウンド
#include "Src/Framework/Sound/Sound.h"
#include "Src/Framework/Sound/SoundManager.h"

//シェーダー設定
#include "Src/Framework/Shader/Shader.h"

//Direct3D
#include "Src/Framework/Direct3D/Direct3D.h"

//全体処理クラス
#include "Src/Framework/Main.h"


//シーン関係のヘッダ
#include "Src/Public/Scene/SceneManager.h"


