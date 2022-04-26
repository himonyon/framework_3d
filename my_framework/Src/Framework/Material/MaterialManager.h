/*-----------------------------------------------------------
 
	MaterialManagerクラス
		マテリアルの管理

-------------------------------------------------------------*/
#pragma once
namespace MyFrameWork {

	class MaterialManager {
	public:
		MaterialManager() {};
		~MaterialManager();

		//マテリアル追加
		bool AddMaterial(std::string matName, stMaterial* mat);
		//マテリアル取得
		noDel_ptr<stMaterial> GetMaterial(std::string matName);
	private:
		std::unordered_map<std::string, stMaterial*> umMaterials;
	};
}