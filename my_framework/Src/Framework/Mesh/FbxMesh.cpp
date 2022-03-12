#include "../../../environment.h"
#include "../../../DirectXTex/DirectXTex.h"

#pragma comment(lib,"DirectXTex/Bin/Debug/DirectXTex.lib")

using namespace MyFrameWork;

FbxMeshFile::FbxMeshFile(const char* fileName) {
	if (CreateFbx(fileName) == false)
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
	for (auto& mesh : vMeshList) {
		mesh.vertexBuffer->Release();
		mesh.indexBuffer->Release();
	}

	for (auto& res : umMaterialLinks) {
		res.second->Release();
	}
}

void FbxMeshFile::Render(stVector3 pos, stVector3 rot, stVector3 scl) {
	UINT strides = sizeof(stVertex3D);
	UINT offsets = 0;

	for (auto& mesh : vMeshList)
	{
		// インデックスバッファの数 = マテリアルの数だけメッシュを描画する
			// IA(InputAssemblerStage)に入力レイアウトを設定する
		Direct3D::getDeviceContext()->IASetInputLayout(MeshRenderer::GetInputLayout());
		// IAに設定する頂点バッファの指定
		Direct3D::getDeviceContext()->IASetVertexBuffers(
			0,								// バッファ送信のスロット番号
			1,								// バッファの数
			&mesh.vertexBuffer,	// 頂点バッファ
			&strides,						// バッファに使用している構造体のサイズ
			&offsets);						// 開始オフセット

		Direct3D::getDeviceContext()->IASetIndexBuffer(
			mesh.indexBuffer,
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
		XMStoreFloat4x4(&MeshRenderer::GetInputCB().world, XMMatrixTranspose(world_matrix));

		SetMaterialColor(umMaterials[mesh.materialName]);

		// テクスチャ設定
		// Samplerの設定
		Direct3D::getDeviceContext()->PSSetSamplers(
			0,					// スロット番号
			1,					// サンプラーの数
			MeshRenderer::GetSampleLinear());	// ID3D11SamplerState

		if (umMaterialLinks.count(mesh.materialName) > 0)
		{
			// PixelShaderで使用するテクスチャの設定
			Direct3D::getDeviceContext()->PSSetShaderResources(
				0,								// スロット番号
				1,								// リソースの数
				&umMaterialLinks[mesh.materialName]);
		}
		else
		{
			
			// PixelShaderで使用するテクスチャの設定
			Direct3D::getDeviceContext()->PSSetShaderResources(
				0,								// スロット番号
				1,								// リソースの数
				nullptr);
		}

		// コンスタントバッファ更新
		Direct3D::getDeviceContext()->UpdateSubresource(MeshRenderer::GetConstantBuffer(), 0, NULL, &MeshRenderer::GetInputCB(), 0, 0);

		ID3D11Buffer* constant_buffer = MeshRenderer::GetConstantBuffer();
		// コンテキストにコンスタントバッファを設定
		Direct3D::getDeviceContext()->VSSetConstantBuffers(0, 1, &constant_buffer);
		Direct3D::getDeviceContext()->PSSetConstantBuffers(0, 1, &constant_buffer);

		// 描画
		Direct3D::getDeviceContext()->DrawIndexed(
			(UINT)mesh.indices.size(),	// 頂点数
			0,								// オフセット
			0);								// 開始頂点のインデックス
	}
}

bool FbxMeshFile::CreateFbx(const char* fileName) {
	//FbxManagerの作成
	FbxManager* pFbxManager = FbxManager::Create();
	if (pFbxManager == nullptr) {
		return false;
	}
	//FbxImporterの作成
	FbxImporter* pFbxImporter = FbxImporter::Create(pFbxManager, "Importer");
	if (pFbxImporter == nullptr) {
		pFbxManager->Destroy();
		return false;
	}
	//FbxSceneの作成
	FbxScene* pFbxScene = FbxScene::Create(pFbxManager, "test");
	if (pFbxScene == nullptr) {
		pFbxImporter->Destroy();
		pFbxManager->Destroy();
		return false;
	}

	//初期化
	pFbxImporter->Initialize(fileName);
	// sceneにインポート
	pFbxImporter->Import(pFbxScene);

	FbxGeometryConverter converter(pFbxManager);
	// メッシュに使われているマテリアル単位でメッシュを分割する
	converter.SplitMeshesPerMaterial(pFbxScene, true);
	// ポリゴンを三角形にする
	converter.Triangulate(pFbxScene, true);

	int material_num = pFbxScene->GetSrcObjectCount<FbxSurfaceMaterial>();
	for (int i = 0; i < material_num; i++)
	{
		LoadMaterial(pFbxScene->GetSrcObject<FbxSurfaceMaterial>(i));
	}

	// FbxMeshの数を取得
	int mesh_num = pFbxScene->GetSrcObjectCount<FbxMesh>();

	for (int i = 0; i < mesh_num; i++)
	{
		// Mesh作成
		CreateMesh(pFbxScene->GetSrcObject<FbxMesh>(i));
	}

	//テクスチャの数を取得
	int texture_num = pFbxScene->GetSrcObjectCount<FbxFileTexture>();
	//テクスチャ
	for (int i = 0; i < texture_num; i++)
	{
		FbxFileTexture* texture = pFbxScene->GetSrcObject<FbxFileTexture>(i);
		if (texture)
		{
			const char* file_name01 = texture->GetFileName();
			const char* file_name02 = texture->GetRelativeFileName();
			int tex = texture->GetSrcObjectCount< FbxSurfaceMaterial>();
		}
	}

	//破棄
	pFbxImporter->Destroy();
	pFbxScene->Destroy();
	pFbxManager->Destroy();
	
	return true;
}
bool FbxMeshFile::CreateVertexBuffer() {
	for (auto& mesh : vMeshList)
	{
		//頂点バッファ作成
		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.ByteWidth = sizeof(stVertex3D) * (UINT)mesh.vertices.size();	// バッファのサイズ
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;			// 使用方法
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// BIND設定
		buffer_desc.CPUAccessFlags = 0;						// リソースへのCPUのアクセス権限についての設定
		buffer_desc.MiscFlags = 0;							// リソースオプションのフラグ
		buffer_desc.StructureByteStride = 0;				// 構造体のサイズ

		D3D11_SUBRESOURCE_DATA sub_resource;
		sub_resource.pSysMem = &mesh.vertices[0];	// バッファの中身の設定
		sub_resource.SysMemPitch = 0;			// textureデータを使用する際に使用するメンバ
		sub_resource.SysMemSlicePitch = 0;		// textureデータを使用する際に使用するメンバ

		// バッファ作成
		if (FAILED(Direct3D::getDevice()->CreateBuffer(
			&buffer_desc,								// バッファ情報
			&sub_resource,								// リソース情報
			&mesh.vertexBuffer)))	// 作成されたバッファの格納先
		{
			return false;
		}
	}

	return true;
}
bool FbxMeshFile::CreateIndexBuffer() {
	for (auto& mesh : vMeshList)
	{
		//頂点バッファ作成
		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.ByteWidth = sizeof(UINT) * (UINT)mesh.indices.size();	// バッファのサイズ
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;							// 使用方法
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;					// BIND設定
		buffer_desc.CPUAccessFlags = 0;										// リソースへのCPUのアクセス権限についての設定
		buffer_desc.MiscFlags = 0;											// リソースオプションのフラグ
		buffer_desc.StructureByteStride = 0;								// 構造体のサイズ

		D3D11_SUBRESOURCE_DATA sub_resource;
		sub_resource.pSysMem = &mesh.indices[0];							// バッファの中身の設定
		sub_resource.SysMemPitch = 0;										// textureデータを使用する際に使用するメンバ
		sub_resource.SysMemSlicePitch = 0;									// textureデータを使用する際に使用するメンバ

		// バッファ作成
		if (FAILED(Direct3D::getDevice()->CreateBuffer(
			&buffer_desc,						// バッファ情報
			&sub_resource,						// リソース情報
			&mesh.indexBuffer)))		// 作成されたバッファの格納先
		{
			return false;
		}
	}

	return true;
}

bool FbxMeshFile::CreateMesh(FbxMesh* pMesh) {
	
	stMeshData mesh_data;
	LoadIndices(mesh_data, pMesh);
	LoadVertices(mesh_data, pMesh);
	LoadNormals(mesh_data, pMesh);
	LoadUV(mesh_data, pMesh);
	LoadColors(mesh_data, pMesh);
	SetMaterialName(mesh_data, pMesh);

	vMeshList.push_back(mesh_data);

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

	//各頂点の座標の設定
	for (int i = 0; i < polygonVertexCount; i++)
	{
		stVertex3D vertex;
		// インデックスバッファから頂点番号を取得
		int index = indices[i];

		// 頂点座標リストから座標を取得する
		vertex.pos.x = (float)-vertices[index][0];
		vertex.pos.y = (float)vertices[index][1];
		vertex.pos.z = (float)vertices[index][2];

		// 追加
		meshData.vertices.emplace_back(vertex);
	}
}
void FbxMeshFile::LoadNormals(stMeshData& meshData, FbxMesh* pMesh) {
	FbxArray<FbxVector4> normals;
	// 法線リストの取得
	pMesh->GetPolygonVertexNormals(normals);

	// 法線設定
	for (int i = 0; i < normals.Size(); i++)
	{
		meshData.vertices[i].nor.x = (float)-normals[i][0];
		meshData.vertices[i].nor.y = (float)normals[i][1];
		meshData.vertices[i].nor.z = (float)normals[i][2];
	}
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
	FbxStringList uvset_names;
	// UVSetの名前リストを取得
	pMesh->GetUVSetNames(uvset_names);

	FbxArray<FbxVector2> uv_buffer;

	// UVSetの名前からUVSetを取得する
	// 今回はマルチテクスチャには対応しないので最初の名前を使う
	pMesh->GetPolygonVertexUVs(uvset_names.GetStringAt(0), uv_buffer);

	for (int i = 0; i < uv_buffer.Size(); i++)
	{
		FbxVector2& uv = uv_buffer[i];

		// そのまま使用して問題ない
		meshData.vertices[i].tex.x = (float)uv[0];
		meshData.vertices[i].tex.y = (float)(1.0 - uv[1]);
	}
}
void FbxMeshFile::LoadMaterial(FbxSurfaceMaterial* material)
{
	stMaterial _entryMaterial;
	enum class eMaterialOrder
	{
		Ambient,
		Diffuse,
		Specular,
		MaxOrder,
	};

	FbxDouble3 colors[(int)eMaterialOrder::MaxOrder];
	FbxDouble factors[(int)eMaterialOrder::MaxOrder];
	FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sAmbient);

	if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
	{
		const char* element_check_list[] =
		{
			FbxSurfaceMaterial::sAmbient,
			FbxSurfaceMaterial::sDiffuse,
		};

		const char* factor_check_list[] =
		{
			FbxSurfaceMaterial::sAmbientFactor,
			FbxSurfaceMaterial::sDiffuseFactor,
		};

		for (int i = 0; i < 2; i++)
		{
			prop = material->FindProperty(element_check_list[i]);
			if (prop.IsValid())
			{
				colors[i] = prop.Get<FbxDouble3>();

			}
			else
			{
				colors[i] = FbxDouble3(1.0, 1.0, 1.0);
			}

			prop = material->FindProperty(factor_check_list[i]);
			if (prop.IsValid())
			{
				factors[i] = prop.Get<FbxDouble>();
			}
			else
			{
				factors[i] = 1.0;
			}
		}
	}

	FbxDouble3 color = colors[(int)eMaterialOrder::Ambient];
	FbxDouble factor = factors[(int)eMaterialOrder::Ambient];
	_entryMaterial.SetAmbient((float)color[0], (float)color[1], (float)color[2], (float)factor);

	color = colors[(int)eMaterialOrder::Diffuse];
	factor = factors[(int)eMaterialOrder::Diffuse];
	_entryMaterial.SetDiffuse((float)color[0], (float)color[1], (float)color[2], (float)factor);

	umMaterials[material->GetName()] = _entryMaterial;

	// テクスチャ読み込み(シングル対応)
	// Diffuseプロパティを取得
	prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
	FbxFileTexture* texture = nullptr;
	std::string keyword;
	int texture_num = prop.GetSrcObjectCount<FbxFileTexture>();
	if (texture_num > 0)
	{
		// propからFbxFileTextureを取得	
		texture = prop.GetSrcObject<FbxFileTexture>(0);
	}
	else
	{
		// FbxLayeredTextureからFbxFileTextureを取得	
		int layer_num = prop.GetSrcObjectCount<FbxLayeredTexture>();
		if (layer_num > 0)
		{
			texture = prop.GetSrcObject<FbxFileTexture>(0);
		}
	}

	if (texture != nullptr &&
		LoadTexture(texture, keyword) == true)
	{
		// 読み込んだテクスチャとマテリアルの関係を覚えておく
		umMaterialLinks[material->GetName()] = umTextures[keyword];
	}
}
bool FbxMeshFile::LoadTexture(FbxFileTexture* texture, std::string& keyword)
{
	if (texture == nullptr)
	{
		return false;
	}
	// ファイル名を取得
	std::string _file_path = texture->GetRelativeFileName();

	// ファイル分解
	char buffer[256];
	ZeroMemory(buffer, sizeof(char) * 256);
	memcpy(buffer, _file_path.c_str(), sizeof(char) * 256);

	// 記号統一
	Replace('\\', '/', buffer);
	std::vector<std::string> split_list;
	std::string replace_file_name = buffer;
	// 「/」で分解
	Split('/', buffer, split_list);

	std::string root_path = "Data/Image/Fbx/";
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
	std::wstring wstr_file_name = cv.from_bytes(root_path + split_list[split_list.size() - 1]);

	// 文字化け対策
	char* file_name;
	size_t size = 0;
	FbxUTF8ToAnsi(split_list[split_list.size() - 1].c_str(), file_name, &size);

	/*if (umTextures.count(file_name) > 0 &&
		umTextures[file_name] != nullptr)
	{
		FbxFree(file_name);
		return true;
	}*/

	TexMetadata metadata;
	ScratchImage image;

	if (strstr(file_name, ".tga") != nullptr)
	{
		HRESULT hr = LoadFromTGAFile(wstr_file_name.c_str(), &metadata, image);

		if (FAILED(hr))
		{
			FbxFree(file_name);
			return false;
		}

		// 画像からシェーダリソースViewの作成
		hr = CreateShaderResourceView(
			Direct3D::getDevice(),
			image.GetImages(),
			image.GetImageCount(),
			metadata,
			&umTextures[file_name]);

		if (FAILED(hr))
		{
			FbxFree(file_name);
			return false;
		}
	}
	else
	{
		//テクスチャー作成
		TexMetadata imagedata;
		auto image = std::make_unique<ScratchImage>();
		if (FAILED(LoadFromWICFile(wstr_file_name.c_str(), WIC_FLAGS_NONE, &imagedata, *image)))
		{
			return false;
		}
		if (FAILED(CreateShaderResourceView(Direct3D::getDevice(), image->GetImages(), image->GetImageCount(), imagedata, &umTextures[file_name])))
		{
			// 失敗
			return false;
		}
	}


	keyword = file_name;
	FbxFree(file_name);
	return true;
}

void FbxMeshFile::SetMaterialName(stMeshData& meshData, FbxMesh* pMesh)
{
	// マテリアルが無ければ終わり
	if (pMesh->GetElementMaterialCount() == 0)
	{
		meshData.materialName = "";
		return;
	}

	// Mesh側のマテリアル情報を取得
	FbxLayerElementMaterial* material = pMesh->GetElementMaterial(0);
	int index = material->GetIndexArray().GetAt(0);
	FbxSurfaceMaterial* surface_material = pMesh->GetNode()->GetSrcObject<FbxSurfaceMaterial>(index);

	if (surface_material != nullptr)
	{
		meshData.materialName = surface_material->GetName();
	}
	else
	{
		meshData.materialName = "";
	}
}
void FbxMeshFile::SetMaterialColor(stMaterial& material)
{
	stCBuffer3D& _inputCB = MeshRenderer::GetInputCB();
	_inputCB.diffuse = XMFLOAT4(material.Diffuse[0], material.Diffuse[1], material.Diffuse[2], material.Diffuse[3]);
	_inputCB.ambient = XMFLOAT4(material.Ambient[0], material.Ambient[1], material.Ambient[2], material.Ambient[3]);
}