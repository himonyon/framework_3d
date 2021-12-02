/*-----------------------------------------------------------

	�X�v���C�g�Ǘ��N���X
		�V�[���ɍ쐬���ꂽ�X�v���C�g�̊Ǘ�
		�z��ŃX�v���C�g�̏��L�������L���Ǘ�

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
