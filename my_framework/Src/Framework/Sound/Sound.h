/*-----------------------------------------------------------

	�T�E���h�N���X
		�T�E���h�̌ʏ��̊Ǘ�

-------------------------------------------------------------*/
#pragma once

class Sound {
private:
	static IXAudio2* pXAudio2;
	static IXAudio2MasteringVoice* pMasteringVoice;

	//�T�E���h�t�@�C������
	IXAudio2SourceVoice* pSourceVoice;
	BYTE* pWavBuffer;//�g�`�f�[�^�i�t�H�[�}�b�g�����܂܂Ȃ��A�����ɔg�`�f�[�^�̂݁j
	DWORD s_dwWavSize;//�g�`�f�[�^�̃T�C�Y

	UINT64 currentSound = 0; //�Đ���~���܂łɏ������ꂽ�T���v���̍��v(�Đ������m�F�p)

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