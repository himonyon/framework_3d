#include "../../../framework.h"
#include "../../../environment.h"

IXAudio2* Sound::pXAudio2 = NULL;
IXAudio2MasteringVoice* Sound::pMasteringVoice = NULL;


bool Sound::InitSound() {
	if(FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED))) {
		return FALSE;
	}

	if (FAILED(XAudio2Create(&pXAudio2, XAUDIO2_DEBUG_ENGINE)))
	{
		CoUninitialize();
		return FALSE;
	}
	if (FAILED(pXAudio2->CreateMasteringVoice(&pMasteringVoice)))
	{
		CoUninitialize();
		return FALSE;
	}

	return TRUE;
}

void Sound::DestroySound() {
	
	pMasteringVoice->DestroyVoice();
	SAFE_RELEASE(pXAudio2);
}

Sound::Sound(LPCWSTR file) {
	this->Load(file);
}

Sound::~Sound() {
	s_dwWavSize = NULL;
	pSourceVoice->DestroyVoice();
}

bool Sound::Load(LPCWSTR szFileName) {
	HMMIO hMmio = NULL;//WindowsマルチメディアAPIのハンドル(WindowsマルチメディアAPIはWAVファイル関係の操作用のAPI)
	DWORD dwWavSize = 0;//WAVファイル内　WAVデータのサイズ（WAVファイルはWAVデータで占められているので、ほぼファイルサイズと同一）
	WAVEFORMATEX* pwfex;//WAVのフォーマット 例）16ビット、44100Hz、ステレオなど
	MMCKINFO ckInfo;//　チャンク情報
	MMCKINFO riffckInfo = {0};// 最上部チャンク（RIFFチャンク）保存用
	PCMWAVEFORMAT pcmWaveForm;
	//WAVファイル内のヘッダー情報（音データ以外）の確認と読み込み
	hMmio = mmioOpen((LPWSTR)szFileName, NULL, MMIO_ALLOCBUF | MMIO_READ);
	//ファイルポインタをRIFFチャンクの先頭にセットする
	mmioDescend(hMmio, &riffckInfo, NULL, 0);
	// ファイルポインタを'f' 'm' 't' ' ' チャンクにセットする
	ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK);
	//フォーマットを読み込む
	mmioRead(hMmio, (HPSTR)&pcmWaveForm, sizeof(pcmWaveForm));
	pwfex = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX)];
	memcpy(pwfex, &pcmWaveForm, sizeof(pcmWaveForm));
	pwfex->cbSize = 0;
	mmioAscend(hMmio, &ckInfo, 0);
	// WAVファイル内の音データの読み込み	
	ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK);//データチャンクにセット
	dwWavSize = ckInfo.cksize;
	pWavBuffer= new BYTE[dwWavSize];
	DWORD dwOffset = ckInfo.dwDataOffset;
	mmioRead(hMmio, (HPSTR)pWavBuffer, dwWavSize);
	//ソースボイスにデータを詰め込む	
	//ソースボイスにデータを詰め込む	
	if (FAILED(pXAudio2->CreateSourceVoice(&pSourceVoice, pwfex)))
	{
		MessageBox(0, L"ソースボイス作成失敗", 0, MB_OK);
		return FALSE;
	}
	s_dwWavSize = dwWavSize;

	return TRUE;
}

void Sound::SetVolume(float volume) {
	pSourceVoice->SetVolume(volume);
}

bool Sound::isPlaying() {
	XAUDIO2_VOICE_STATE state;
	pSourceVoice->GetState(&state);

	if (state.SamplesPlayed != currentSound) {
		return true;
	}

	return  false;
}

void Sound::Stop() {
	XAUDIO2_VOICE_STATE state;
	pSourceVoice->GetState(&state);
	if (state.BuffersQueued != 0) {
		pSourceVoice->Stop();
		currentSound = state.SamplesPlayed;
	}
}

void Sound::Play() {
	//いったん止める
	pSourceVoice->Stop();
	//初期化
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.pAudioData = pWavBuffer;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = s_dwWavSize;
	pSourceVoice->FlushSourceBuffers();
	if (FAILED(pSourceVoice->SubmitSourceBuffer(&buffer)))
	{
		MessageBox(0, L"ソースボイスにサブミット失敗", 0, MB_OK);
		return;
	}
	//再生
	pSourceVoice->Start(0, XAUDIO2_COMMIT_NOW);
}

void Sound::Resume() {
	pSourceVoice->Start();
}

bool Sound::PlaySound() {
	return true;
}