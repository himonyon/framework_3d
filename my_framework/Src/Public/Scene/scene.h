/*-----------------------------------------------------------

	シーンクラス（基底クラス）
		各シーンの共通のクラス・抽象クラス

-------------------------------------------------------------*/
#pragma once
#include "../InputConfig.h"

enum class eRenderOrder {
	Background = -30,
	BackObject = -20,
	BackUI = -10,
	Object = 0,
	UI = 10,
	FrontObject = 20,
	FrontUI = 30,
};

class Scene : public GameObjectManager,public SpriteManager, public MeshManager {
public:
	bool isInitialized = false;

public:
	//コンストラクタ
	Scene();

	//デストラクタ
	virtual ~Scene();

	//初期化
	virtual void Initialize() = 0;

	virtual void Terminate() = 0;

	//処理
	virtual void Execute();

	//描画
	virtual void Render();
};