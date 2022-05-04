#include "../../../environment.h"
#include "../../../DirectXTex/DirectXTex.h"

#if _DEBUG
#pragma comment(lib,"DirectXTex/Bin/Debug/DirectXTex.lib")
#else
#pragma comment(lib,"DirectXTex/Bin/Release/DirectXTex.lib")
#endif

using namespace MyFrameWork;

FbxMeshFile::FbxMeshFile(FbxMesh* mesh, eMeshFormat format) : Mesh(format) {

	fbxMeshData = noDel_ptr<FbxMesh>(mesh);

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
	//姿勢取得
	FbxNode* node = pMesh->GetNode();
	FbxAMatrix _matA = LoadMeshTransform(node);

	LoadIndices(*meshData, pMesh);
	LoadVertices(*meshData, pMesh, _matA);
	LoadNormals(*meshData, pMesh, _matA);
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
void FbxMeshFile::LoadVertices(stMeshData& meshData, FbxMesh* pMesh, FbxAMatrix& mat) {
	// 頂点バッファの取得
	FbxVector4* vertices = pMesh->GetControlPoints();
	// インデックスバッファの取得
	int* indices = pMesh->GetPolygonVertices();
	// 頂点座標の数の取得
	int polygonVertexCount = pMesh->GetPolygonVertexCount();

	

	//各頂点の座標の設定
	for (int i = 0; i < polygonVertexCount; i++)
	{
		stVertex3D vertex;
		// インデックスバッファから頂点番号を取得
		int index = indices[i];

		//-------------------------------------------------------------
		vertex.pos.x = (float)-vertices[index][0];
		vertex.pos.y = (float)vertices[index][1];
		vertex.pos.z = (float)vertices[index][2];

	/*	FbxVector4 _temp = { vertex.pos.x, vertex.pos.y, vertex.pos.z, 1 };
		FbxVector4 _res = mat.MultT(_temp);

		vertex.pos.x = (float)-_res[0];
		vertex.pos.y = (float)_res[1];
		vertex.pos.z = (float)_res[2];*/

		// 追加
		meshData.vertices.emplace_back(vertex);
	}
}
void FbxMeshFile::LoadNormals(stMeshData& meshData, FbxMesh* pMesh, FbxAMatrix& mat) {
	FbxArray<FbxVector4> normals;
	// 法線リストの取得
	pMesh->GetPolygonVertexNormals(normals);

	// 法線設定
	for (int i = 0; i < normals.Size(); i++)
	{
		meshData.vertices[i].nor.x = float(-normals[i][0]);
		meshData.vertices[i].nor.y = float(normals[i][1]);
		meshData.vertices[i].nor.z = float(normals[i][2]);

		/*FbxVector4 _temp = { meshData.vertices[i].nor.x, 	meshData.vertices[i].nor.y,	meshData.vertices[i].nor.z, 1 };
		FbxVector4 _res = mat.MultT(_temp);

		meshData.vertices[i].nor.x = (float)-_res[0];
		meshData.vertices[i].nor.y = (float)_res[1];
		meshData.vertices[i].nor.z = (float)_res[2];*/
	}
}
FbxAMatrix FbxMeshFile::LoadMeshTransform(FbxNode* node) {
	//ジオメトリマトリックス取得
	FbxAMatrix _matrixGeo; 
	_matrixGeo.SetIdentity();
	if (node->GetNodeAttribute())
	{
		const FbxVector4 lT = node->GetGeometricTranslation(FbxNode::eSourcePivot);
		const FbxVector4 lR = node->GetGeometricRotation(FbxNode::eSourcePivot);
		const FbxVector4 lS = node->GetGeometricScaling(FbxNode::eSourcePivot);
		_matrixGeo.SetT(lT);
		_matrixGeo.SetR(lR);
		_matrixGeo.SetS(lS);
	}

	//ローカルのMatrixを取得
	FbxAMatrix _localMatrix = node->EvaluateLocalTransform();

	FbxNode* _pParentNode = node->GetParent();
	FbxAMatrix _parentMatrix = _pParentNode->EvaluateLocalTransform();
	while ((_pParentNode = _pParentNode->GetParent()) != NULL)
	{
		_parentMatrix = _pParentNode->EvaluateLocalTransform() * _parentMatrix;
	}
	//ジオメトリ・ローカル・親のマトリクスを結合
	FbxAMatrix matrix = _parentMatrix * _localMatrix * _matrixGeo;

	//実験--------------------------------------------------------------------

	//ローカル座標
	FbxDouble3 pp = node->LclTranslation.Get();
	FbxDouble3 rr = node->LclRotation.Get();
	FbxDouble3 ss = node->LclScaling.Get();

	FbxDouble3 newRot = node->LclRotation.Get();
	FbxDouble3 newScl = node->LclScaling.Get();

	//ローカルのマトリクス
	XMMATRIX _local_matrix;
	XMMATRIX _translate = XMMatrixTranslation((float)pp[0], (float)pp[1], (float)pp[2]);
	XMMATRIX _rotateX = XMMatrixRotationX(XMConvertToRadians((float)rr[0]));
	XMMATRIX _rotateY = XMMatrixRotationY(XMConvertToRadians((float)rr[1]));
	XMMATRIX _rotateZ = XMMatrixRotationZ(XMConvertToRadians((float)rr[2]));
	XMMATRIX _scaleMat = XMMatrixScaling((float)ss[0], (float)ss[1], (float)ss[2]);
	_local_matrix = _scaleMat * _rotateX * _rotateY * _rotateZ * _translate;

	//親のマトリクス生成
	FbxNode* _pParentNodeT = node->GetParent();
	XMMATRIX _parent_matrix = {};
	bool first = true;

	while (_pParentNodeT != NULL)
	{
		pp = _pParentNodeT->LclTranslation.Get();
		rr = _pParentNodeT->LclRotation.Get();
		ss = _pParentNodeT->LclScaling.Get();

		for (int i = 0; i < 3; i++) newRot[i] += rr[i];
		for (int i = 0; i < 3; i++) newScl[i] *= ss[i];

		_translate = XMMatrixTranslation((float)pp[0], (float)pp[1], (float)pp[2]);
		_rotateX = XMMatrixRotationX(XMConvertToRadians((float)rr[0]));
		_rotateY = XMMatrixRotationY(XMConvertToRadians((float)rr[1]));
		_rotateZ = XMMatrixRotationZ(XMConvertToRadians((float)rr[2]));
		_scaleMat = XMMatrixScaling((float)ss[0], (float)ss[1], (float)ss[2]);
		if (first) _parent_matrix = _scaleMat * _rotateX * _rotateY * _rotateZ * _translate;
		else _parent_matrix = _parent_matrix * (_scaleMat * _rotateX * _rotateY * _rotateZ * _translate);

		first = false;

		_pParentNodeT = _pParentNodeT->GetParent();
	}

	XMMATRIX _resultMat = _local_matrix * _parent_matrix;

	_translate = XMMatrixTranslation((float)_resultMat.r[3].m128_f32[0], (float)_resultMat.r[3].m128_f32[1], (float)_resultMat.r[3].m128_f32[2]);
	_rotateX = XMMatrixRotationX(XMConvertToRadians((float)newRot[0]));
	_rotateY = XMMatrixRotationY(XMConvertToRadians((float)newRot[1]));
	_rotateZ = XMMatrixRotationZ(XMConvertToRadians((float)newRot[2]));
	_scaleMat = XMMatrixScaling((float)newScl[0], (float)newScl[1], (float)newScl[2]);
	XMMATRIX _newMatrix = _scaleMat * _rotateX * _rotateY * _rotateZ * _translate;

	initPos = { (float)_resultMat.r[3].m128_f32[0], (float)_resultMat.r[3].m128_f32[1], (float)_resultMat.r[3].m128_f32[2] };
	initRot = { (float)newRot[0] ,(float)newRot[1] ,(float)newRot[2] };
	initScl = { (float)newScl[0] ,(float)newScl[1] ,(float)newScl[2] };

	//格納
	return matrix;
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