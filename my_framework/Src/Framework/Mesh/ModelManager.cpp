#include "../../../environment.h"

using namespace MyFrameWork;

ModelManager::ModelManager() {
	matManager = new MaterialManager;
}

ModelManager::~ModelManager() {
	//モデル削除
	for (auto& model : umModels) {
		delete model.second;
	}
	umModels.clear();
	//マテリアルマネージャーdelete
	if (matManager != NULL) {
		delete matManager;
		matManager = NULL;
	}
}

noDel_ptr<Model> ModelManager::CreateModel(const char* fileName) {
	//モデル作成
	ModelCreator* creator = new ModelCreator;
	noDel_ptr<Model> _model;
	std::string _fileName = fileName;
	//すでに同じモデルを作成していればそれを使う
	if (umModels.count(_fileName) != 0) {
		_model = noDel_ptr<Model>(umModels[_fileName]);
	}
	//未作成のモデルは新たに生成
	if (_model == NULL) {
		_model = creator->CreateModel(fileName, noDel_ptr<MaterialManager>(matManager));
	}
	delete creator;
		
	if(_model != NULL) umModels[_fileName] = _model.get();
	return _model;
}

