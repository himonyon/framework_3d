#include "../../framework.h"
#include "../../environment.h"

//一度すべての配列の情報を初期化
SpriteAnimation::SpriteAnimation(Sprite* pSprite, bool loop) {
	showSprite = pSprite; //現在表示するスプライト
	isLoop = loop;

	if (pScene != NULL) {
		pScene->RegisterAnimation(this);
	}
}

void SpriteAnimation::CreateKeyFlame(int keyNum) {
	for (int i = 0; i < keyNum + 1; i++) {
		if (i < keyFlame.size()) continue;
		keyFlame.emplace_back(new KeyFlame());

		keyFlame[i]->flame = 0;
		keyFlame[i]->pSprite = showSprite;
		keyFlame[i]->x = showSprite->posX;
		keyFlame[i]->y = showSprite->posY;
		keyFlame[i]->r = showSprite->vtx[0].r;
		keyFlame[i]->g = showSprite->vtx[0].g;
		keyFlame[i]->b = showSprite->vtx[0].b;
		keyFlame[i]->a = showSprite->vtx[0].a;
		keyFlame[i]->scaleX = showSprite->scaleX;
		keyFlame[i]->scaleY = showSprite->scaleY;
		keyFlame[i]->rot = showSprite->rot;
	}
}

void SpriteAnimation::AnimOn() {
	//各キーフレームを照合する
	for (int i = 0; i < keyFlame.size(); i++) {
		if (currentKeyFlameIndex >= i) continue; //到達しているキーフレームより小さい場合はとばす
		if (keyFlame[i]->flame < flameCount) { //未到達指定キーフレーム数より現在の経過フレーム数が大きい場合
			if (i == keyFlame.size() - 1) {
				if (isLoop) AnimLoop();
				else AnimOff();
				return;
			}
			continue;
		}

		//フレーム数が同じ場合、全ての状態を変更
		if (flameCount == keyFlame[i]->flame) {
			currentKeyFlameIndex = i;

			//スプライト状態変更
			if (keyFlame[i]->pSprite != showSprite) {
				showSprite->SetRenderEnable(false);
				keyFlame[i]->pSprite->SetRenderEnable(true);
				showSprite = keyFlame[i]->pSprite;
			}

			if (showSprite->isRenderEnable() == false) showSprite->SetRenderEnable(true);

			showSprite->SetPosition(keyFlame[i]->x, keyFlame[i]->y);
			showSprite->SetColor(keyFlame[i]->r, keyFlame[i]->g, keyFlame[i]->b, keyFlame[i]->a);
			showSprite->SetScale(keyFlame[i]->scaleX, keyFlame[i]->scaleY);
			showSprite->rot = keyFlame[i]->rot;

			//キーフレームの最後に到達した場合
			if (currentKeyFlameIndex == keyFlame.size() - 1) {
				if (isLoop) AnimLoop();
				else AnimOff();
 				return;
			}

			break;
		}

		//
		//次のキーフレームに向けて徐々に状態を遷移
		// 
		
		if (currentKeyFlameIndex < 0) break; //初期値の場合終了

		//次のキーフレームまでの進捗割合(現在のフレーム数-到達しているキーフレーム数) / (次のキーフレーム数-到達しているキーフレーム数)
		float rate = (flameCount - keyFlame[currentKeyFlameIndex]->flame) / 
			(keyFlame[i]->flame - keyFlame[currentKeyFlameIndex]->flame);

		//pos
		float move_x = (keyFlame[i]->x - keyFlame[currentKeyFlameIndex]->x) * rate;
		showSprite->posX = keyFlame[currentKeyFlameIndex]->x + move_x;
		float move_y = (keyFlame[i]->y - keyFlame[currentKeyFlameIndex]->y) * rate;
		showSprite->posY = keyFlame[currentKeyFlameIndex]->y + move_y;

		//col
		float r_diff = (keyFlame[i]->r - keyFlame[currentKeyFlameIndex]->r) * rate;
		float g_diff = (keyFlame[i]->g - keyFlame[currentKeyFlameIndex]->g) * rate;
		float b_diff = (keyFlame[i]->b - keyFlame[currentKeyFlameIndex]->b) * rate;
		float a_diff = (keyFlame[i]->a - keyFlame[currentKeyFlameIndex]->a) * rate;
		showSprite->SetColor(keyFlame[currentKeyFlameIndex]->r + r_diff, keyFlame[currentKeyFlameIndex]->g + g_diff,
			keyFlame[currentKeyFlameIndex]->b + b_diff, keyFlame[currentKeyFlameIndex]->a + a_diff);

		//scale
		float scaleX_diff = (keyFlame[i]->scaleX - keyFlame[currentKeyFlameIndex]->scaleX) * rate;
		float scaleY_diff = (keyFlame[i]->scaleY - keyFlame[currentKeyFlameIndex]->scaleY) * rate;
		showSprite->SetScale(keyFlame[currentKeyFlameIndex]->scaleX + scaleX_diff,
			keyFlame[currentKeyFlameIndex]->scaleY + scaleY_diff);

		//rot
		float rot_diff = (keyFlame[i]->rot - keyFlame[currentKeyFlameIndex]->rot) * rate;
		showSprite->rot = keyFlame[currentKeyFlameIndex]->rot + rot_diff;

		break;
	}

	flameCount++; //フレームカウント更新
}

