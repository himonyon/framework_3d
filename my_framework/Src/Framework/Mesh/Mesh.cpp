#include "../../../environment.h"

using namespace MyFrameWork;

Mesh::Mesh(eMeshFormat format) {
	meshData = new stMeshData;
	this->format = format;
}
Mesh::~Mesh() {
	if (meshData != NULL) delete meshData;
}

void Mesh::SetParent(noDel_ptr<Mesh> parent) {
	pParent = parent;
	parent->vChildren.emplace_back(noDel_ptr<Mesh>(this));
}
noDel_ptr<Mesh> Mesh::GetParent() {
	return pParent;
}
noDel_ptr<Mesh> Mesh::GetChild(int index) {
	if (index > (int)vChildren.size()) return NULL;
	return vChildren[index];
}
int Mesh::GetChildCount() {
	return (int)vChildren.size();
}
