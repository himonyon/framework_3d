#include "../../framework.h"
#include "../../environment.h"

//一度すべての配列の情報を初期化
SpriteAnimation::SpriteAnimation(const WCHAR* texture_file, bool loop) {
	isLoop = loop;

	FILE* fp = NULL;
	char _key[256] = { 0 };
	_wfopen_s(&fp, texture_file, L"rt");
	if (fp == NULL) {
		return;
	}

	KeyFrame* setKey = new KeyFrame();

	while (!feof(fp))
	{
		//キーワード読み込み
		fscanf_s(fp, "%s", _key, (int)sizeof(_key));
		
		if (strcmp(_key, "newkey") == 0) {
			KeyFrame* temp = new KeyFrame();
			setKey = temp;
		}
		else if (strcmp(_key, "fr") == 0) {
			if(setKey != nullptr) fscanf_s(fp, "%f", &setKey->frame);
		}
		else if (strcmp(_key, "sprite") == 0) {
			fscanf_s(fp, "%s", _key, (int)sizeof(_key));
			if (strcmp(_key, "0") == 0) continue;
			if (setKey != nullptr) setKey->pSprite = SpriteManager::Find(_key);
		}
		else if (strcmp(_key, "pos") == 0) {
			if (setKey != nullptr) fscanf_s(fp, "%f %f", &setKey->x, &setKey->y);
		}
		else if (strcmp(_key, "color") == 0) {
			if (setKey != nullptr) fscanf_s(fp, "%f %f %f %f", &setKey->color.r, &setKey->color.g, &setKey->color.b, &setKey->color.a);
		}
		else if (strcmp(_key, "scale") == 0) {
			if (setKey != nullptr) fscanf_s(fp, "%f %f", &setKey->scaleX, &setKey->scaleY);
		}
		else if (strcmp(_key, "rot") == 0) {
			if (setKey != nullptr) fscanf_s(fp, "%f", &setKey->rot);
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
	//各キーフレームを照合する
	for (int i = 0; i < keyFrames.size(); i++) {
		if (currentKeyFrameIndex >= i) continue; //到達しているキーフレームより小さい場合はとばす

		/*フレーム数が同じ場合、全ての状態を変更*/
		if (frameCount == keyFrames[i]->frame) {
			currentKeyFrameIndex = i; //到達キーフレーム更新

			/*スプライト状態変更*/
			if (keyFrames[i]->pSprite != pAnimRenderer->pRenderSprite && keyFrames[i]->pSprite != nullptr) {
				pAnimRenderer->pRenderSprite = keyFrames[i]->pSprite;
			}

			pAnimRenderer->SetColor(keyFrames[i]->color.r, keyFrames[i]->color.g, keyFrames[i]->color.b, keyFrames[i]->color.a);
			pAnimRenderer->transform->SetScale(keyFrames[i]->scaleX, keyFrames[i]->scaleY);
			pAnimRenderer->transform->rotation.z = keyFrames[i]->rot;

			/*キーフレームの最後に到達した場合*/
			if (currentKeyFrameIndex == keyFrames.size() - 1) {
				AnimOff();
 				return;
			}

			break;
		}

		if (currentKeyFrameIndex < 0) break; //初期値の場合終了

		//次のキーフレームに向けて徐々に状態を遷移
		//次のキーフレームまでの進捗割合(現在のフレーム数-到達しているキーフレーム数) / (次のキーフレーム数-到達しているキーフレーム数)
		float rate = (frameCount - keyFrames[currentKeyFrameIndex]->frame) / 
			(keyFrames[i]->frame - keyFrames[currentKeyFrameIndex]->frame);

		//pos(キーフレームのx,yの長さを次のキーまでのフレーム数で割った値(1フレームでの移動量)をPositionに足す)
		float move_x = keyFrames[i]->x / (keyFrames[i]->frame - keyFrames[currentKeyFrameIndex]->frame);
		pAnimRenderer->transform->position.x += move_x;
		float move_y = keyFrames[i]->y / (keyFrames[i]->frame - keyFrames[currentKeyFrameIndex]->frame);
		pAnimRenderer->transform->position.y += move_y;

		//col
		float r_diff = (keyFrames[i]->color.r - keyFrames[currentKeyFrameIndex]->color.r) * rate;
		float g_diff = (keyFrames[i]->color.g - keyFrames[currentKeyFrameIndex]->color.g) * rate;
		float b_diff = (keyFrames[i]->color.b - keyFrames[currentKeyFrameIndex]->color.b) * rate;
		float a_diff = (keyFrames[i]->color.a - keyFrames[currentKeyFrameIndex]->color.a) * rate;
		pAnimRenderer->SetColor(keyFrames[currentKeyFrameIndex]->color.r + r_diff, keyFrames[currentKeyFrameIndex]->color.g + g_diff,
			keyFrames[currentKeyFrameIndex]->color.b + b_diff, keyFrames[currentKeyFrameIndex]->color.a + a_diff);

		//scale
		float scaleX_diff = (keyFrames[i]->scaleX - keyFrames[currentKeyFrameIndex]->scaleX) * rate;
		float scaleY_diff = (keyFrames[i]->scaleY - keyFrames[currentKeyFrameIndex]->scaleY) * rate;
		pAnimRenderer->transform->SetScale(keyFrames[currentKeyFrameIndex]->scaleX + scaleX_diff,
			keyFrames[currentKeyFrameIndex]->scaleY + scaleY_diff);

		//rot
		float rot_diff = (keyFrames[i]->rot - keyFrames[currentKeyFrameIndex]->rot) * rate;
		pAnimRenderer->transform->rotation.z = keyFrames[currentKeyFrameIndex]->rot + rot_diff;

		break;
	}

	frameCount++; //フレームカウント更新
}

void SpriteAnimation::AnimOff() {
	frameCount = 0;
	currentKeyFrameIndex = -1;
}