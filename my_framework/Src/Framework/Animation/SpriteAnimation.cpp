#include "../../framework.h"
#include "../../environment.h"

//��x���ׂĂ̔z��̏���������
SpriteAnimation::SpriteAnimation(noDel_ptr<GameObject2D> obj, bool loop) {
	pAnimObj = obj; //���ݕ\������X�v���C�g
	isLoop = loop;
}

void SpriteAnimation::CreateKeyFrame(int keyNum) {
	for (int i = 0; i < keyNum + 1; i++) {
		if (i < keyFrame.size()) continue;
		keyFrame.emplace_back(new KeyFrame());

		keyFrame[i]->flame = 0;
		keyFrame[i]->pSprite = pAnimObj->pRenderSprite;
		keyFrame[i]->x = pAnimObj->position.x;
		keyFrame[i]->y = pAnimObj->position.y;
		keyFrame[i]->r = pAnimObj->col_r[0];
		keyFrame[i]->g = pAnimObj->col_g[0];
		keyFrame[i]->b = pAnimObj->col_b[0];
		keyFrame[i]->a = pAnimObj->col_a[0];
		keyFrame[i]->scaleX = pAnimObj->scale.x;
		keyFrame[i]->scaleY = pAnimObj->scale.y;
		keyFrame[i]->rot = pAnimObj->rot.z;
	}
}

void SpriteAnimation::AnimOn() {
	//�e�L�[�t���[�����ƍ�����
	for (int i = 0; i < keyFrame.size(); i++) {
		if (currentKeyFrameIndex >= i) continue; //���B���Ă���L�[�t���[����菬�����ꍇ�͂Ƃ΂�
		if (keyFrame[i]->flame < flameCount) { //�����B�w��L�[�t���[������茻�݂̌o�߃t���[�������傫���ꍇ
			if (i == keyFrame.size() - 1) {
				if (isLoop) AnimLoop();
				else AnimOff();
				return;
			}
			continue;
		}

		//�t���[�����������ꍇ�A�S�Ă̏�Ԃ�ύX
		if (flameCount == keyFrame[i]->flame) {
			currentKeyFrameIndex = i;

			//�X�v���C�g��ԕύX
			if (keyFrame[i]->pSprite != pAnimObj->pRenderSprite) {
				pAnimObj->pRenderSprite = keyFrame[i]->pSprite;
			}

			pAnimObj->SetPosition(keyFrame[i]->x, keyFrame[i]->y);
			pAnimObj->SetColor(keyFrame[i]->r, keyFrame[i]->g, keyFrame[i]->b, keyFrame[i]->a);
			pAnimObj->SetScale(keyFrame[i]->scaleX, keyFrame[i]->scaleY);
			pAnimObj->rot.z = keyFrame[i]->rot;

			//�L�[�t���[���̍Ō�ɓ��B�����ꍇ
			if (currentKeyFrameIndex == keyFrame.size() - 1) {
				if (isLoop) AnimLoop();
				else AnimOff();
 				return;
			}

			break;
		}

		//
		//���̃L�[�t���[���Ɍ����ď��X�ɏ�Ԃ�J��
		// 
		if (currentKeyFrameIndex < 0) break; //�����l�̏ꍇ�I��

		//���̃L�[�t���[���܂ł̐i������(���݂̃t���[����-���B���Ă���L�[�t���[����) / (���̃L�[�t���[����-���B���Ă���L�[�t���[����)
		float rate = (flameCount - keyFrame[currentKeyFrameIndex]->flame) / 
			(keyFrame[i]->flame - keyFrame[currentKeyFrameIndex]->flame);

		//pos
		float move_x = (keyFrame[i]->x - keyFrame[currentKeyFrameIndex]->x) * rate;
		pAnimObj->position.x = keyFrame[currentKeyFrameIndex]->x + move_x;
		float move_y = (keyFrame[i]->y - keyFrame[currentKeyFrameIndex]->y) * rate;
		pAnimObj->position.y = keyFrame[currentKeyFrameIndex]->y + move_y;

		//col
		float r_diff = (keyFrame[i]->r - keyFrame[currentKeyFrameIndex]->r) * rate;
		float g_diff = (keyFrame[i]->g - keyFrame[currentKeyFrameIndex]->g) * rate;
		float b_diff = (keyFrame[i]->b - keyFrame[currentKeyFrameIndex]->b) * rate;
		float a_diff = (keyFrame[i]->a - keyFrame[currentKeyFrameIndex]->a) * rate;
		pAnimObj->SetColor(keyFrame[currentKeyFrameIndex]->r + r_diff, keyFrame[currentKeyFrameIndex]->g + g_diff,
			keyFrame[currentKeyFrameIndex]->b + b_diff, keyFrame[currentKeyFrameIndex]->a + a_diff);

		//scale
		float scaleX_diff = (keyFrame[i]->scaleX - keyFrame[currentKeyFrameIndex]->scaleX) * rate;
		float scaleY_diff = (keyFrame[i]->scaleY - keyFrame[currentKeyFrameIndex]->scaleY) * rate;
		pAnimObj->SetScale(keyFrame[currentKeyFrameIndex]->scaleX + scaleX_diff,
			keyFrame[currentKeyFrameIndex]->scaleY + scaleY_diff);

		//rot
		float rot_diff = (keyFrame[i]->rot - keyFrame[currentKeyFrameIndex]->rot) * rate;
		pAnimObj->rot.z = keyFrame[currentKeyFrameIndex]->rot + rot_diff;

		break;
	}

	flameCount++; //�t���[���J�E���g�X�V
}

