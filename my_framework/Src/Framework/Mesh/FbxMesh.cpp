#include "../../../environment.h"
#include "../../../DirectXTex/DirectXTex.h"

#if _DEBUG
#pragma comment(lib,"DirectXTex/Bin/Debug/DirectXTex.lib")
#else
#pragma comment(lib,"DirectXTex/Bin/Release/DirectXTex.lib")
#endif

using namespace MyFrameWork;

FbxMeshFile::FbxMeshFile(FbxMesh* mesh) {
	if (CreateFbx(mesh) == false)
	{
		return;
	}

	if (CreateVertexBuffer() == false)
	{
		return;
	}

	if (CreateIndexBuffer() == false)
	{
		return;
	}
}

FbxMeshFile::~FbxMeshFile() {
}

void FbxMeshFile::Render(stVector3 pos, stVector3 rot, stVector3 scl) {
	UINT strides = sizeof(stVertex3D);
	UINT offsets = 0;

	// インデックスバッファの数 = マテリアルの数だけメッシュを描画する
		// IA(InputAssemblerStage)に入力レイアウトを設定する
	Direct3D::GetDeviceContext()->IASetInputLayout(Renderer3D::GetInputLayout());
	// IAに設定する頂点バッファの指定
	Direct3D::GetDeviceContext()->IASetVertexBuffers(
		0,								// バッファ送信のスロット番号
		1,								// バッファの数
		&meshData->vertexBuffer,	// 頂点バッファ
		&strides,						// バッファに使用している構造体のサイズ
		&offsets);						// 開始オフセット

	Direct3D::GetDeviceContext()->IASetIndexBuffer(
		meshData->indexBuffer,
		DXGI_FORMAT_R32_UINT,
		0);

	// ワールドマトリクス設定
	XMMATRIX world_matrix;
	XMMATRIX translate = XMMatrixTranslation(pos.x, pos.y, pos.z);
	XMMATRIX rotate_x = XMMatrixRotationX(XMConvertToRadians(rot.x));
	XMMATRIX rotate_y = XMMatrixRotationY(XMConvertToRadians(rot.y));
	XMMATRIX rotate_z = XMMatrixRotationZ(XMConvertToRadians(rot.z));
	XMMATRIX scale_mat = XMMatrixScaling(scl.x, scl.y, scl.z);
	world_matrix = scale_mat * rotate_x * rotate_y * rotate_z * translate;

	// ワールドマトリクスをコンスタントバッファに設定
	XMStoreFloat4x4(&Renderer3D::GetInputCB().world, XMMatrixTranspose(world_matrix));

	///色設定
	SetMaterialColor(*pMaterial.get());

	// テクスチャ設定
	// Samplerの設定
	ID3D11SamplerState* _sampleState = Renderer3D::GetSampleLinear();
	Direct3D::GetDeviceContext()->PSSetSamplers(0, 1, &_sampleState);	// ID3D11SamplerState

	if (pMaterial->pTexture != NULL)
	{
		Direct3D::GetDeviceContext()->PSSetShader(Shader::getPixelShader(Shader::ePixelShader::PS_3D)->getShader(), nullptr, 0);
		Direct3D::GetDeviceContext()->PSSetShaderResources(0,1,&pMaterial->pTexture);
	}
	else
	{
		Direct3D::GetDeviceContext()->PSSetShader(Shader::getPixelShader(Shader::ePixelShader::PS_3D_NOTEX)->getShader(), nullptr, 0);
	}

	// コンスタントバッファ更新
	Direct3D::GetDeviceContext()->UpdateSubresource(Renderer3D::GetConstantBuffer(), 0, NULL, &Renderer3D::GetInputCB(), 0, 0);

	ID3D11Buffer* constant_buffer = Renderer3D::GetConstantBuffer();
	// コンテキストにコンスタントバッファを設定
	Direct3D::GetDeviceContext()->VSSetConstantBuffers(0, 1, &constant_buffer);
	Direct3D::GetDeviceContext()->PSSetConstantBuffers(0, 1, &constant_buffer);

	// 描画
	Direct3D::GetDeviceContext()->DrawIndexed(
		(UINT)meshData->indices.size(),	// 頂点数
		0,								// オフセット
		0);								// 開始頂点のインデックス
}

bool FbxMeshFile::CreateFbx(FbxMesh* mesh) {
	// Mesh作成
	CreateMesh(mesh);
	
	return true;
}
bool FbxMeshFile::CreateVertexBuffer() {
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
		&buffer_desc,								// バッファ情報
		&sub_resource,								// リソース情報
		&meshData->vertexBuffer)))	// 作成されたバッファの格納先
	{
		return false;
	}

	return true;
}
bool FbxMeshFile::CreateIndexBuffer() {
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
		&buffer_desc,						// バッファ情報
		&sub_resource,						// リソース情報
		&meshData->indexBuffer)))		// 作成されたバッファの格納先
	{
		return false;
	}

	return true;
}

