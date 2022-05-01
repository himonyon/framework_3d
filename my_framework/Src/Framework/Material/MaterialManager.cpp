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
	//すでにある場合は追加しない
	if (umMaterials.count(matName) != 0) return false;
	//追加
	umMaterials[matName] = mat;

	return true;
}
noDel_ptr<stMaterial> MaterialManager::GetMaterial(std::string matName) {
	//マテリアルがなければNULLを返す
	if (umMaterials.count(matName) == 0) return NULL;
	//対象マテリアルを返す
	return noDel_ptr<stMaterial>(umMaterials[matName]);
}
