#include "../../../environment.h"

using namespace MyFrameWork;

MaterialManager::MaterialManager() {
	stMaterial* startMat = new stMaterial();
	AddMaterial("default", startMat);
}

MaterialManager::~MaterialManager() {
	for (auto& mat : umMaterials) {
		delete mat.second;
	}
	umMaterials.clear();
}

bool MaterialManager::AddMaterial(std::string matName, stMaterial* mat) {
	//���łɂ���ꍇ�͒ǉ����Ȃ�
	if (umMaterials.count(matName) != 0) return false;
	//�ǉ�
	umMaterials[matName] = mat;

	return true;
}
noDel_ptr<stMaterial> MaterialManager::GetMaterial(std::string matName) {
	//�}�e���A�����Ȃ����NULL��Ԃ�
	if (umMaterials.count(matName) == 0) return NULL;
	//�Ώۃ}�e���A����Ԃ�
	return noDel_ptr<stMaterial>(umMaterials[matName]);
}
