/*-----------------------------------------------------------

	Mesh�N���X
		���b�V���{��

-------------------------------------------------------------*/
#pragma once
namespace MyFrameWork {

	class ObjMesh : public Mesh {
	public:
		ObjMesh(eMeshFormat format);
		~ObjMesh();

	private:
		//"/"�ŕ�����𕪉�
		void ParseSlashKeywordTag(int* list, char* buffer);

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