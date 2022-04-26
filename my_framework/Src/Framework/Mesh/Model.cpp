#include "../../../environment.h"

using namespace MyFrameWork;

Model::Model(std::vector<noDel_ptr<Mesh>> vMeshes) {
	//�z��𐶃|�C���^�ɂ��ăR�s�[
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
	//���[�g���b�V���̐��̎擾
	int _rootNum = 0;
	for (auto& mesh : vMeshes) {
		if (mesh->GetParent() == NULL) _rootNum++;
	}
	return _rootNum;
}

noDel_ptr<Mesh> Model::GetRootMesh(int index) {
	//���[�g���b�V���̐��̎擾
	int _rootNum = GetRootCount();
	if (_rootNum < index) index = _rootNum; //�ő吔�𒴂����ꍇ�A�Ō�̃��[�g���b�V���̔ԍ���Ԃ�

	//�w��ԍ��ڂ̃��[�g���b�V����Ԃ�
	int _count = 0;
	for (auto& mesh : vMeshes) {
		if (mesh->GetParent() != NULL) continue;
		if (_count != index) {
			_count++;
			continue;
		}
		//�Ԃ�
		return noDel_ptr<Mesh>(mesh);
	}

	//�����ɍ������̂��Ȃ����NULL��Ԃ�
	return NULL;
}

int Model::GetMeshCount() {
	return (int)vMeshes.size();
}