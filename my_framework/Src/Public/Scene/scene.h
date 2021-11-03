#pragma once

#define MaxSprites (256) //１シーンに管理できる画像数
#define MaxAnimation (64) //１シーンに管理できるアニメーション

class Scene {
private:
	Sprite* pObject2D_array[MaxSprites];
	std::vector<SpriteAnimation*> pSpriteAnimation_array;

public:
	//コンストラクタ
	Scene();

	//デストラクタ
	virtual ~Scene();

	//初期化
	virtual bool Initialize() = 0;

	virtual void Terminate() = 0;

	//処理
	virtual void Execute();

	//描画
	virtual void Render();


	bool RegisterObject(Sprite* p);
	void DeleteObject(Sprite* p);

	bool RegisterAnimation(SpriteAnimation* anim);


	//当たり判定
	static bool isCollider(Sprite* collider, float x, float y); //画像と座標
	static bool isCollider(Sprite* collider, int x, int y); //画像と座標
private:
	void RenderOrderSort(int start, int end);
};