#include "../../../framework.h"
#include "../../../environment.h"

using namespace MyFrameWork;

SpriteAnimation::SpriteAnimation(const WCHAR* animation_file, bool loop) {
	isLoop = loop;

	FILE* fp = NULL;
	WCHAR _key[256] = { 0 };

	//�t�@�C�����J��
	_wfopen_s(&fp, animation_file, L"rt");
	if (fp == NULL) {
		return;
	}

	KeyFrame* setKey = new KeyFrame();

	while (!feof(fp))
	{
		//�L�[���[�h�ǂݍ���
		fwscanf_s(fp, L"%s", _key, 256);
		
		if (wcscmp(_key, L"newkey") == 0) {
			//�i�[���Y�ꂪ����Ίi�[
			if (setKey != NULL) {
				keyFrames.emplace_back(setKey);
				setKey = nullptr;
			}
			//�V���ȃL�[�̐���
			KeyFrame* temp = new KeyFrame();
			setKey = temp;
			SetUpDefaultValue(setKey);
		}
		else if (wcscmp(_key, L"fr") == 0) {
			if(setKey != nullptr) fwscanf_s(fp, L"%f", &setKey->frame);
		}
		else if (wcscmp(_key, L"sprite") == 0) {
			if (setKey == NULL) continue;
			WCHAR _name[256] = { 0 };
			fwscanf_s(fp, L"%s", _key, 256);
			fwscanf_s(fp, L"%s", _name, 256);
			if (wcscmp(_key, L"0") == 0) continue;
			setKey->pSprite = new Sprite(_key,_name);
		}
		else if (wcscmp(_key, L"pos") == 0) {
			if (setKey != nullptr) fwscanf_s(fp, L"%f %f", &setKey->x, &setKey->y);
		}
		else if (wcscmp(_key, L"color") == 0) {
			if (setKey != nullptr) fwscanf_s(fp, L"%f %f %f %f", &setKey->color.r, &setKey->color.g, &setKey->color.b, &setKey->color.a);
			setKey->trigger.color = true;
		}
		else if (wcscmp(_key, L"scale") == 0) {
			if (setKey != nullptr) fwscanf_s(fp, L"%f %f", &setKey->scaleX, &setKey->scaleY);
			setKey->trigger.scale = true;
		}
		else if (wcscmp(_key, L"rot") == 0) {
			if (setKey != nullptr) fwscanf_s(fp, L"%f", &setKey->rot);
			setKey->trigger.rotation = true;
		}
		else if (wcscmp(_key, L"end") == 0) {
			keyFrames.emplace_back(setKey);
			setKey = nullptr;
		}
	}
	if (setKey != NULL) {
		keyFrames.emplace_back(setKey);
		setKey = nullptr;
	}
}

void SpriteAnimation::SetUpDefaultValue(KeyFrame* key) {
	key->pSprite = NULL;
	key->frame = 0;
	key->x = 0, key->y = 0;
	key->color = {1,1,1,1};
	key->scaleX = 1;
	key->scaleY = 1;
	key->rot = 0;
}

void SpriteAnimation::SetAnimRenderer(noDel_ptr<SpriteState> renderSprite) {
	if (renderSprite == NULL) return;
	pAnimRenderer = renderSprite;

	//�L�[���̔��f
	//�X�v���C�g��NULL�̏ꍇ�Ȃɂ��ύX�������Ȃ����߁A�e�L�[�̏��Ƃ��Ă͔��f�����Ȃ�
	//Position���ړ��ʂ��Ƃ��Ă��邽�ߎ��̃L�[�ɂ͔��f�����Ȃ�

	//�ŏ��̃L�[�͌��݂̃X�v���C�g�̏��������p��
	keyFrames[0]->color = pAnimRenderer->GetColor();
	keyFrames[0]->scaleX = pAnimRenderer->transform->scale.x;
	keyFrames[0]->scaleY = pAnimRenderer->transform->scale.y;
	keyFrames[0]->rot = pAnimRenderer->transform->rotation.z;

	//�Ӑ}�I�ɃA�j���[�V�����Ƃ��ĕύX���������Ă��Ȃ����ڂ͈�O�̃L�[�̏��������p��
	for (int i = 0; i < keyFrames.size(); i++) {
		if (i == 0) continue; //�ŏ��̃L�[�̓X�L�b�v
		int _befKey = i - 1;
		if(keyFrames[i]->trigger.color == false)
			keyFrames[i]->color = keyFrames[_befKey]->color;
		if (keyFrames[i]->trigger.scale == false) {
			keyFrames[i]->scaleX = keyFrames[_befKey]->scaleX;
			keyFrames[i]->scaleY = keyFrames[_befKey]->scaleY;
		}
		if (keyFrames[i]->trigger.rotation == false)
			keyFrames[i]->rot = keyFrames[_befKey]->rot;
	}
}

SpriteAnimation::~SpriteAnimation() {
	for (auto& key : keyFrames) {
		delete key;
	}
}

