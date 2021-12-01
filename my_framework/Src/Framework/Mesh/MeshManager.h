#pragma once

class MeshManager {
public:
	MeshManager() {};
	~MeshManager() {
		for (auto& mesh : meshes) {
			delete mesh;
		}
		meshes.clear();
	}

	noDel_ptr<Mesh> CreateMesh(Mesh* instance) {
		meshes.emplace_back(instance);
		noDel_ptr<Mesh> p = noDel_ptr<Mesh>(meshes.back());
		return p;
	}

	noDel_ptr<Mesh> CreateMesh(const WCHAR* fileName) {
		Mesh* instance = new Mesh(fileName);
		meshes.emplace_back(instance);
		noDel_ptr<Mesh> p = noDel_ptr<Mesh>(meshes.back());
		return p;
	}

private:
	std::vector<Mesh*> meshes;
};