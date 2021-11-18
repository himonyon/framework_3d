#define _WIN32_DCOM
#define WINDOW_WIDTH 1280 //�E�B���h�E��
#define WINDOW_HEIGHT 750 //�E�B���h�E����#define WINDOW_WIDTH 1280 //�E�B���h�E��
#define SCREEN_WIDTH (WINDOW_WIDTH) //�X�N���[����
#define SCREEN_HEIGHT (WINDOW_HEIGHT) //�X�N���[������
#define SCREEN_WIDTH_CENTER (SCREEN_WIDTH / 2) //�X�N���[����
#define SCREEN_HEIGHT_CENTER (SCREEN_HEIGHT / 2) //�X�N���[������
#define FONT_STRING_MAX	(0x0400)	//1�t���[���Ŏ��s�ł���render�̐�
#define FONT_CHARACTER_MAX	(0x2000)	//1�t���[���ŕ`��ł��镶���̐�



#define DIRECTINPUT_VERSION 0x0800

//�}�N��
#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}


//���ʃw�b�_�[
#include <d3d11.h>
#include <d2d1.h>
#include <dwrite.h>
#include <d3dcompiler.h>
#include <xaudio2.h>
#include <dinput.h>

#include <iostream>
#include <time.h>
#include <vector>
#include <locale.h>
#include <mmsystem.h>

#include <DirectXMath.h>
#include "WICTextureLoader11.h"


//���C�u�����ƃl�[���X�y�[�X
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

//�t���[�����[�N�̃w�b�_�[
#include "Src/Framework/Sprite/Sprite.h"
#include "Src/Framework/Sprite/SpriteManager.h"

#include "Src//Framework//Mesh/Mesh.h"
#include "Src//Framework//Mesh/MeshManager.h"

#include "Src/Framework/GameObject/GameObject.h"
#include "Src/Framework/GameObject/GameObject2D.h"
#include "Src/Framework/GameObject/GameObject3D.h"
#include "Src//Framework/GameObject/GameObjectManager.h"

#include "Src//Framework/Timer/Timer.h"

#include "Src/Framework/Font/Font.h"

#include "Src/Framework/Input/DirectInput.h"
#include "Src//Framework//Input/Keyboard.h"
#include "Src//Framework//Input/Mouse.h"
#include "Src//Framework//Input/Joystick.h"
#include "Src//Framework//Input/Input.h"

#include "Src/Framework/Sound/Sound.h"
#include "Src/Framework/Sound/SoundManager.h"

#include "Src/Framework/Shader/Shader.h"

#include "Src/Framework/Direct3D/Direct3D.h"

#include "Src/Framework/Main.h"

//�A�j���[�V����
#include "Src/Framework/Animation/SpriteAnimation.h"
#include "Src/Framework/Animation/AnimationManager.h"

//�V�[���֌W�̃w�b�_
#include "Src/Public/Scene/sceneTable.h"


