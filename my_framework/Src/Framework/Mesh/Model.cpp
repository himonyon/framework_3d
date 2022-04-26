#include "../../../environment.h"

using namespace MyFrameWork;

Model::Model(std::vector<noDel_ptr<Mesh>> vMeshes) {
	//配列を生ポインタにしてコピー
	for (auto& mesh : vMeshes) {
		this->vMeshes.emplace_back(mesh.get());
	}
}
Model::~Model() {
	for (auto& mesh : vMeshes) {
		delete mesh;
	}
}

int Model::GetRootCount() {
	//ルートメッシュの数の取得
	int _rootNum = 0;
	for (auto& mesh : vMeshes) {
		if (mesh->GetParent() == NULL) _rootNum++;
	}
	return _rootNum;
}

noDel_ptr<Mesh> Model::GetRootMesh(int index) {
	//ルートメッシュの数の取得
	int _rootNum = GetRootCount();
	if (_rootNum < index) index = _rootNum; //最大数を超えた場合、最後のルートメッシュの番号を返す

	//指定番号目のルートメッシュを返す
	int _count = 0;
	for (auto& mesh : vMeshes) {
		if (mesh->GetParent() != NULL) continue;
		if (_count != index) {
			_count++;
			continue;
		}
		//返す
		return noDel_ptr<Mesh>(mesh);
	}

	//条件に合うものがなければNULLを返す
	return NULL;
}

int Model::GetMeshCount() {
	return (int)vMeshes.size();
}