bool FbxMeshFile::CreateMesh(FbxMesh* pMesh) {
	
	LoadIndices(*meshData, pMesh);
	LoadVertices(*meshData, pMesh);
	LoadNormals(*meshData, pMesh);
	LoadUV(*meshData, pMesh);
	LoadColors(*meshData, pMesh);

	return true;
}
void FbxMeshFile::LoadIndices(stMeshData& meshData, FbxMesh* pMesh) {
	// ポリゴン数の取得
	int polygon_count = pMesh->GetPolygonCount();

	// ポリゴンの数だけ連番として保存する
	for (int i = 0; i < polygon_count; i++)
	{
		meshData.indices.emplace_back(i * 3 + 2);
		meshData.indices.emplace_back(i * 3 + 1);
		meshData.indices.emplace_back(i * 3);
	}
}
void FbxMeshFile::LoadVertices(stMeshData& meshData, FbxMesh* pMesh) {
	// 頂点バッファの取得
	FbxVector4* vertices = pMesh->GetControlPoints();
	// インデックスバッファの取得
	int* indices = pMesh->GetPolygonVertices();
	// 頂点座標の数の取得
	int polygonVertexCount = pMesh->GetPolygonVertexCount();

	//中心情報(姿勢)取得
	FbxNode* node = pMesh->GetNode();
	FbxDouble3 _meshPos, _meshRot, _meshScl; //メッシュ座標
	LoadMeshTransform(node, _meshPos, _meshRot, _meshScl);

	//回転行列の作成
	XMMATRIX _rotXM = XMMatrixRotationX(float(_meshRot[0]));
	XMMATRIX _rotYM = XMMatrixRotationY(float(_meshRot[1]));
	XMMATRIX _rotZM = XMMatrixRotationZ(float(_meshRot[2]));
	XMMATRIX _rotM = _rotXM * _rotYM * _rotXM;
	XMMATRIX _newPos;
	//---------------------------------------------------------

	//各頂点の座標の設定a
	for (int i = 0; i < polygonVertexCount; i++)
	{
		stVertex3D vertex;
		// インデックスバッファから頂点番号を取得
		int index = indices[i];

		// 頂点座標リストから座標を取得 & モデルの姿勢を反映させる
		vertex.pos.x = (float)-vertices[index][0] * float(_meshScl[0]) + float(_meshPos[0]);
		vertex.pos.y = (float)vertices[index][1] * float(_meshScl[1]) + float(_meshPos[1]);
		vertex.pos.z = (float)vertices[index][2] * float(_meshScl[2]) + float(_meshPos[2]);

		//回転を含んだ新たな座標を取得
		XMMATRIX _translate = XMMatrixTranslation(vertex.pos.x, vertex.pos.y, vertex.pos.z);
		_newPos = _translate * _rotM;
		XMFLOAT4X4 _temp;
		XMStoreFloat4x4(&_temp, XMMatrixTranspose(_newPos));
		vertex.pos.x = _temp._14;
		vertex.pos.y = _temp._24;
		vertex.pos.z = _temp._34;

		// 追加
		meshData.vertices.emplace_back(vertex);
	}
}
void FbxMeshFile::LoadNormals(stMeshData& meshData, FbxMesh* pMesh) {
	FbxArray<FbxVector4> normals;
	// 法線リストの取得
	pMesh->GetPolygonVertexNormals(normals);
	//中心情報(姿勢)取得
	FbxNode* node = pMesh->GetNode();
	FbxDouble3 _meshPos, _meshRot, _meshScl; //メッシュ座標
	LoadMeshTransform(node, _meshPos, _meshRot, _meshScl);

	//回転行列の作成
	XMMATRIX _rotXM = XMMatrixRotationX(float(_meshRot.mData[0]));
	XMMATRIX _rotYM = XMMatrixRotationY(float(_meshRot.mData[1]));
	XMMATRIX _rotZM = XMMatrixRotationZ(float(_meshRot.mData[2]));
	XMMATRIX _rotM = _rotXM * _rotYM * _rotXM;
	XMMATRIX _newNor;

	// 法線設定
	for (int i = 0; i < normals.Size(); i++)
	{
		meshData.vertices[i].nor.x = (float)-normals[i][0] * float(_meshScl[0]) + float(_meshPos[0]);
		meshData.vertices[i].nor.y = (float)normals[i][1] * float(_meshScl[1]) + float(_meshPos[1]);
		meshData.vertices[i].nor.z = (float)normals[i][2] * float(_meshScl[2]) + float(_meshPos[2]);

		//回転を含んだ新たな法線を取得
		XMMATRIX _normal = XMMatrixTranslation(meshData.vertices[i].nor.x,
			meshData.vertices[i].nor.y, meshData.vertices[i].nor.z);
		_newNor = _normal * _rotM;
		XMFLOAT4X4 _temp;
		XMStoreFloat4x4(&_temp, XMMatrixTranspose(_newNor));
		meshData.vertices[i].nor.x = _temp._14;
		meshData.vertices[i].nor.y = _temp._24;
		meshData.vertices[i].nor.z = _temp._34;
	}
}
void FbxMeshFile::LoadMeshTransform(FbxNode* node, FbxDouble3& pos, FbxDouble3& rot, FbxDouble3& scl) {
	FbxNode* parent = node->GetParent();
	FbxDouble3 _parentPos, _parentRot, _parentScl; //親座標

	// 各座標取得
	pos = node->LclTranslation.Get(); //座標
	rot = node->LclRotation.Get(); //回転
	scl = node->LclScaling.Get(); //スケール
	while (parent != NULL) {
		_parentPos = parent->LclTranslation.Get();
		_parentRot = parent->LclRotation.Get();
		_parentScl = parent->LclScaling.Get();

		//親座標と結合
		for (int i = 0; i < 3; i++) {
			pos[i] += _parentPos[i];
			rot[i] += _parentRot[i];
			scl[i] *= _parentScl[i];
		}
		//親がいなくなるまで探索
		parent = parent->GetParent();
	}

	//親再設定
	parent = node->GetParent();

	//cmからｍへ長さを変換
	for (auto& scl : scl.mData) scl /= 100;
	//親がメッシュを持っている場合座標を長さ変換
	if (parent->GetMesh() == NULL) for (auto& pos : pos.mData) pos /= 100;

	//差分を計算（スケーリング後の親との結合のため）
	FbxDouble3 _geometry = node->GeometricTranslation.Get();
	for (auto& pos : _geometry.mData) pos /= 100; //単位変換
	pos = { pos[0] - _geometry[0],pos[1] - _geometry[1], pos[2] - _geometry[2] };
	FbxDouble3 _defOnScl = { pos[0] * (1 - scl[0]), pos[1] * (1 - scl[1]), pos[2] * (1 - scl[2]) };
	pos = { pos[0] - _defOnScl[0],pos[1] - _defOnScl[1], pos[2] - _defOnScl[2] };
	pos = { _geometry[0] + pos[0],_geometry[1] + pos[1], _geometry[2] + pos[2] };
}
void FbxMeshFile::LoadColors(stMeshData& meshData, FbxMesh* pMesh) {
	// 頂点カラーデータの数を確認
	int color_count = pMesh->GetElementVertexColorCount();
	if (color_count == 0)
	{
		for (auto& ver : meshData.vertices) {
			ver.col.r = 1;
			ver.col.g = 1;
			ver.col.b = 1;
			ver.col.a = 1;
		}
		return;
	}

	// 頂点カラーデータの取得
	FbxGeometryElementVertexColor* vertex_colors = pMesh->GetElementVertexColor(0);

	if (vertex_colors == nullptr)
	{
		return;
	}

	FbxLayerElement::EMappingMode mapping_mode = vertex_colors->GetMappingMode();
	FbxLayerElement::EReferenceMode reference_mode = vertex_colors->GetReferenceMode();

	if (mapping_mode == FbxLayerElement::eByPolygonVertex)
	{
		if (reference_mode == FbxLayerElement::eIndexToDirect)
		{
			// 頂点カラーバッファ取得
			FbxLayerElementArrayTemplate<FbxColor>& colors = vertex_colors->GetDirectArray();
			// 頂点カラーインデックスバッファ取得
			FbxLayerElementArrayTemplate<int>& indeces = vertex_colors->GetIndexArray();
			for (int i = 0; i < indeces.GetCount(); i++)
			{
				int id = indeces.GetAt(i);
				FbxColor color = colors.GetAt(id);
				meshData.vertices[i].col.a = (float)color.mAlpha;
				meshData.vertices[i].col.r = (float)color.mRed;
				meshData.vertices[i].col.g = (float)color.mGreen;
				meshData.vertices[i].col.b = (float)color.mBlue;
			}
		}
	}
}
void FbxMeshFile::LoadUV(stMeshData& meshData, FbxMesh* pMesh)
{
	FbxStringList _uvset_names;
	// UVSetの名前リストを取得
	pMesh->GetUVSetNames(_uvset_names);

	FbxArray<FbxVector2> _uv_buffer;

	// UVSetの名前からUVSetを取得する
	// マルチテクスチャには対応しないので最初の名前を使う
	pMesh->GetPolygonVertexUVs(_uvset_names.GetStringAt(0), _uv_buffer);

	for (int i = 0; i < _uv_buffer.Size(); i++)
	{
		FbxVector2& uv = _uv_buffer[i];

		meshData.vertices[i].tex.x = (float)uv[0];
		meshData.vertices[i].tex.y = (float)(1.0 - uv[1]);
	}
}

void FbxMeshFile::SetMaterialColor(stMaterial& material)
{
	stCBuffer3D& _inputCB = Renderer3D::GetInputCB();
	_inputCB.diffuse = XMFLOAT4(material.diffuse.r, material.diffuse.g, material.diffuse.b, material.diffuse.a);
	_inputCB.ambient = XMFLOAT4(material.ambient.r, material.ambient.g, material.ambient.b, material.ambient.a);
}