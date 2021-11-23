/*-----------------------------------------------------------

	スプライトアニメーションクラス
		キーをフレーム数と共に設定して各キーの時間になると
		その処理を行うアニメーション機能

-------------------------------------------------------------*/
#pragma once

#define MAX_ANIMATION2D (50)

class SpriteAnimation {
private:
	bool animEnable = false;
	bool isLoop = false;
	float flameCount = 0; //アニメーションフレーム数

	noDel_ptr<GameObject2D> pAnimObj = NULL; //表示するスプライト
	int currentKeyFrameIndex = -1; //現在到達しているキーフレーム配列番号

public:
	//アニメーションのキーフレーム
	class KeyFrame {
	public:
		noDel_ptr<Sprite> pSprite = 0;
		float flame = 0;
		float x = 0, y = 0;
		stColor4 color;;
		float scaleX = 0, scaleY = 0;
		float rot = 0;
	};

private:
	std::vector<KeyFrame*> keyFrame;

public:
	SpriteAnimation(noDel_ptr<GameObject2D> obj, bool loop = false);

	bool isAnimEnable();
	void SetAnimEnable(bool flag);

	void AnimOn(); //アニメーション実行
	void AnimOff(); //アニメーション終了


	//arrayIndexは配列の番号を指定
	void SetKeyFrameSprite(int keyIndex, noDel_ptr<Sprite> sprite);
	void SetKeyFrameSprite(int keyIndex, noDel_ptr<Sprite> sprite, float flame);

	void SetKeyFramePos(int keyIndex, float x, float y);
	void SetKeyFramePos(int keyIndex, float x, float y, float flame);

	void SetKeyFrameCol(int keyIndex, float r, float g, float b, float a);
	void SetKeyFrameCol(int keyIndex, float r, float g, float b, float a, float flame);

	void SetKeyFrameScale(int keyIndex, float x, float y);
	void SetKeyFrameScale(int keyIndex, float x, float y, float flame);

	void SetKeyFrameRot(int keyIndex, float rot);
	void SetKeyFrameRot(int keyIndex, float rot, float flame);

private:
	void AnimLoop(); //アニメーションをもう一度再生
	void CreateKeyFrame(int keyNum);
};