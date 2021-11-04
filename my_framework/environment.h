#define _WIN32_DCOM
#define WINDOW_WIDTH 1280 //ウィンドウ幅
#define WINDOW_HEIGHT 750 //ウィンドウ高さ#define WINDOW_WIDTH 1280 //ウィンドウ幅
#define SCREEN_WIDTH (WINDOW_WIDTH) //スクリーン幅
#define SCREEN_HEIGHT (WINDOW_HEIGHT) //スクリーン高さ
#define SCREEN_WIDTH_CENTER (SCREEN_WIDTH / 2) //スクリーン幅
#define SCREEN_HEIGHT_CENTER (SCREEN_HEIGHT / 2) //スクリーン高さ
#define MOVE_SPEED 1 //ジョイスティック幅
#define FONT_STRING_MAX	(0x0400)	//1フレームで実行できるrenderの数
#define FONT_CHARACTER_MAX	(0x2000)	//1フレームで描画できる文字の数
#define MaxFontNum (20)	//1フレームの描画できる文字列
#define MaxSounds (50)

//ジョイスティック
#define JOY_SQUARE (0)
#define JOY_CROSS (1)
#define JOY_CIRCLE (2)
#define JOY_TRIANGLAR (3)
#define JOY_L (4)
#define JOY_R (5)
#define JOY_L2 (6)
#define JOY_R2 (7)
#define JOY_SHARE (8)
#define JOY_OPTION (9)
#define JOY_L_STICK (10)
#define JOY_R_STICK (11)
#define JOY_R_PS (12)
#define JOY_PAD (13)

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

#include <stdio.h>
#include <vector>
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


//フレームワークのヘッダー
#include "Src/Framework/Model.h"
#include "Src/Framework/Sprite.h"
#include "Src/Framework/Font.h"
#include "Src/Framework/Input.h"
#include "Src/Framework/Sound.h"
#include "Src/Framework/Shader.h"
#include "Src/Framework/Direct3D.h"
#include "Src/Framework/Main.h"


//アニメーション
#include "Src/Framework/Animation/SpriteAnimation.h"

//シーン関係のヘッダ
#include "Src/Public/Scene/sceneTable.h"


