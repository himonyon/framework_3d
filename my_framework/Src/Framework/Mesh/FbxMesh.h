/*-----------------------------------------------------------

	FbxMesh�N���X
		���b�V���{��

-------------------------------------------------------------*/
#pragma once

#include <fbxsdk.h>

namespace MyFrameWork {

	class FbxMeshFile : public Mesh {
	public:
		FbxMeshFile(FbxMesh* mesh);
		~FbxMeshFile();

		void Render(stVector3 pos, stVector3 rot, stVector3 scl);

	private:
		//Fbx���f���쐬
		bool CreateFbx(FbxMesh* mesh);
		//Vertex�o�b�t�@�쐬
		bool CreateVertexBuffer();
		//index�o�b�t�@�쐬
		bool CreateIndexBuffer();
		//���b�V���쐬
		bool CreateMesh(FbxMesh* pMesh);
		//index�f�[�^�ǂݍ���
		void LoadIndices(stMeshData& meshData, FbxMesh* pMesh);
		//vertex�f�[�^�ǂݍ���
		void LoadVertices(stMeshData& meshData, FbxMesh* pMesh, FbxAMatrix& mat);
		//���b�V����Transform���Z�o
		FbxAMatrix LoadMeshTransform(FbxNode* node);
		//�@���ǂݍ���
		void LoadNormals(stMeshData& meshData, FbxMesh* pMesh, FbxAMatrix& mat);
		//�J���[�ǂݍ���
		void LoadColors(stMeshData& meshData, FbxMesh* pMesh);
		//UV�̓ǂݍ���
		void LoadUV(stMeshData& meshData, FbxMesh* pMesh);

		//�}�e���A���J���[�Z�b�g
		void SetMaterialColor(stMaterial& material);
	};
}