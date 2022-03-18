#include "../../../environment.h"

using namespace MyFrameWork;

noDel_ptr<Mesh> MeshManager::CreateMesh(const char* fileName) {
	//Šg’£ŽqŽæ“¾
	const char* _ext = strchr(fileName, '.');
	Mesh* instance;
	if (_ext) {
		if (strcmp(_ext, ".obj") == 0) instance = new ObjMesh(fileName);
		else if (strcmp(_ext, ".fbx") == 0)instance = new FbxMeshFile(fileName);
		else return NULL;
	}
	else {
		return NULL;
	}
	meshes.emplace_back(instance);
	noDel_ptr<Mesh> p = noDel_ptr<Mesh>(meshes.back());
	return p;
}
