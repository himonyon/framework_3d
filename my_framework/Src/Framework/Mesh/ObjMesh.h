/*-----------------------------------------------------------

	Mesh�N���X
		���b�V���{��

-------------------------------------------------------------*/
#pragma once
namespace MyFrameWork {

	class ObjMesh : public Mesh {
	public:
		ObjMesh();
		~ObjMesh();

		void Render(stVector3 pos, stVector3 rot, stVector3 scl);

	private:
		//"/"�ŕ�����𕪉�
		void ParseSlashKeywordTag(int* list, char* buffer);

		void LoadMaterial(stMaterial& material);
		void LoadTexture(ID3D11ShaderResourceView* texture);

	public:
		//�ʏ��̉��
		void ParseFKeywordTag(std::unordered_map<std::string, int>& indexList,
			std::string current_material_name, std::vector<stVector3>& vertices, std::vector<stVector3>& textures,
			std::vector<stVector3>& normals, char* buffer);

		//vertex�o�b�t�@�쐬
		bool CreateVertexBuffer();

		//index�o�b�t�@�쐬
		bool CreateIndexBuffer();
	};
}