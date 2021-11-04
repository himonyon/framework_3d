#define _WIN32_DCOM
#define WINDOW_WIDTH 1280 //�E�B���h�E��
#define WINDOW_HEIGHT 750 //�E�B���h�E����#define WINDOW_WIDTH 1280 //�E�B���h�E��
#define SCREEN_WIDTH (WINDOW_WIDTH) //�X�N���[����
#define SCREEN_HEIGHT (WINDOW_HEIGHT) //�X�N���[������
#define SCREEN_WIDTH_CENTER (SCREEN_WIDTH / 2) //�X�N���[����
#define SCREEN_HEIGHT_CENTER (SCREEN_HEIGHT / 2) //�X�N���[������
#define MOVE_SPEED 1 //�W���C�X�e�B�b�N��
#define FONT_STRING_MAX	(0x0400)	//1�t���[���Ŏ��s�ł���render�̐�
#define FONT_CHARACTER_MAX	(0x2000)	//1�t���[���ŕ`��ł��镶���̐�
#define MaxFontNum (20)	//1�t���[���̕`��ł��镶����
#define MaxSounds (50)

//�W���C�X�e�B�b�N
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

//�}�N��
#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}


//���ʃw�b�_�[
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



//���C�u�����ƃl�[���X�y�[�X
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib") 
#pragma comment(lib,"d3dCompiler.lib")

using namespace DirectX;


//�t���[�����[�N�̃w�b�_�[
#include "Src/Framework/Model.h"
#include "Src/Framework/Sprite.h"
#include "Src/Framework/Font.h"
#include "Src/Framework/Input.h"
#include "Src/Framework/Sound.h"
#include "Src/Framework/Shader.h"
#include "Src/Framework/Direct3D.h"
#include "Src/Framework/Main.h"


//�A�j���[�V����
#include "Src/Framework/Animation/SpriteAnimation.h"

//�V�[���֌W�̃w�b�_
#include "Src/Public/Scene/sceneTable.h"


