/*-----------------------------------------------------------

	MeshManager�N���X
		�V�[���쐬���ꂽ���b�V���̕ۊ�
		�|�C���^���L���̊Ǘ�

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