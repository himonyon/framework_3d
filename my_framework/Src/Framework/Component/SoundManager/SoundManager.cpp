#include "../../../../framework.h"
#include "../../../../environment.h"

SoundManager::SoundManager() : Component(eComponentType::Sound) {
}
SoundManager::~SoundManager() {
	for (auto& sound : umSound) {
		delete sound.second;
	}
	umSound.clear();
}

void SoundManager::Play(std::string name) {
	umSound[name]->Play();
}
void SoundManager::Resume(std::string name) {
	umSound[name]->Resume();
}
void SoundManager::Stop() {
	for (auto& sound : umSound) {
		sound.second->Stop();
	}
}
void SoundManager::Stop(std::string name) {
	umSound[name]->Stop();
}
void SoundManager::SetVolume(float volume) {
	for (auto& sound : umSound) {
		sound.second->SetVolume(volume);
	}
}
void SoundManager::SetVolume(std::string name, float volume) {
	umSound[name]->SetVolume(volume);
}
bool SoundManager::IsPlaying(std::string name) {
	return umSound[name]->isPlaying();
}

void SoundManager::AddSound(std::string name, LPCWSTR file) {
	umSound[name] = new Sound(file);
}