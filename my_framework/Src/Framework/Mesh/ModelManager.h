/*-----------------------------------------------------------

	MeshManagerクラス
		シーン作成されたメッシュの保管
		ポインタ所有権の管理

-------------------------------------------------------------*/
#pragma once
namespace MyFrameWork {

	class ModelManager {
	public:
		ModelManager();
		~ModelManager();

		noDel_ptr<Model> CreateModel(const char* fileName);

	private:
		MaterialManager* matManager;
		std::unordered_map<std::string, Model*> umModels;
	};
}