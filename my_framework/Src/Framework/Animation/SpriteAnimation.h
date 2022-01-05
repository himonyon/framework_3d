/*-----------------------------------------------------------

	スプライトアニメーションクラス
		キーをフレーム数と共に設定して各キーの時間になると
		その処理を行うアニメーション機能

-------------------------------------------------------------*/
#pragma once

class SpriteAnimation {
private:
	bool isLoop = false;

	bool isEnd = false;

	float frameCount = 0; //進んだフレーム数

	int curKeyIndex = -1; //現在到達しているキーフレーム配列番号

public:
	noDel_ptr<Renderer2D> pAnimRenderer = NULL; //表示するスプライトレンダラー
	//アニメーションのキーフレーム
	class KeyFrame {
	public:
		Sprite* pSprite = 0;
		float frame = 0;
		float x = 0, y = 0;
		stColor4 color = {};
		float scaleX = 0, scaleY = 0;
		float rot = 0;

	public:
		KeyFrame() {};
		~KeyFrame();
	};

private:
	std::vector<KeyFrame*> keyFrames;

public:
	SpriteAnimation(const WCHAR* animation_file, bool loop = false);
	~SpriteAnimation();

	void AnimOn(); //アニメーション実行
	void AnimOff(); //アニメーション終了

	bool IsLoop() { return isLoop; }
	bool IsEnd() { return isEnd; }
};