void SpriteAnimation::AnimOff() {
	animEnable = false;
	flameCount = 0;
	currentKeyFlameIndex = -1;
}
void SpriteAnimation::AnimLoop() {
	flameCount = 0;
	currentKeyFlameIndex = -1;
}


bool SpriteAnimation::isAnimEnable() {
	return animEnable;
}
void SpriteAnimation::SetAnimEnable(bool flag) {
	animEnable = flag;
}

void SpriteAnimation::SetKeyFlameSprite(int index, Sprite* sprite) {
	if (index >= keyFlame.size()) CreateKeyFlame(index);
	keyFlame[index]->pSprite = sprite;
}
void SpriteAnimation::SetKeyFlameSprite(int index, Sprite* sprite, float flame) {
	if (index >= keyFlame.size()) CreateKeyFlame(index);
	keyFlame[index]->pSprite = sprite;
	keyFlame[index]->flame = flame;
}

void SpriteAnimation::SetKeyFlamePos(int index, float x, float y) {
	if (index >= keyFlame.size()) CreateKeyFlame(index);
	keyFlame[index]->x = x;
	keyFlame[index]->y = y;
}
void SpriteAnimation::SetKeyFlamePos(int index, float x, float y, float flame) {
	if (index >= keyFlame.size()) CreateKeyFlame(index);
	keyFlame[index]->x = x;
	keyFlame[index]->y = y;
	keyFlame[index]->flame = flame;
}

void SpriteAnimation::SetKeyFlameCol(int index, float r, float g, float b, float a) {
	if (index >= keyFlame.size()) CreateKeyFlame(index);
	keyFlame[index]->r = r;
	keyFlame[index]->g = g;
	keyFlame[index]->b = b;
	keyFlame[index]->a = a;
}
void SpriteAnimation::SetKeyFlameCol(int index, float r, float g, float b, float a, float flame) {
	if (index >= keyFlame.size()) CreateKeyFlame(index);
	keyFlame[index]->r = r;
	keyFlame[index]->g = g;
	keyFlame[index]->b = b;
	keyFlame[index]->a = a;
	keyFlame[index]->flame = flame;
}

void SpriteAnimation::SetKeyFlameScale(int index, float x, float y) {
	if (index >= keyFlame.size()) CreateKeyFlame(index);
	keyFlame[index]->scaleX = x;
	keyFlame[index]->scaleY = y;
}
void SpriteAnimation::SetKeyFlameScale(int index, float x, float y, float flame) {
	if (index >= keyFlame.size()) CreateKeyFlame(index);
	keyFlame[index]->scaleX = x;
	keyFlame[index]->scaleY = y;
	keyFlame[index]->flame = flame;
}

void SpriteAnimation::SetKeyFlameRot(int index, float rot) {
	if (index >= keyFlame.size()) CreateKeyFlame(index);
	keyFlame[index]->rot = rot;
}
void SpriteAnimation::SetKeyFlameRot(int index, float rot, float flame) {
	if (index >= keyFlame.size()) CreateKeyFlame(index);
	keyFlame[index]->rot = rot;
	keyFlame[index]->flame = flame;
}