void SpriteAnimation::AnimOff() {
	animEnable = false;
	flameCount = 0;
	currentKeyFrameIndex = -1;
}
void SpriteAnimation::AnimLoop() {
	flameCount = 0;
	currentKeyFrameIndex = -1;
}


bool SpriteAnimation::isAnimEnable() {
	return animEnable;
}
void SpriteAnimation::SetAnimEnable(bool flag) {
	animEnable = flag;
}

void SpriteAnimation::SetKeyFrameSprite(int keyIndex, noDel_ptr<Sprite> sprite) {
	if (keyIndex >= keyFrame.size()) CreateKeyFrame(keyIndex);
	keyFrame[keyIndex]->pSprite = sprite;
}
void SpriteAnimation::SetKeyFrameSprite(int keyIndex, noDel_ptr<Sprite> sprite, float flame) {
	if (keyIndex >= keyFrame.size()) CreateKeyFrame(keyIndex);
	keyFrame[keyIndex]->pSprite = sprite;
	keyFrame[keyIndex]->flame = flame;
}

void SpriteAnimation::SetKeyFramePos(int keyIndex, float x, float y) {
	if (keyIndex >= keyFrame.size()) CreateKeyFrame(keyIndex);
	keyFrame[keyIndex]->x = x;
	keyFrame[keyIndex]->y = y;
}
void SpriteAnimation::SetKeyFramePos(int keyIndex, float x, float y, float flame) {
	if (keyIndex >= keyFrame.size()) CreateKeyFrame(keyIndex);
	keyFrame[keyIndex]->x = x;
	keyFrame[keyIndex]->y = y;
	keyFrame[keyIndex]->flame = flame;
}

void SpriteAnimation::SetKeyFrameCol(int keyIndex, float r, float g, float b, float a) {
	if (keyIndex >= keyFrame.size()) CreateKeyFrame(keyIndex);
	keyFrame[keyIndex]->r = r;
	keyFrame[keyIndex]->g = g;
	keyFrame[keyIndex]->b = b;
	keyFrame[keyIndex]->a = a;
}
void SpriteAnimation::SetKeyFrameCol(int keyIndex, float r, float g, float b, float a, float flame) {
	if (keyIndex >= keyFrame.size()) CreateKeyFrame(keyIndex);
	keyFrame[keyIndex]->r = r;
	keyFrame[keyIndex]->g = g;
	keyFrame[keyIndex]->b = b;
	keyFrame[keyIndex]->a = a;
	keyFrame[keyIndex]->flame = flame;
}

void SpriteAnimation::SetKeyFrameScale(int keyIndex, float x, float y) {
	if (keyIndex >= keyFrame.size()) CreateKeyFrame(keyIndex);
	keyFrame[keyIndex]->scaleX = x;
	keyFrame[keyIndex]->scaleY = y;
}
void SpriteAnimation::SetKeyFrameScale(int keyIndex, float x, float y, float flame) {
	if (keyIndex >= keyFrame.size()) CreateKeyFrame(keyIndex);
	keyFrame[keyIndex]->scaleX = x;
	keyFrame[keyIndex]->scaleY = y;
	keyFrame[keyIndex]->flame = flame;
}

void SpriteAnimation::SetKeyFrameRot(int keyIndex, float rot) {
	if (keyIndex >= keyFrame.size()) CreateKeyFrame(keyIndex);
	keyFrame[keyIndex]->rot = rot;
}
void SpriteAnimation::SetKeyFrameRot(int keyIndex, float rot, float flame) {
	if (keyIndex >= keyFrame.size()) CreateKeyFrame(keyIndex);
	keyFrame[keyIndex]->rot = rot;
	keyFrame[keyIndex]->flame = flame;
}