#include "../../../framework.h"
#include "../../../environment.h"

//��x���ׂĂ̔z��̏���������
SpriteAnimation::SpriteAnimation(const WCHAR* animation_file, bool loop) {
	isLoop = loop;

	FILE* fp = NULL;
	WCHAR _key[256] = { 0 };

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
			KeyFrame* temp = new KeyFrame();
			setKey = temp;
		}
		else if (wcscmp(_key, L"fr") == 0) {
			if(setKey != nullptr) fwscanf_s(fp, L"%f", &setKey->frame);
		}
		else if (wcscmp(_key, L"sprite") == 0) {
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
		}
		else if (wcscmp(_key, L"scale") == 0) {
			if (setKey != nullptr) fwscanf_s(fp, L"%f %f", &setKey->scaleX, &setKey->scaleY);
		}
		else if (wcscmp(_key, L"rot") == 0) {
			if (setKey != nullptr) fwscanf_s(fp, L"%f", &setKey->rot);
			keyFrames.emplace_back(setKey);
			setKey = nullptr;
		}
	}
}

SpriteAnimation::~SpriteAnimation() {
	for (auto& key : keyFrames) {
		delete key;
	}
}

void SpriteAnimation::AnimOn() {
	//�e�L�[�t���[�����ƍ�����
	for (int i = 0; i < keyFrames.size(); i++) {
		if (curKeyIndex >= i) continue; //���B���Ă���L�[�t���[����菬�����ꍇ�͂Ƃ΂�

		/*�t���[�����������ꍇ�A�S�Ă̏�Ԃ�ύX*/
		if (frameCount == keyFrames[i]->frame) {
			curKeyIndex = i; //���B�L�[�t���[���X�V

			/*�X�v���C�g��ԕύX*/
			if (keyFrames[i]->pSprite != pAnimRenderer->pRenderSprite.get() && keyFrames[i]->pSprite != nullptr) {
				pAnimRenderer->pRenderSprite = noDel_ptr<Sprite>(keyFrames[i]->pSprite);
			}

			pAnimRenderer->SetColor(keyFrames[i]->color.r, keyFrames[i]->color.g, keyFrames[i]->color.b, keyFrames[i]->color.a);
			pAnimRenderer->transform->SetScale(keyFrames[i]->scaleX, keyFrames[i]->scaleY);
			pAnimRenderer->transform->rotation.z = keyFrames[i]->rot;

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

		//pos(�L�[�t���[����x,y�̒��������̃L�[�܂ł̃t���[�����Ŋ������l(1�t���[���ł̈ړ���)��Position�ɑ���)
		float move_x = keyFrames[i]->x / (keyFrames[i]->frame - keyFrames[curKeyIndex]->frame);
		pAnimRenderer->transform->position.x += move_x;
		float move_y = keyFrames[i]->y / (keyFrames[i]->frame - keyFrames[curKeyIndex]->frame);
		pAnimRenderer->transform->position.y += move_y;

		//col
		float r_diff = (keyFrames[i]->color.r - keyFrames[curKeyIndex]->color.r) * rate;
		float g_diff = (keyFrames[i]->color.g - keyFrames[curKeyIndex]->color.g) * rate;
		float b_diff = (keyFrames[i]->color.b - keyFrames[curKeyIndex]->color.b) * rate;
		float a_diff = (keyFrames[i]->color.a - keyFrames[curKeyIndex]->color.a) * rate;
		pAnimRenderer->SetColor(keyFrames[curKeyIndex]->color.r + r_diff, keyFrames[curKeyIndex]->color.g + g_diff,
			keyFrames[curKeyIndex]->color.b + b_diff, keyFrames[curKeyIndex]->color.a + a_diff);

		//scale
		float scaleX_diff = (keyFrames[i]->scaleX - keyFrames[curKeyIndex]->scaleX) * rate;
		float scaleY_diff = (keyFrames[i]->scaleY - keyFrames[curKeyIndex]->scaleY) * rate;
		pAnimRenderer->transform->SetScale(keyFrames[curKeyIndex]->scaleX + scaleX_diff,
			keyFrames[curKeyIndex]->scaleY + scaleY_diff);

		//rot
		float rot_diff = (keyFrames[i]->rot - keyFrames[curKeyIndex]->rot) * rate;
		pAnimRenderer->transform->rotation.z = keyFrames[curKeyIndex]->rot + rot_diff;

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