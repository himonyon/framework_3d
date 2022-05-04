/*-----------------------------------------------------------

	FbxMeshクラス
		メッシュ本体

-------------------------------------------------------------*/
#pragma once

#include <fbxsdk.h>

namespace MyFrameWork {

	class FbxMeshFile : public Mesh {
	private:
		noDel_ptr<FbxMesh> fbxMeshData;

	public:
		FbxMeshFile(FbxMesh* mesh, eMeshFormat format);
		~FbxMeshFile();

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

	public:
		noDel_ptr<FbxMesh> GetFbxMesh() { return fbxMeshData; }
	};
}