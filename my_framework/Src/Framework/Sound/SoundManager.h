/*-----------------------------------------------------------

	サウンドマネージャークラス
		シーンにインスタンス化されたサウンドの管理
		サウンドの所有権をもつ

-------------------------------------------------------------*/
#pragma once

class SoundManager {
public:
	SoundManager() {}
	~SoundManager(){
		for (auto& sd : sounds) {
			delete sd;
		}
		sounds.clear();
	}

	
	noDel_ptr<Sound> CreateSound(Sound* instance) {
		sounds.emplace_back(instance);
		noDel_ptr<Sound> p = noDel_ptr<Sound>(sounds.back());
		return p;
	}
	noDel_ptr<Sound> CreateSound(LPCWSTR file) {
		Sound* instance = new Sound(file);
		sounds.emplace_back(instance);
		noDel_ptr<Sound> p = noDel_ptr<Sound>(sounds.back());
		return p;
	}

private:
	std::vector<Sound*> sounds;
};
