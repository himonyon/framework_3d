#include "../../../environment.h"
#include "../../../DirectXTex/DirectXTex.h"

#include <sstream>
#include <iomanip>

#if _DEBUG
#pragma comment(lib,"DirectXTex/Bin/Debug/DirectXTex.lib")
#else
#pragma comment(lib,"DirectXTex/Bin/Release/DirectXTex.lib")
#endif

using namespace MyFrameWork;

ObjMesh::ObjMesh() {
}

ObjMesh::~ObjMesh() {
}

void ObjMesh::Render(stVector3 pos, stVector3 rot, stVector3 scl) {
	UINT strides = sizeof(stVertex3D);
	UINT offsets = 0;

	// 描画する
	// IA(InputAssemblerStage)に入力レイアウトを設定する
	Direct3D::GetDeviceContext()->IASetInputLayout(Renderer3D::GetInputLayout());
	// IAに設定する頂点バッファの指定
	Direct3D::GetDeviceContext()->IASetVertexBuffers(
		0,						// バッファ送信のスロット番号
		1,						// バッファの数
		&meshData->vertexBuffer,		// 頂点バッファ
		&strides,				// バッファに使用している構造体のサイズ
		&offsets);				// 開始オフセット

	Direct3D::GetDeviceContext()->IASetIndexBuffer(
		meshData->indexBuffer,
		DXGI_FORMAT_R32_UINT,
		0);

	// ワールドマトリクス設定
	XMMATRIX mWorld;
	XMMATRIX translate = XMMatrixTranslation(pos.x, pos.y, pos.z);
	XMMATRIX rotate_x = XMMatrixRotationX(XMConvertToRadians(rot.x));
	XMMATRIX rotate_y = XMMatrixRotationY(XMConvertToRadians(rot.y));
	XMMATRIX rotate_z = XMMatrixRotationZ(XMConvertToRadians(rot.z));
	XMMATRIX scale_mat = ::XMMatrixScaling(scl.x, scl.y, scl.z);
	mWorld = scale_mat * rotate_x * rotate_y * rotate_z * translate;

	stCBuffer3D& _inputCB = Renderer3D::GetInputCB();

	// ワールドマトリクスをコンスタントバッファに設定
	XMStoreFloat4x4(&_inputCB.world, XMMatrixTranspose(mWorld));

	//マテリアル情報セット
	LoadMaterial(*pMaterial.get());

	// テクスチャ設定
	// Samplerの設定
	ID3D11SamplerState* _sampleState = Renderer3D::GetSampleLinear();
	Direct3D::GetDeviceContext()->PSSetSamplers(0, 1, &_sampleState);	// ID3D11SamplerState
	// マテリアルにテクスチャがあるなら設定する
	if (pMaterial->pTexture != NULL)
	{
		Direct3D::GetDeviceContext()->PSSetShader(Shader::getPixelShader(Shader::ePixelShader::PS_3D)->getShader(), nullptr, 0);
		Direct3D::GetDeviceContext()->PSSetShaderResources(0, 1, &pMaterial->pTexture);
	}
	else
	{
		Direct3D::GetDeviceContext()->PSSetShader(Shader::getPixelShader(Shader::ePixelShader::PS_3D_NOTEX)->getShader(), nullptr, 0);
	}

	ID3D11Buffer* _constantBuffer = Renderer3D::GetConstantBuffer();
	// コンスタントバッファ更新
	Direct3D::GetDeviceContext()->UpdateSubresource(_constantBuffer, 0, NULL, &_inputCB, 0, 0);

	// コンテキストにコンスタントバッファを設定
	Direct3D::GetDeviceContext()->VSSetConstantBuffers(0, 1, &_constantBuffer);
	Direct3D::GetDeviceContext()->PSSetConstantBuffers(0, 1, &_constantBuffer);

	// 描画
	Direct3D::GetDeviceContext()->DrawIndexed(
		(UINT)meshData->indices.size(),		// 頂点数
		0,						// オフセット
		0);		
}

