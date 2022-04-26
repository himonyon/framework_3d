/*-----------------------------------------------------------

	MeshCreatorクラス
		メッシュの生成

-------------------------------------------------------------*/
#pragma once
namespace MyFrameWork {
	class ModelCreator {
	private:
		noDel_ptr<MaterialManager> matManager;
		std::vector<noDel_ptr<Mesh>> vMeshList; //メッシュ一覧
		std::unordered_map<std::string, ID3D11ShaderResourceView*> umTextures;

	public:
		noDel_ptr<Model> CreateModel(const char* fileName, noDel_ptr<MaterialManager> matManager);

	private:
		//FBXモデル作成------------------------------------------------
		bool CreateFbxMeshes(const char* fileName);
		//マテリアル読み込み
		void LoadMaterial(FbxSurfaceMaterial* material);
		bool LoadTexture(FbxFileTexture* texture, std::string& keyword);
		//マテリアルセット
		void SetMaterial(noDel_ptr<Mesh> pMeshFile, FbxMesh* pFbxMesh);
		//メッシュの親子関係を設定
		void ParentSetting(std::unordered_map<FbxMesh*, noDel_ptr<Mesh>>&);

		//Objモデル作成
		bool CreateObjMeshes(const char* fileName);
		bool LoadObjMeshes(std::unordered_map<std::string, ObjMesh*>& objs, std::vector<std::string>& outMaterialList, const char* fileName);
		//頂点情報の解析
		void ParseVKeywordTag(std::vector<stVector3>& outVertices, char* buff);
		//マテリアル読み込み
		bool LoadMaterialFile(std::vector<std::string> fileList, std::string filePath);
		//テクスチャ読み込み
		bool LoadTexture(std::string key_word, std::string fileName);
	};
}