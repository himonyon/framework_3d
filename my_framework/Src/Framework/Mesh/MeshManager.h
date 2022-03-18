/*-----------------------------------------------------------

	MeshManager�N���X
		�V�[���쐬���ꂽ���b�V���̕ۊ�
		�|�C���^���L���̊Ǘ�

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