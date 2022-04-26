/*-----------------------------------------------------------
 
	MaterialManager�N���X
		�}�e���A���̊Ǘ�

-------------------------------------------------------------*/
#pragma once
namespace MyFrameWork {

	class MaterialManager {
	public:
		MaterialManager() {};
		~MaterialManager();

		//�}�e���A���ǉ�
		bool AddMaterial(std::string matName, stMaterial* mat);
		//�}�e���A���擾
		noDel_ptr<stMaterial> GetMaterial(std::string matName);
	private:
		std::unordered_map<std::string, stMaterial*> umMaterials;
	};
}