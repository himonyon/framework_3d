#include "../../framework.h"
#include "../../environment.h"


//コンストラクタ
Scene::Scene()
{
}

//デストラクタ
Scene::~Scene()
{
}

//処理
void Scene::Execute()
{
	//画像の処理
	GameObjectManager::Execute();
	AnimationManager::Execute();
}

void Scene::Render()
{
	GameObjectManager::Render();
}

//スプライト同士の衝突を検出する
//戻り値 eCollideState : 衝突の状態
//引数
//vnBlock2D *p1 : 衝突を検出する1つ目のオブジェクト
//vnBlock2D *p2 : 衝突を検出する2つ目のオブジェクト
//bool resolve : 衝突の解消を行う(p1を動かして、p2は固定する)
Scene::eCollideState Scene::IsCollide2D(noDel_ptr<GameObject2D> p1, noDel_ptr<GameObject2D> p2, bool resolve)
{
	//引数のエラーチェック
	if (p1 == NULL || p2 == NULL)
	{
		return eCollideState::None;
	}

	//戻り値用の変数
	eCollideState hit = eCollideState::None;

	//衝突の検出
	float rx = 0.0f;	//重なっている長方形(Rectangle)の幅
	float ry = 0.0f;	//重なっている長方形(Rectangle)の高さ

	//横(X)方向

	//スプライトの中心同士の座標の差分(differencial)
	float dx = fabsf(p1->position.x - p2->position.x);
	//2つのスプライトの幅(半分)をスケールの合計
	float sx = p1->sizeX * 0.5f * p1->scale.x + p2->sizeX * 0.5f * p2->scale.x;

	if (dx < sx)	//差分が幅より小さければ横(X)方向で当たっている
	{
		//縦(Y)方向

		//スプライトの中心同士の座標の差分(differencial)
		float dy = fabsf(p1->position.y - p2->position.y);
		//2つのスプライトの高さ(半分)をスケールの合計
		float sy = p1->sizeY * 0.5f * p1->scale.y + p2->sizeY * 0.5f * p2->scale.y;

		if (dy < sy)	//差分が高さより小さければ縦(Y)方向で当たっている
		{
			//当たっていないという情報から当たったという情報に書き換える
			hit = eCollideState::FromLeft;

			//重なっている長方形のサイズを保持
			rx = sx - dx;
			ry = sy - dy;

			Font::Print(640, 40, L"r : %.3f, %.3f", rx, ry);
		}
	}

	//衝突がある場合、めり込みを解消する
	//p1を動かして、p2は動かさない
	if (hit != eCollideState::None && resolve == true)
	{
		if (rx < ry)
		{	//重なった領域(長方形)が縦長→左右から衝突
			if (p1->position.x < p2->position.x)
			{	//p1が左から衝突
				Font::Print(640, 60, L"from Left");
				p1->position.x -= rx;
				hit = eCollideState::FromLeft;
			}
			else
			{	//p1が右から衝突
				Font::Print(640, 60, L"from Right");
				p1->position.x += rx;
				hit = eCollideState::FromRight;
			}
		}
		else
		{	//重なった領域(長方形)が横長→上下から衝突
			if (p1->position.y < p2->position.y)
			{	//p1が上から衝突
				Font::Print(640, 60, L"from Top");
				p1->position.y -= ry;
				hit = eCollideState::FromTop;
			}
			else
			{	//p1が下から衝突
				Font::Print(640, 60, L"from Bottom");
				p1->position.y += ry;
				hit = eCollideState::FromBottom;
			}
		}
	}

	return hit;
}

