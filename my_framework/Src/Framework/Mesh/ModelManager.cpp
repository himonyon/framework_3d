#include "../../../environment.h"

using namespace MyFrameWork;

ModelManager::ModelManager() {
	matManager = new MaterialManager;
}

ModelManager::~ModelManager() {
	//���f���폜
	for (auto& model : umModels) {
		delete model.second;
	}
	umModels.clear();
	//�}�e���A���}�l�[�W���[delete
	if (matManager != NULL) {
		delete matManager;
		matManager = NULL;
	}
}

noDel_ptr<Model> ModelManager::CreateModel(const char* fileName) {
	//���f���쐬
	ModelCreator* creator = new ModelCreator;
	noDel_ptr<Model> _model;
	std::string _fileName = fileName;
	//���łɓ������f�����쐬���Ă���΂�����g��
	if (umModels.count(_fileName) != 0) {
		_model = noDel_ptr<Model>(umModels[_fileName]);
	}
	//���쐬�̃��f���͐V���ɐ���
	if (_model == NULL) {
		_model = creator->CreateModel(fileName, noDel_ptr<MaterialManager>(matManager));
	}
	delete creator;
		
	if(_model != NULL) umModels[_fileName] = _model.get();
	return _model;
}

