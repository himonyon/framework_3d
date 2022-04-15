/*-----------------------------------------------------------

	FbxMeshクラス
		メッシュ本体

-------------------------------------------------------------*/
#pragma once

#include <fbxsdk.h>

namespace MyFrameWork {

	class FbxMeshFile : public Mesh {
	private:
		struct stMeshData
		{
			ID3D11Buffer* vertexBuffer;			//!< @brief 頂点バッファ(Shader送信用)
			ID3D11Buffer* indexBuffer;			//!< @brief インデックスバッファ(Shader送信用)
			std::vector<stVertex3D> vertices;
			std::vector<UINT> indices;
			std::string materialName;
			std::string uvSetName;
		};

		std::vector<stMeshData> vMeshList; //メッシュ一覧
		std::unordered_map<std::string, ID3D11ShaderResourceView*> umMaterialLinks;

	public:
		FbxMeshFile(const char* fileName);
		~FbxMeshFile();

		void Render(stVector3 pos, stVector3 rot, stVector3 scl);

	private:
		//Fbxモデル作成
		bool CreateFbx(const char* fileName);
		//Vertexバッファ作成
		bool CreateVertexBuffer();
		//indexバッファ作成
		bool CreateIndexBuffer();
		//メッシュ作成
		bool CreateMesh(FbxMesh* pMesh);
		//indexデータ読み込み
		void LoadIndices(stMeshData& meshData, FbxMesh* pMesh);
		//vertexデータ読み込み
		void LoadVertices(stMeshData& meshData, FbxMesh* pMesh);
		//メッシュのTransformを算出
		void LoadMeshTransform(FbxNode* node, FbxDouble3& pos, FbxDouble3& rot, FbxDouble3& scl);
		//法線読み込み
		void LoadNormals(stMeshData& meshData, FbxMesh* pMesh);
		//カラー読み込み
		void LoadColors(stMeshData& meshData, FbxMesh* pMesh);
		//UVの読み込み
		void LoadUV(stMeshData& meshData, FbxMesh* pMesh);
		//マテリアル読み込み
		void LoadMaterial(FbxSurfaceMaterial* material);
		//マテリアル名セット
		void SetMaterialName(stMeshData& meshData, FbxMesh* pMesh);
		//マテリアルカラーセット
		void SetMaterialColor(stMaterial& material);
		//テクスチャ読み込み
		bool LoadTexture(FbxFileTexture* material, std::string& keyword);
	};
}