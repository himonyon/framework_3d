/*-----------------------------------------------------------

	�T�E���h�}�l�[�W���[�R���|�[�l���g
		�V�[���ɃC���X�^���X�����ꂽ�T�E���h�̊Ǘ�
		�T�E���h�̏��L��������

-------------------------------------------------------------*/
#pragma once

class SoundManager : public Component {
private:
	std::unordered_map<std::string, Sound*> umSound;

public:
	SoundManager();
	~SoundManager();
	
	//�炷
	void Play(std::string name);
	//�ĊJ
	void Resume(std::string name);
	//�~�߂�(�S��)
	void Stop();
	//�~�߂�(�P��)
	void Stop(std::string name);
	//���ʒ���(�S��)
	void SetVolume(float volume);
	//���ʒ���(�P��)
	void SetVolume(std::string name, float volume);
	//���Ă��邩(�P��)
	bool IsPlaying(std::string name);
	
	//�T�E���h��������
	void AddSound(std::string name, LPCWSTR file);
	
};
