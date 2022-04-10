#include "../../../framework.h"
#include "../../../environment.h"

using namespace MyFrameWork;

SpriteAnimation::SpriteAnimation(const WCHAR* animation_file, bool loop) {
	isLoop = loop;

	FILE* fp = NULL;
	WCHAR _key[256] = { 0 };

	//ファイルを開く
	_wfopen_s(&fp, animation_file, L"rt");
	if (fp == NULL) {
		return;
	}

	KeyFrame* setKey = new KeyFrame();

	while (!feof(fp))
	{
		//キーワード読み込み
		fwscanf_s(fp, L"%s", _key, 256);
		
		if (wcscmp(_key, L"newkey") == 0) {
			//格納し忘れがあれば格納
			if (setKey != NULL) {
				keyFrames.emplace_back(setKey);
				setKey = nullptr;
			}
			//新たなキーの生成
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

	//キー情報の反映
	//スプライトはNULLの場合なにも変更を加えないため、各キーの情報としては反映させない
	//Positionも移動量をとっているため次のキーには反映させない

	//最初のキーは現在のスプライトの情報を引き継ぐ
	keyFrames[0]->color = pAnimRenderer->GetColor();
	keyFrames[0]->scaleX = pAnimRenderer->transform->scale.x;
	keyFrames[0]->scaleY = pAnimRenderer->transform->scale.y;
	keyFrames[0]->rot = pAnimRenderer->transform->rotation.z;

	//意図的にアニメーションとして変更を加えられていない項目は一つ前のキーの情報を引き継ぐ
	for (int i = 0; i < keyFrames.size(); i++) {
		if (i == 0) continue; //最初のキーはスキップ
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
	//終了トリガーを外す
	isEnd = false;

	//各キーフレームを照合する
	for (int i = 0; i < keyFrames.size(); i++) {
		if (curKeyIndex >= i) continue; //到達しているキーフレームより小さい場合はとばす

		/*フレーム数が同じ場合、全ての状態を変更*/
		if (frameCount == keyFrames[i]->frame) {
			curKeyIndex = i; //到達キーフレーム更新

			/*スプライト状態変更*/
			SetKeySpriteState(keyFrames[curKeyIndex]);
			SetKeyState(keyFrames[curKeyIndex]);

			/*キーフレームの最後に到達した場合*/
			if (curKeyIndex == keyFrames.size() - 1) {
				AnimOff();
 				return;
			}

			break;
		}

		if (curKeyIndex < 0) break; //初期値の場合終了

		//次のキーフレームに向けて徐々に状態を遷移
		//次のキーフレームまでの進捗割合(現在のフレーム数-到達しているキーフレーム数) / (次のキーフレーム数-到達しているキーフレーム数)
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

	frameCount++; //フレームカウント更新
}
void SpriteAnimation::AnimOff() {
	isEnd = true;
	frameCount = 0;
	curKeyIndex = -1;
}

SpriteAnimation::KeyFrame::~KeyFrame() {
	delete pSprite;
}

//キーフレームのスプライト状態の反映
void SpriteAnimation::SetKeySpriteState(KeyFrame* key) {
	//スプライトが以前の状態と違う場合処理
	if (key->pSprite != pAnimRenderer->pRenderSprite.get() && key->pSprite != nullptr) {
		pAnimRenderer->pRenderSprite = noDel_ptr<Sprite>(key->pSprite);
		pAnimRenderer->SetDefaultUV();
	}
}

//次のキーに向けて変化
void SpriteAnimation::TransPos(KeyFrame* key) {
	//pos(キーフレームのx,yの長さを次のキーまでのフレーム数で割った値(1フレームでの移動量)をPositionに足す)
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

//特定のキーフレーム状態に設定(座標以外)
void  SpriteAnimation::SetKeyState(KeyFrame* key) {
	SetKeySpriteState(key);
	//色
	pAnimRenderer->SetColor(key->color.r, key->color.g,	key->color.b, key->color.a);
	//スケール
	pAnimRenderer->transform->SetScale(key->scaleX,key->scaleY);
	//回転
	pAnimRenderer->transform->rotation.z = key->rot;
}
