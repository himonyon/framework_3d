/*-----------------------------------------------------------

	Modelクラス
		モデル(生成されたメッシュの塊)

-------------------------------------------------------------*/
#pragma once
namespace MyFrameWork {

	class Model {
	private:
		std::vector<Mesh*> vMeshes;

	public:
		Model(std::vector<noDel_ptr<Mesh>> vMeshes);
		Model() {};
		~Model();

		int GetMeshCount();
		int GetRootCount();
		noDel_ptr<Mesh> GetRootMesh(int index);
	};
}