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

ObjMesh::ObjMesh(eMeshFormat format) : Mesh(format) {
	initPos = { 0,0,0 };
	initRot = { 0,0,0 };
	initScl = { 1,1,1 };
}

ObjMesh::~ObjMesh() {
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