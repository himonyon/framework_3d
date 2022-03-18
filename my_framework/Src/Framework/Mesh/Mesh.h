/*-----------------------------------------------------------

	Meshクラス
		メッシュ本体

-------------------------------------------------------------*/
#pragma once

namespace MyFrameWork {
	class Mesh {
	protected:
		ID3D11Buffer* pVertexBuffer;					//!< @brief 頂点バッファ(Shader送信用)
		std::vector<ID3D11Buffer*> vIndexBuffers;		//インデックスバッファ(Shader送信用)
		std::unordered_map<std::string, stMaterial> umMaterials;
		std::vector<stVertex3D> vVertices;	//頂点バッファ
		std::unordered_map<std::string, std::vector<UINT>> umIndices; //インデックスバッファ
		std::unordered_map<std::string, ID3D11ShaderResourceView*> umTextures;

	public:
		Mesh();
		virtual ~Mesh();

		virtual void Render(stVector3 pos, stVector3 rot, stVector3 scl) = 0;
	};
}