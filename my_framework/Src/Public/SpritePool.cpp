#include "../../framework.h"
#include "../../environment.h"

#pragma region Sprite
std::vector<Sprite*> SpriteManager::sprites;

noDel_ptr<Sprite> SpritePool::pSp0 = 0;
noDel_ptr<Sprite> SpritePool::pSp1 = 0;
#pragma endregion

void SpritePool::Initialize() {
	pSp0 = SpriteManager::CreateSprite(new Sprite(L"Data/Image/bg.jpg", "bg", 0, 0.5f));
	pSp1 = SpriteManager::CreateSprite(new Sprite(L"Data/Image/sample.png", "sample", 0, 0.5f));
}