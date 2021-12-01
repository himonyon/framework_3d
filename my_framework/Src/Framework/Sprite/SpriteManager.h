/*-----------------------------------------------------------

	スプライト管理クラス
		シーンに作成されたスプライトの管理
		配列でスプライトの所有権を所有し管理

-------------------------------------------------------------*/
#pragma once


class SpriteManager {
public:
	static void Destroy() {
		for (auto& sprite : sprites) {
			delete sprite;
		}
		sprites.clear();
	}

	static noDel_ptr<Sprite> CreateSprite(Sprite* instance) {
		sprites.emplace_back(instance);
		noDel_ptr<Sprite> p = noDel_ptr<Sprite>(sprites.back());
		return p;
	}

	static noDel_ptr<Sprite> Find(char* name) {
		for (auto& sprite : sprites) {
			if (strcmp(name, sprite->name) == 0) return noDel_ptr<Sprite>(sprite);
		}
		return nullptr;
	}

private:
	static std::vector<Sprite*> sprites;
};
