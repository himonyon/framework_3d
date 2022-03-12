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
	HMMIO hMmio = NULL;//Windows�}���`���f�B�AAPI�̃n���h��(Windows�}���`���f�B�AAPI��WAV�t�@�C���֌W�̑���p��API)
	DWORD dwWavSize = 0;//WAV�t�@�C�����@WAV�f�[�^�̃T�C�Y�iWAV�t�@�C����WAV�f�[�^�Ő�߂��Ă���̂ŁA�قڃt�@�C���T�C�Y�Ɠ���j
	WAVEFORMATEX* pwfex;//WAV�̃t�H�[�}�b�g ��j16�r�b�g�A44100Hz�A�X�e���I�Ȃ�
	MMCKINFO ckInfo;//�@�`�����N���
	MMCKINFO riffckInfo = {0};// �ŏ㕔�`�����N�iRIFF�`�����N�j�ۑ��p
	PCMWAVEFORMAT pcmWaveForm;
	//WAV�t�@�C�����̃w�b�_�[���i���f�[�^�ȊO�j�̊m�F�Ɠǂݍ���
	hMmio = mmioOpen((LPWSTR)szFileName, NULL, MMIO_ALLOCBUF | MMIO_READ);
	//�t�@�C���|�C���^��RIFF�`�����N�̐擪�ɃZ�b�g����
	mmioDescend(hMmio, &riffckInfo, NULL, 0);
	// �t�@�C���|�C���^��'f' 'm' 't' ' ' �`�����N�ɃZ�b�g����
	ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK);
	//�t�H�[�}�b�g��ǂݍ���
	mmioRead(hMmio, (HPSTR)&pcmWaveForm, sizeof(pcmWaveForm));
	pwfex = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX)];
	memcpy(pwfex, &pcmWaveForm, sizeof(pcmWaveForm));
	pwfex->cbSize = 0;
	mmioAscend(hMmio, &ckInfo, 0);
	// WAV�t�@�C�����̉��f�[�^�̓ǂݍ���	
	ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK);//�f�[�^�`�����N�ɃZ�b�g
	dwWavSize = ckInfo.cksize;
	pWavBuffer= new BYTE[dwWavSize];
	DWORD dwOffset = ckInfo.dwDataOffset;
	mmioRead(hMmio, (HPSTR)pWavBuffer, dwWavSize);
	//�\�[�X�{�C�X�Ƀf�[�^���l�ߍ���	
	//�\�[�X�{�C�X�Ƀf�[�^���l�ߍ���	
	if (FAILED(pXAudio2->CreateSourceVoice(&pSourceVoice, pwfex)))
	{
		MessageBox(0, L"�\�[�X�{�C�X�쐬���s", 0, MB_OK);
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
	//��������~�߂�
	pSourceVoice->Stop();
	//������
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.pAudioData = pWavBuffer;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = s_dwWavSize;
	pSourceVoice->FlushSourceBuffers();
	if (FAILED(pSourceVoice->SubmitSourceBuffer(&buffer)))
	{
		MessageBox(0, L"�\�[�X�{�C�X�ɃT�u�~�b�g���s", 0, MB_OK);
		return;
	}
	//�Đ�
	pSourceVoice->Start(0, XAUDIO2_COMMIT_NOW);
}

void Sound::Resume() {
	pSourceVoice->Start();
}

bool Sound::PlaySound() {
	return true;
}