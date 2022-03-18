/*-----------------------------------------------------------

	MeshManagerクラス
		シーン作成されたメッシュの保管
		ポインタ所有権の管理

-------------------------------------------------------------*/
#pragma once
namespace MyFrameWork {

	class MeshManager {
	public:
		MeshManager() {};
		~MeshManager() {
			for (auto& mesh : meshes) {
				delete mesh;
			}
			meshes.clear();
		}

		noDel_ptr<Mesh> CreateMesh(const char* fileName);

	private:
		std::vector<Mesh*> meshes;
	};
}