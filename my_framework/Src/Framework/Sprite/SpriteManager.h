/*-----------------------------------------------------------

	スプライト管理クラス
		シーンに作成されたスプライトの管理
		配列でスプライトの所有権を所有し管理

-------------------------------------------------------------*/
#pragma once


class SpriteManager {
public:
	void Destroy() {
		for (auto& sprite : sprites) {
			delete sprite;
		}
		sprites.clear();
	}

	noDel_ptr<Sprite> CreateSprite(Sprite* instance) {
		sprites.emplace_back(instance);
		noDel_ptr<Sprite> p = noDel_ptr<Sprite>(sprites.back());
		return p;
	}

private:
	std::vector<Sprite*> sprites;
};
