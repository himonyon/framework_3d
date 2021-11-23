/*-----------------------------------------------------------

	サウンドクラス
		サウンドの個別情報の管理

-------------------------------------------------------------*/
#pragma once

class Sound {
private:
	static IXAudio2* pXAudio2;
	static IXAudio2MasteringVoice* pMasteringVoice;

	//サウンドファイルごと
	IXAudio2SourceVoice* pSourceVoice;
	BYTE* pWavBuffer;//波形データ（フォーマット等を含まない、純粋に波形データのみ）
	DWORD s_dwWavSize;//波形データのサイズ

	UINT64 currentSound = 0; //再生停止時までに処理されたサンプルの合計(再生中か確認用)

private:
	bool PlaySound();
	bool Load(LPCWSTR szFileName);
public:
	Sound(LPCWSTR file);
	~Sound();

	static bool InitSound();
	static void DestroySound();


	void SetVolume(float volume);

	bool isPlaying();
	void Play();
	void Stop();
};