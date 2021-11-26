/*-----------------------------------------------------------

	シーンクラス（基底クラス）
		各シーンの共通のクラス・抽象クラス

-------------------------------------------------------------*/
#pragma once
#include "../InputConfig.h"

class Scene : public GameObjectManager, public SpriteManager, public MeshManager, public SoundManager, public AnimationManager {
public:

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

	//当たり判定
	enum class eCollideState	//衝突の状態を表す列挙子(enum)
	{
		None,		//衝突していない
		FromLeft,	//左側から衝突
		FromRight,	//右側から衝突
		FromTop,	//上側から衝突
		FromBottom,	//下側から衝突
	};
	//bool resolve : 衝突の解消を行う(p1を動かして、p2は固定する)
	static eCollideState IsCollide2D(noDel_ptr<GameObject2D> p1, noDel_ptr<GameObject2D> p2, bool resolve = true);

	//int型の数字の＋１ー１を指定の幅(0~maxNum)で行う
	static int CountUpInRange(int target, int maxRange);
	static int CountDownInRange(int target, int maxRange);
};