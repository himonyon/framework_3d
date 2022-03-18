/*-----------------------------------------------------------

	Mesh�N���X
		���b�V���{��

-------------------------------------------------------------*/
#pragma once

namespace MyFrameWork {
	class Mesh {
	protected:
		ID3D11Buffer* pVertexBuffer;					//!< @brief ���_�o�b�t�@(Shader���M�p)
		std::vector<ID3D11Buffer*> vIndexBuffers;		//�C���f�b�N�X�o�b�t�@(Shader���M�p)
		std::unordered_map<std::string, stMaterial> umMaterials;
		std::vector<stVertex3D> vVertices;	//���_�o�b�t�@
		std::unordered_map<std::string, std::vector<UINT>> umIndices; //�C���f�b�N�X�o�b�t�@
		std::unordered_map<std::string, ID3D11ShaderResourceView*> umTextures;

	public:
		Mesh();
		virtual ~Mesh();

		virtual void Render(stVector3 pos, stVector3 rot, stVector3 scl) = 0;
	};
}