/*-----------------------------------------------------------

	サウンドマネージャーコンポーネント
		シーンにインスタンス化されたサウンドの管理
		サウンドの所有権をもつ

-------------------------------------------------------------*/
#pragma once

class SoundManager : public Component {
private:
	std::unordered_map<std::string, Sound*> umSound;

public:
	SoundManager();
	~SoundManager();
	
	//鳴らす
	void Play(std::string name);
	//再開
	void Resume(std::string name);
	//止める(全体)
	void Stop();
	//止める(単体)
	void Stop(std::string name);
	//音量調節(全体)
	void SetVolume(float volume);
	//音量調節(単体)
	void SetVolume(std::string name, float volume);
	//鳴っているか(単体)
	bool IsPlaying(std::string name);
	
	//サウンドを加える
	void AddSound(std::string name, LPCWSTR file);
	
};
