/*-----------------------------------------------------------

	FbxMeshクラス
		メッシュ本体

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
		//Fbxモデル作成
		bool CreateFbx(FbxMesh* mesh);
		//Vertexバッファ作成
		bool CreateVertexBuffer();
		//indexバッファ作成
		bool CreateIndexBuffer();
		//メッシュ作成
		bool CreateMesh(FbxMesh* pMesh);
		//indexデータ読み込み
		void LoadIndices(stMeshData& meshData, FbxMesh* pMesh);
		//vertexデータ読み込み
		void LoadVertices(stMeshData& meshData, FbxMesh* pMesh, FbxAMatrix& mat);
		//メッシュのTransformを算出
		FbxAMatrix LoadMeshTransform(FbxNode* node);
		//法線読み込み
		void LoadNormals(stMeshData& meshData, FbxMesh* pMesh, FbxAMatrix& mat);
		//カラー読み込み
		void LoadColors(stMeshData& meshData, FbxMesh* pMesh);
		//UVの読み込み
		void LoadUV(stMeshData& meshData, FbxMesh* pMesh);

		//マテリアルカラーセット
		void SetMaterialColor(stMaterial& material);
	};
}