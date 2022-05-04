/*-----------------------------------------------------------

	Meshクラス
		メッシュ本体

-------------------------------------------------------------*/
#pragma once
namespace MyFrameWork {

	class ObjMesh : public Mesh {
	public:
		ObjMesh(eMeshFormat format);
		~ObjMesh();

	private:
		//"/"で文字列を分解
		void ParseSlashKeywordTag(int* list, char* buffer);

	public:
		//面情報の解析
		void ParseFKeywordTag(std::unordered_map<std::string, int>& indexList,
			std::string current_material_name, std::vector<stVector3>& vertices, std::vector<stVector3>& textures,
			std::vector<stVector3>& normals, char* buffer);

		//vertexバッファ作成
		bool CreateVertexBuffer();

		//indexバッファ作成
		bool CreateIndexBuffer();
	};
}