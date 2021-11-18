#pragma once
#include "../../Framework/Input/InputConfig.h"

class Scene : public GameObjectManager, public SpriteManager, public SoundManager, public AnimationManager,
	 public MeshManager
{
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
	eCollideState IsCollide2D(noDel_ptr<GameObject2D> p1, noDel_ptr<GameObject2D> p2, bool resolve = true);
};