bool ObjMesh::CreateVertexBuffer() {
	//頂点バッファ作成
	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.ByteWidth = sizeof(stVertex3D) * (UINT)meshData->vertices.size();	// バッファのサイズ
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;			// 使用方法
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// BIND設定
	buffer_desc.CPUAccessFlags = 0;						// リソースへのCPUのアクセス権限についての設定
	buffer_desc.MiscFlags = 0;							// リソースオプションのフラグ
	buffer_desc.StructureByteStride = 0;				// 構造体のサイズ

	D3D11_SUBRESOURCE_DATA sub_resource;
	sub_resource.pSysMem = &meshData->vertices[0];	// バッファの中身の設定
	sub_resource.SysMemPitch = 0;			// textureデータを使用する際に使用するメンバ
	sub_resource.SysMemSlicePitch = 0;		// textureデータを使用する際に使用するメンバ

	// バッファ作成
	if (FAILED(Direct3D::GetDevice()->CreateBuffer(
		&buffer_desc,				// バッファ情報
		&sub_resource,				// リソース情報
		&meshData->vertexBuffer)))			// 作成されたバッファの格納先
	{
		return false;
	}

	return true;
}
bool ObjMesh::CreateIndexBuffer() {

	//頂点バッファ作成
	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.ByteWidth = sizeof(UINT) * (UINT)meshData->indices.size();	// バッファのサイズ
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;							// 使用方法
	buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;					// BIND設定
	buffer_desc.CPUAccessFlags = 0;										// リソースへのCPUのアクセス権限についての設定
	buffer_desc.MiscFlags = 0;											// リソースオプションのフラグ
	buffer_desc.StructureByteStride = 0;								// 構造体のサイズ

	D3D11_SUBRESOURCE_DATA sub_resource;
	sub_resource.pSysMem = &meshData->indices[0];							// バッファの中身の設定
	sub_resource.SysMemPitch = 0;										// textureデータを使用する際に使用するメンバ
	sub_resource.SysMemSlicePitch = 0;									// textureデータを使用する際に使用するメンバ

	// バッファ作成
	if (FAILED(Direct3D::GetDevice()->CreateBuffer(
		&buffer_desc,					// バッファ情報
		&sub_resource,					// リソース情報
		&meshData->indexBuffer)))		// 作成されたバッファの格納先
	{
		return false;
	}

	return true;
}

void ObjMesh::ParseFKeywordTag(std::unordered_map<std::string, int>& index_list,
	std::string curMaterialName, std::vector<stVector3>& vertices, std::vector<stVector3>& textures,
	std::vector<stVector3>& normals, char* buffer) {
	int count = 0;
	int vertex_info[3] =
	{
		-1, -1, -1,
	};
	std::vector<std::string> space_split;

	Split(' ', buffer, space_split);

	int add_index_count = 0;
	for (int i = 0; i < space_split.size(); i++)
	{
		stVertex3D vertex;
		ParseSlashKeywordTag(vertex_info, (char*)space_split[i].c_str());

		for (int i = 0; i < 4; i++)
		{
			if (vertex_info[i] == -1)
			{
				continue;
			}

			int id = vertex_info[i];

			switch (i)
			{
			case 0:
				vertex.pos = vertices[id];
				break;
			case 1:
				vertex.tex = stVector2{ textures[id].x, textures[id].y };
				break;
			case 2:
				vertex.nor = normals[id];
				break;
			case 3:
				vertex.col = {1,1,1,1};
				break;
			}

		}

		std::string key = "";

		// 調査頂点情報作成
		for (int i = 0; i < 3; i++)
		{
			std::ostringstream sout;
			sout << std::setfill('0') << std::setw(5) << vertex_info[i];
			key += sout.str();
		}

		// 重複チェック
		if (index_list.count(key) > 0)
		{
			// 登録されている頂点バッファの要素番号をインデックスバッファに保存する
			meshData->indices.push_back(index_list[key]);
		}
		else
		{
			// 頂点情報追加
			meshData->vertices.push_back(vertex);

			// 頂点バッファの要素番号追加
			meshData->indices.push_back((UINT)meshData->vertices.size() - 1);

			// 頂点バッファの要素番号追加
			index_list[key] = (int)meshData->vertices.size() - 1;
		}

		add_index_count++;
	}

	UINT size = (UINT)meshData->indices.size();
	UINT temp = 0;

	// 「f」キーワードの分解結果が3を超えた場合は頂点を追加する
	if (space_split.size() > 3)
	{
		meshData->indices.push_back(meshData->indices[size - 4]);
		meshData->indices.push_back(meshData->indices[size - 2]);
		add_index_count += 2;
	}
}
void ObjMesh::ParseSlashKeywordTag(int* list, char* buffer) {
	int _counter = 0;
	std::vector<std::string> slash_split;
	Split('/', buffer, slash_split);

	for (std::string str : slash_split)
	{
		if (str.size() > 0)
		{
			list[_counter] = atoi(str.c_str()) - 1;
		}
		_counter++;
	}
}



void ObjMesh::LoadMaterial(stMaterial& material) {
	stCBuffer3D& cb = Renderer3D::GetInputCB();
	cb.ambient = XMFLOAT4(material.ambient.r, material.ambient.g, material.ambient.b, 1);
	cb.diffuse = XMFLOAT4(material.diffuse.r, material.diffuse.g, material.diffuse.b, 1);
	cb.specular = XMFLOAT4(material.specular.r, material.specular.g, material.specular.b, 1);
}
void ObjMesh::LoadTexture(ID3D11ShaderResourceView* texture) {
	// Samplerの設定
	ID3D11SamplerState* _sampleState = Renderer3D::GetSampleLinear();
	Direct3D::GetDeviceContext()->PSSetSamplers(0,	1, &_sampleState);

	// PixelShaderで使用するテクスチャの設定
	Direct3D::GetDeviceContext()->PSSetShaderResources(0,1,	&texture);
}