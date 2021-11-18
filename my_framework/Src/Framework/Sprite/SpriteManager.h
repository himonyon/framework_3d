#pragma once

class SpriteManager {
public:
	SpriteManager() {};
	~SpriteManager() {
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
	
	noDel_ptr<Sprite> CreateSprite(const WCHAR* texture_file, float left_u = 0.0f, float right_u = 1.0f, float top_v = 0.0f, float bottom_v = 1.0f) {
		Sprite* instance = new Sprite(texture_file, left_u, right_u, top_v, bottom_v);
		sprites.emplace_back(instance);
		noDel_ptr<Sprite> p = noDel_ptr<Sprite>(sprites.back());
		return p;
	}

private:
	std::vector<Sprite*> sprites;
};
