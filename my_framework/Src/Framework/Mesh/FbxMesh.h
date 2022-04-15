/*-----------------------------------------------------------

	FbxMesh�N���X
		���b�V���{��

-------------------------------------------------------------*/
#pragma once

#include <fbxsdk.h>

namespace MyFrameWork {

	class FbxMeshFile : public Mesh {
	private:
		struct stMeshData
		{
			ID3D11Buffer* vertexBuffer;			//!< @brief ���_�o�b�t�@(Shader���M�p)
			ID3D11Buffer* indexBuffer;			//!< @brief �C���f�b�N�X�o�b�t�@(Shader���M�p)
			std::vector<stVertex3D> vertices;
			std::vector<UINT> indices;
			std::string materialName;
			std::string uvSetName;
		};

		std::vector<stMeshData> vMeshList; //���b�V���ꗗ
		std::unordered_map<std::string, ID3D11ShaderResourceView*> umMaterialLinks;

	public:
		FbxMeshFile(const char* fileName);
		~FbxMeshFile();

		void Render(stVector3 pos, stVector3 rot, stVector3 scl);

	private:
		//Fbx���f���쐬
		bool CreateFbx(const char* fileName);
		//Vertex�o�b�t�@�쐬
		bool CreateVertexBuffer();
		//index�o�b�t�@�쐬
		bool CreateIndexBuffer();
		//���b�V���쐬
		bool CreateMesh(FbxMesh* pMesh);
		//index�f�[�^�ǂݍ���
		void LoadIndices(stMeshData& meshData, FbxMesh* pMesh);
		//vertex�f�[�^�ǂݍ���
		void LoadVertices(stMeshData& meshData, FbxMesh* pMesh);
		//���b�V����Transform���Z�o
		void LoadMeshTransform(FbxNode* node, FbxDouble3& pos, FbxDouble3& rot, FbxDouble3& scl);
		//�@���ǂݍ���
		void LoadNormals(stMeshData& meshData, FbxMesh* pMesh);
		//�J���[�ǂݍ���
		void LoadColors(stMeshData& meshData, FbxMesh* pMesh);
		//UV�̓ǂݍ���
		void LoadUV(stMeshData& meshData, FbxMesh* pMesh);
		//�}�e���A���ǂݍ���
		void LoadMaterial(FbxSurfaceMaterial* material);
		//�}�e���A�����Z�b�g
		void SetMaterialName(stMeshData& meshData, FbxMesh* pMesh);
		//�}�e���A���J���[�Z�b�g
		void SetMaterialColor(stMaterial& material);
		//�e�N�X�`���ǂݍ���
		bool LoadTexture(FbxFileTexture* material, std::string& keyword);
	};
}