void SpriteAnimation::AnimOn() {
	//�I���g���K�[���O��
	isEnd = false;

	//�e�L�[�t���[�����ƍ�����
	for (int i = 0; i < keyFrames.size(); i++) {
		if (curKeyIndex >= i) continue; //���B���Ă���L�[�t���[����菬�����ꍇ�͂Ƃ΂�

		/*�t���[�����������ꍇ�A�S�Ă̏�Ԃ�ύX*/
		if (frameCount == keyFrames[i]->frame) {
			curKeyIndex = i; //���B�L�[�t���[���X�V

			/*�X�v���C�g��ԕύX*/
			SetKeySpriteState(keyFrames[curKeyIndex]);
			SetKeyState(keyFrames[curKeyIndex]);

			/*�L�[�t���[���̍Ō�ɓ��B�����ꍇ*/
			if (curKeyIndex == keyFrames.size() - 1) {
				AnimOff();
 				return;
			}

			break;
		}

		if (curKeyIndex < 0) break; //�����l�̏ꍇ�I��

		//���̃L�[�t���[���Ɍ����ď��X�ɏ�Ԃ�J��
		//���̃L�[�t���[���܂ł̐i������(���݂̃t���[����-���B���Ă���L�[�t���[����) / (���̃L�[�t���[����-���B���Ă���L�[�t���[����)
		float rate = (frameCount - keyFrames[curKeyIndex]->frame) / 
			(keyFrames[i]->frame - keyFrames[curKeyIndex]->frame);

		//pos
		TransPos(keyFrames[i]);
		//col
		TransCol(keyFrames[i], rate);
		//scale
		TransScl(keyFrames[i], rate);
		//rot
		TransRot(keyFrames[i], rate);

		break;
	}

	frameCount++; //�t���[���J�E���g�X�V
}
void SpriteAnimation::AnimOff() {
	isEnd = true;
	frameCount = 0;
	curKeyIndex = -1;
}

SpriteAnimation::KeyFrame::~KeyFrame() {
	delete pSprite;
}

//�L�[�t���[���̃X�v���C�g��Ԃ̔��f
void SpriteAnimation::SetKeySpriteState(KeyFrame* key) {
	//�X�v���C�g���ȑO�̏�ԂƈႤ�ꍇ����
	if (key->pSprite != pAnimRenderer->pRenderSprite.get() && key->pSprite != nullptr) {
		pAnimRenderer->pRenderSprite = noDel_ptr<Sprite>(key->pSprite);
		pAnimRenderer->SetDefaultUV();
	}
}

//���̃L�[�Ɍ����ĕω�
void SpriteAnimation::TransPos(KeyFrame* key) {
	//pos(�L�[�t���[����x,y�̒��������̃L�[�܂ł̃t���[�����Ŋ������l(1�t���[���ł̈ړ���)��Position�ɑ���)
	float move_x = key->x / (key->frame - keyFrames[curKeyIndex]->frame);
	pAnimRenderer->transform->position.x += move_x;
	float move_y = key->y / (key->frame - keyFrames[curKeyIndex]->frame);
	pAnimRenderer->transform->position.y += move_y;
}
void SpriteAnimation::TransCol(KeyFrame* key, float rate) {
	float r_diff = (key->color.r - keyFrames[curKeyIndex]->color.r) * rate;
	float g_diff = (key->color.g - keyFrames[curKeyIndex]->color.g) * rate;
	float b_diff = (key->color.b - keyFrames[curKeyIndex]->color.b) * rate;
	float a_diff = (key->color.a - keyFrames[curKeyIndex]->color.a) * rate;
	pAnimRenderer->SetColor(keyFrames[curKeyIndex]->color.r + r_diff, keyFrames[curKeyIndex]->color.g + g_diff,
		keyFrames[curKeyIndex]->color.b + b_diff, keyFrames[curKeyIndex]->color.a + a_diff);
}
void SpriteAnimation::TransScl(KeyFrame* key, float rate) {
	float scaleX_diff = (key->scaleX - keyFrames[curKeyIndex]->scaleX) * rate;
	float scaleY_diff = (key->scaleY - keyFrames[curKeyIndex]->scaleY) * rate;
	pAnimRenderer->transform->SetScale(keyFrames[curKeyIndex]->scaleX + scaleX_diff,
		keyFrames[curKeyIndex]->scaleY + scaleY_diff);
}
void SpriteAnimation::TransRot(KeyFrame* key, float rate) {
	float rot_diff = (key->rot - keyFrames[curKeyIndex]->rot) * rate;
	pAnimRenderer->transform->rotation.z = keyFrames[curKeyIndex]->rot + rot_diff;
}

//����̃L�[�t���[����Ԃɐݒ�(���W�ȊO)
void  SpriteAnimation::SetKeyState(KeyFrame* key) {
	SetKeySpriteState(key);
	//�F
	pAnimRenderer->SetColor(key->color.r, key->color.g,	key->color.b, key->color.a);
	//�X�P�[��
	pAnimRenderer->transform->SetScale(key->scaleX,key->scaleY);
	//��]
	pAnimRenderer->transform->rotation.z = key->rot;
}
