/*-----------------------------------------------------------

	MeshCreator�N���X
		���b�V���̐���

-------------------------------------------------------------*/
#pragma once
namespace MyFrameWork {
	class ModelCreator {
	private:
		noDel_ptr<MaterialManager> matManager;
		std::vector<noDel_ptr<Mesh>> vMeshList; //���b�V���ꗗ
		std::unordered_map<std::string, ID3D11ShaderResourceView*> umTextures;

	public:
		noDel_ptr<Model> CreateModel(const char* fileName, noDel_ptr<MaterialManager> matManager);

	private:
		//FBX���f���쐬------------------------------------------------
		bool CreateFbxMeshes(const char* fileName);
		//�}�e���A���ǂݍ���
		void LoadMaterial(FbxSurfaceMaterial* material);
		bool LoadTexture(FbxFileTexture* texture, std::string& keyword);
		//�}�e���A���Z�b�g
		void SetMaterial(noDel_ptr<Mesh> pMeshFile, FbxMesh* pFbxMesh);
		//���b�V���̐e�q�֌W��ݒ�
		void ParentSetting(std::unordered_map<FbxMesh*, noDel_ptr<Mesh>>&);

		//Obj���f���쐬
		bool CreateObjMeshes(const char* fileName);
		bool LoadObjMeshes(std::unordered_map<std::string, ObjMesh*>& objs, std::vector<std::string>& outMaterialList, const char* fileName);
		//���_���̉��
		void ParseVKeywordTag(std::vector<stVector3>& outVertices, char* buff);
		//�}�e���A���ǂݍ���
		bool LoadMaterialFile(std::vector<std::string> fileList, std::string filePath);
		//�e�N�X�`���ǂݍ���
		bool LoadTexture(std::string key_word, std::string fileName);
	};
}