#pragma once

#define MAX_ANIMATION2D (50)

class SpriteAnimation {
private:
	bool animEnable = false;
	bool isLoop = false;
	float flameCount = 0; //アニメーションフレーム数


	Sprite* showSprite = NULL; //表示するスプライト
	int currentKeyFlameIndex = -1; //現在到達しているキーフレーム配列番号

public:
	//アニメーションのキーフレーム
	class KeyFlame {
	public:
		Sprite* pSprite;
		float flame;
		float x, y;
		float r, b, g, a;
		float scaleX, scaleY;
		float rot;
	};

private:
	std::vector<KeyFlame*> keyFlame;

public:
	SpriteAnimation(Sprite* pSprite, bool loop = false);

	bool isAnimEnable();
	void SetAnimEnable(bool flag);

	void AnimOn(); //アニメーション実行
	void AnimOff(); //アニメーション終了


	//arrayIndexは配列の番号を指定
	void SetKeyFlameSprite(int index, Sprite* sprite);
	void SetKeyFlameSprite(int index, Sprite* sprite, float flame);

	void SetKeyFlamePos(int index, float x, float y);
	void SetKeyFlamePos(int index, float x, float y, float flame);

	void SetKeyFlameCol(int index, float r, float g, float b, float a);
	void SetKeyFlameCol(int index, float r, float g, float b, float a, float flame);

	void SetKeyFlameScale(int index, float x, float y);
	void SetKeyFlameScale(int index, float x, float y, float flame);

	void SetKeyFlameRot(int index, float rot);
	void SetKeyFlameRot(int index, float rot, float flame);

private:
	void AnimLoop(); //アニメーションをもう一度再生
	void CreateKeyFlame(int keyNum);
};