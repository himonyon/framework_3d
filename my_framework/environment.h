#define _WIN32_DCOM
#define WINDOW_WIDTH 1280 //�E�B���h�E��
#define WINDOW_HEIGHT 750 //�E�B���h�E����#define WINDOW_WIDTH 1280 //�E�B���h�E��
#define SCREEN_WIDTH (WINDOW_WIDTH) //�X�N���[����
#define SCREEN_HEIGHT (WINDOW_HEIGHT) //�X�N���[������
#define SCREEN_WIDTH_CENTER (SCREEN_WIDTH / 2) //�X�N���[����
#define SCREEN_HEIGHT_CENTER (SCREEN_HEIGHT / 2) //�X�N���[������


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
#include <string>
#include <time.h>
#include <vector>
#include <unordered_map>
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
#include "Src//Utility/UtilFunc.h"


//�t���[�����[�N�̃w�b�_�[
//�X�v���C�g
#include "Src/Framework/Sprite/Sprite.h"
#include "Src/Framework/Sprite/SpriteManager.h"

//���b�V��
#include "Src//Framework//Mesh/Mesh.h"
#include "Src//Framework//Mesh/MeshManager.h"

//Object�N���X
#include "Src/Framework/Object/Object.h"

//���b�Z�[�W�V�X�e��
#include "Src//Framework/MessageSystem/MessageSystem.h"

//�R���|�[�l���g
#include "Src/Framework/Component/Component.h"

//�R���|�[�l���g�Q
#include "Src/Framework/Component/Transform/Transform.h"
#include "Src/Framework/Component/Collider/Collider2D.h"
#include "Src/Framework/Component/Physics/Physics2D.h"
#include "Src/Framework/Component/Renderer/MeshRenderer.h"
#include "Src/Framework/Component/Renderer/SpriteRenderer.h"
#include "Src/Framework/Component/Behaviour/Behaviour.h"
#include "Src/Framework/Component/Font/Font.h"

//�X�v���C�g�A�j���[�V����
#include "Src/Framework/Animation/SpriteAnimation.h"

//�A�j���[�^�[�R���|�[�l���g
#include "Src/Framework/Component/Animator/Animator.h"

//�Q�[���I�u�W�F�N�g
#include "Src/Framework/Object/GameObject.h"

//�I�u�W�F�N�g�A�R���|�[�l���g�Ǘ��N���X
#include "Src//Framework/Object/GameObjectManager.h"





//�^�C�}�[
#include "Src//Framework/Timer/Timer.h"

//�C���v�b�g
#include "Src/Framework/Input/DirectInput.h"
#include "Src//Framework//Input/Keyboard.h"
#include "Src//Framework//Input/Mouse.h"
#include "Src//Framework//Input/Joystick.h"
#include "Src//Framework//Input/Input.h"

//�T�E���h
#include "Src/Framework/Sound/Sound.h"
#include "Src/Framework/Sound/SoundManager.h"

//�V�F�[�_�[�ݒ�
#include "Src/Framework/Shader/Shader.h"

//Direct3D
#include "Src/Framework/Direct3D/Direct3D.h"

//�S�̏����N���X
#include "Src/Framework/Main.h"


//�V�[���֌W�̃w�b�_
#include "Src/Public/Scene/SceneManager.h"


