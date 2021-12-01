/*-----------------------------------------------------------

	シーンクラス（基底クラス）
		各シーンの共通のクラス・抽象クラス

-------------------------------------------------------------*/
#pragma once
#include "../InputConfig.h"



class Scene : public GameObjectManager, public MeshManager, public SoundManager {
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

	//int型の数字の＋１ー１を指定の幅(0~maxNum)で行う
	static int CountUpInRange(int target, int maxRange);
	static int CountDownInRange(int target, int maxRange);
};