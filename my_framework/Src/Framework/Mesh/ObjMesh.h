/*-----------------------------------------------------------

	Mesh�N���X
		���b�V���{��

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
		//���b�V���쐬
		bool CreateMesh(std::vector<std::string>& outMaterialList, const char* fileName);

		//vertex�o�b�t�@�쐬
		bool CreateVertexBuffer();

		//index�o�b�t�@�쐬
		bool CreateIndexBuffer();

		//���_���̉��
		void ParseVKeywordTag(std::vector<stVector3>& outVertices, char* buff);
		//�ʏ��̉��
		void ParseFKeywordTag(std::vector<stVertex3D>& outVertices, std::unordered_map<std::string, int>& indexList,
			std::string current_material_name, std::vector<stVector3>& vertices, std::vector<stVector3>& textures,
			std::vector<stVector3>& normals, char* buffer);
		//"/"�ŕ�����𕪉�
		void ParseSlashKeywordTag(int* list, char* buffer);

		//�F�ݒ�
		void LoadColor();

		//�}�e���A���ǂݍ���
		bool LoadMaterialFile(std::vector<std::string> fileList, std::string filePath);
		//�e�N�X�`���ǂݍ���
		bool LoadTexture(std::string key_word, std::string fileName);

		void SetMaterial(stMaterial material);
		void SetTexture(ID3D11ShaderResourceView* texture);
	};
}