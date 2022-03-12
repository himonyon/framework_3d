#include "../../../environment.h"

using namespace MyFrameWork;

Mesh::Mesh() {
}

Mesh::~Mesh() {
	if(pVertexBuffer != nullptr) pVertexBuffer->Release();

	for (auto& index : vIndexBuffers) {
		if (index != nullptr) index->Release();
	}


	for (auto& tex : umTextures) {
		if (tex.second != nullptr) tex.second->Release();;
	}
}
