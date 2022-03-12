/*-----------------------------------------------------------

	Meshクラス
		メッシュ本体

-------------------------------------------------------------*/
#pragma once
namespace MyFrameWork {

	class ObjMesh : public Mesh {
	private:

	public:
		ObjMesh(const char* fileName);
		~ObjMesh();

		void Render(stVector3 pos, stVector3 rot, stVector3 scl);

	private:
		//メッシュ作成
		bool CreateMesh(std::vector<std::string>& outMaterialList, const char* fileName);

		//vertexバッファ作成
		bool CreateVertexBuffer();

		//indexバッファ作成
		bool CreateIndexBuffer();

		//頂点情報の解析
		void ParseVKeywordTag(std::vector<stVector3>& outVertices, char* buff);
		//面情報の解析
		void ParseFKeywordTag(std::vector<stVertex3D>& outVertices, std::unordered_map<std::string, int>& indexList,
			std::string current_material_name, std::vector<stVector3>& vertices, std::vector<stVector3>& textures,
			std::vector<stVector3>& normals, char* buffer);
		//"/"で文字列を分解
		void ParseSlashKeywordTag(int* list, char* buffer);

		//色設定
		void LoadColor();

		//マテリアル読み込み
		bool LoadMaterialFile(std::vector<std::string> fileList, std::string filePath);
		//テクスチャ読み込み
		bool LoadTexture(std::string key_word, std::string fileName);

		void SetMaterial(stMaterial material);
		void SetTexture(ID3D11ShaderResourceView* texture);
	};
}