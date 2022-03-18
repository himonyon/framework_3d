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
		// �C���f�b�N�X�o�b�t�@�̐� = �}�e���A���̐��������b�V����`�悷��
			// IA(InputAssemblerStage)�ɓ��̓��C�A�E�g��ݒ肷��
		Direct3D::getDeviceContext()->IASetInputLayout(MeshRenderer::GetInputLayout());
		// IA�ɐݒ肷�钸�_�o�b�t�@�̎w��
		Direct3D::getDeviceContext()->IASetVertexBuffers(
			0,								// �o�b�t�@���M�̃X���b�g�ԍ�
			1,								// �o�b�t�@�̐�
			&mesh.vertexBuffer,	// ���_�o�b�t�@
			&strides,						// �o�b�t�@�Ɏg�p���Ă���\���̂̃T�C�Y
			&offsets);						// �J�n�I�t�Z�b�g

		Direct3D::getDeviceContext()->IASetIndexBuffer(
			mesh.indexBuffer,
			DXGI_FORMAT_R32_UINT,
			0);

		// ���[���h�}�g���N�X�ݒ�
		XMMATRIX world_matrix;
		XMMATRIX translate = XMMatrixTranslation(pos.x, pos.y, pos.z);
		XMMATRIX rotate_x = XMMatrixRotationX(XMConvertToRadians(rot.x));
		XMMATRIX rotate_y = XMMatrixRotationY(XMConvertToRadians(rot.y));
		XMMATRIX rotate_z = XMMatrixRotationZ(XMConvertToRadians(rot.z));
		XMMATRIX scale_mat = XMMatrixScaling(scl.x, scl.y, scl.z);
		world_matrix = scale_mat * rotate_x * rotate_y * rotate_z * translate;

		// ���[���h�}�g���N�X���R���X�^���g�o�b�t�@�ɐݒ�
		XMStoreFloat4x4(&MeshRenderer::GetInputCB().world, XMMatrixTranspose(world_matrix));

		SetMaterialColor(umMaterials[mesh.materialName]);

		// �e�N�X�`���ݒ�
		// Sampler�̐ݒ�
		Direct3D::getDeviceContext()->PSSetSamplers(
			0,					// �X���b�g�ԍ�
			1,					// �T���v���[�̐�
			MeshRenderer::GetSampleLinear());	// ID3D11SamplerState

		if (umMaterialLinks.count(mesh.materialName) > 0)
		{
			// PixelShader�Ŏg�p����e�N�X�`���̐ݒ�
			Direct3D::getDeviceContext()->PSSetShaderResources(
				0,								// �X���b�g�ԍ�
				1,								// ���\�[�X�̐�
				&umMaterialLinks[mesh.materialName]);
		}
		else
		{
			
			// PixelShader�Ŏg�p����e�N�X�`���̐ݒ�
			Direct3D::getDeviceContext()->PSSetShaderResources(
				0,								// �X���b�g�ԍ�
				1,								// ���\�[�X�̐�
				nullptr);
		}

		// �R���X�^���g�o�b�t�@�X�V
		Direct3D::getDeviceContext()->UpdateSubresource(MeshRenderer::GetConstantBuffer(), 0, NULL, &MeshRenderer::GetInputCB(), 0, 0);

		ID3D11Buffer* constant_buffer = MeshRenderer::GetConstantBuffer();
		// �R���e�L�X�g�ɃR���X�^���g�o�b�t�@��ݒ�
		Direct3D::getDeviceContext()->VSSetConstantBuffers(0, 1, &constant_buffer);
		Direct3D::getDeviceContext()->PSSetConstantBuffers(0, 1, &constant_buffer);

		// �`��
		Direct3D::getDeviceContext()->DrawIndexed(
			(UINT)mesh.indices.size(),	// ���_��
			0,								// �I�t�Z�b�g
			0);								// �J�n���_�̃C���f�b�N�X
	}
}

bool FbxMeshFile::CreateFbx(const char* fileName) {
	//FbxManager�̍쐬
	FbxManager* pFbxManager = FbxManager::Create();
	if (pFbxManager == nullptr) {
		return false;
	}
	//FbxImporter�̍쐬
	FbxImporter* pFbxImporter = FbxImporter::Create(pFbxManager, "Importer");
	if (pFbxImporter == nullptr) {
		pFbxManager->Destroy();
		return false;
	}
	//FbxScene�̍쐬
	FbxScene* pFbxScene = FbxScene::Create(pFbxManager, "test");
	if (pFbxScene == nullptr) {
		pFbxImporter->Destroy();
		pFbxManager->Destroy();
		return false;
	}

	//������
	pFbxImporter->Initialize(fileName);
	// scene�ɃC���|�[�g
	pFbxImporter->Import(pFbxScene);

	FbxGeometryConverter converter(pFbxManager);
	// ���b�V���Ɏg���Ă���}�e���A���P�ʂŃ��b�V���𕪊�����
	converter.SplitMeshesPerMaterial(pFbxScene, true);
	// �|���S�����O�p�`�ɂ���
	converter.Triangulate(pFbxScene, true);

	int material_num = pFbxScene->GetSrcObjectCount<FbxSurfaceMaterial>();
	for (int i = 0; i < material_num; i++)
	{
		LoadMaterial(pFbxScene->GetSrcObject<FbxSurfaceMaterial>(i));
	}

	// FbxMesh�̐����擾
	int mesh_num = pFbxScene->GetSrcObjectCount<FbxMesh>();

	for (int i = 0; i < mesh_num; i++)
	{
		// Mesh�쐬
		CreateMesh(pFbxScene->GetSrcObject<FbxMesh>(i));
	}

	//�e�N�X�`���̐����擾
	int texture_num = pFbxScene->GetSrcObjectCount<FbxFileTexture>();
	//�e�N�X�`��
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

	//�j��
	pFbxImporter->Destroy();
	pFbxScene->Destroy();
	pFbxManager->Destroy();
	
	return true;
}
bool FbxMeshFile::CreateVertexBuffer() {
	for (auto& mesh : vMeshList)
	{
		//���_�o�b�t�@�쐬
		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.ByteWidth = sizeof(stVertex3D) * (UINT)mesh.vertices.size();	// �o�b�t�@�̃T�C�Y
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;			// �g�p���@
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// BIND�ݒ�
		buffer_desc.CPUAccessFlags = 0;						// ���\�[�X�ւ�CPU�̃A�N�Z�X�����ɂ��Ă̐ݒ�
		buffer_desc.MiscFlags = 0;							// ���\�[�X�I�v�V�����̃t���O
		buffer_desc.StructureByteStride = 0;				// �\���̂̃T�C�Y

		D3D11_SUBRESOURCE_DATA sub_resource;
		sub_resource.pSysMem = &mesh.vertices[0];	// �o�b�t�@�̒��g�̐ݒ�
		sub_resource.SysMemPitch = 0;			// texture�f�[�^���g�p����ۂɎg�p���郁���o
		sub_resource.SysMemSlicePitch = 0;		// texture�f�[�^���g�p����ۂɎg�p���郁���o

		// �o�b�t�@�쐬
		if (FAILED(Direct3D::getDevice()->CreateBuffer(
			&buffer_desc,								// �o�b�t�@���
			&sub_resource,								// ���\�[�X���
			&mesh.vertexBuffer)))	// �쐬���ꂽ�o�b�t�@�̊i�[��
		{
			return false;
		}
	}

	return true;
}
bool FbxMeshFile::CreateIndexBuffer() {
	for (auto& mesh : vMeshList)
	{
		//���_�o�b�t�@�쐬
		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.ByteWidth = sizeof(UINT) * (UINT)mesh.indices.size();	// �o�b�t�@�̃T�C�Y
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;							// �g�p���@
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;					// BIND�ݒ�
		buffer_desc.CPUAccessFlags = 0;										// ���\�[�X�ւ�CPU�̃A�N�Z�X�����ɂ��Ă̐ݒ�
		buffer_desc.MiscFlags = 0;											// ���\�[�X�I�v�V�����̃t���O
		buffer_desc.StructureByteStride = 0;								// �\���̂̃T�C�Y

		D3D11_SUBRESOURCE_DATA sub_resource;
		sub_resource.pSysMem = &mesh.indices[0];							// �o�b�t�@�̒��g�̐ݒ�
		sub_resource.SysMemPitch = 0;										// texture�f�[�^���g�p����ۂɎg�p���郁���o
		sub_resource.SysMemSlicePitch = 0;									// texture�f�[�^���g�p����ۂɎg�p���郁���o

		// �o�b�t�@�쐬
		if (FAILED(Direct3D::getDevice()->CreateBuffer(
			&buffer_desc,						// �o�b�t�@���
			&sub_resource,						// ���\�[�X���
			&mesh.indexBuffer)))		// �쐬���ꂽ�o�b�t�@�̊i�[��
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
	// �|���S�����̎擾
	int polygon_count = pMesh->GetPolygonCount();

	// �|���S���̐������A�ԂƂ��ĕۑ�����
	for (int i = 0; i < polygon_count; i++)
	{
		meshData.indices.emplace_back(i * 3 + 2);
		meshData.indices.emplace_back(i * 3 + 1);
		meshData.indices.emplace_back(i * 3);
	}
}
void FbxMeshFile::LoadVertices(stMeshData& meshData, FbxMesh* pMesh) {
	// ���_�o�b�t�@�̎擾
	FbxVector4* vertices = pMesh->GetControlPoints();
	// �C���f�b�N�X�o�b�t�@�̎擾
	int* indices = pMesh->GetPolygonVertices();
	// ���_���W�̐��̎擾
	int polygonVertexCount = pMesh->GetPolygonVertexCount();

	//�e���_�̍��W�̐ݒ�
	for (int i = 0; i < polygonVertexCount; i++)
	{
		stVertex3D vertex;
		// �C���f�b�N�X�o�b�t�@���璸�_�ԍ����擾
		int index = indices[i];

		// ���_���W���X�g������W���擾����
		vertex.pos.x = (float)-vertices[index][0];
		vertex.pos.y = (float)vertices[index][1];
		vertex.pos.z = (float)vertices[index][2];

		// �ǉ�
		meshData.vertices.emplace_back(vertex);
	}
}
void FbxMeshFile::LoadNormals(stMeshData& meshData, FbxMesh* pMesh) {
	FbxArray<FbxVector4> normals;
	// �@�����X�g�̎擾
	pMesh->GetPolygonVertexNormals(normals);

	// �@���ݒ�
	for (int i = 0; i < normals.Size(); i++)
	{
		meshData.vertices[i].nor.x = (float)-normals[i][0];
		meshData.vertices[i].nor.y = (float)normals[i][1];
		meshData.vertices[i].nor.z = (float)normals[i][2];
	}
}
void FbxMeshFile::LoadColors(stMeshData& meshData, FbxMesh* pMesh) {
	// ���_�J���[�f�[�^�̐����m�F
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

	// ���_�J���[�f�[�^�̎擾
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
			// ���_�J���[�o�b�t�@�擾
			FbxLayerElementArrayTemplate<FbxColor>& colors = vertex_colors->GetDirectArray();
			// ���_�J���[�C���f�b�N�X�o�b�t�@�擾
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
	// UVSet�̖��O���X�g���擾
	pMesh->GetUVSetNames(uvset_names);

	FbxArray<FbxVector2> uv_buffer;

	// UVSet�̖��O����UVSet���擾����
	// ����̓}���`�e�N�X�`���ɂ͑Ή����Ȃ��̂ōŏ��̖��O���g��
	pMesh->GetPolygonVertexUVs(uvset_names.GetStringAt(0), uv_buffer);

	for (int i = 0; i < uv_buffer.Size(); i++)
	{
		FbxVector2& uv = uv_buffer[i];

		// ���̂܂܎g�p���Ė��Ȃ�
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

	// �e�N�X�`���ǂݍ���(�V���O���Ή�)
	// Diffuse�v���p�e�B���擾
	prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
	FbxFileTexture* texture = nullptr;
	std::string keyword;
	int texture_num = prop.GetSrcObjectCount<FbxFileTexture>();
	if (texture_num > 0)
	{
		// prop����FbxFileTexture���擾	
		texture = prop.GetSrcObject<FbxFileTexture>(0);
	}
	else
	{
		// FbxLayeredTexture����FbxFileTexture���擾	
		int layer_num = prop.GetSrcObjectCount<FbxLayeredTexture>();
		if (layer_num > 0)
		{
			texture = prop.GetSrcObject<FbxFileTexture>(0);
		}
	}

	if (texture != nullptr &&
		LoadTexture(texture, keyword) == true)
	{
		// �ǂݍ��񂾃e�N�X�`���ƃ}�e���A���̊֌W���o���Ă���
		umMaterialLinks[material->GetName()] = umTextures[keyword];
	}
}
bool FbxMeshFile::LoadTexture(FbxFileTexture* texture, std::string& keyword)
{
	if (texture == nullptr)
	{
		return false;
	}
	// �t�@�C�������擾
	std::string _file_path = texture->GetRelativeFileName();

	// �t�@�C������
	char buffer[256];
	ZeroMemory(buffer, sizeof(char) * 256);
	memcpy(buffer, _file_path.c_str(), sizeof(char) * 256);

	// �L������
	Replace('\\', '/', buffer);
	std::vector<std::string> split_list;
	std::string replace_file_name = buffer;
	// �u/�v�ŕ���
	Split('/', buffer, split_list);

	std::string root_path = "Data/Image/Fbx/";
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
	std::wstring wstr_file_name = cv.from_bytes(root_path + split_list[split_list.size() - 1]);

	// ���������΍�
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

		// �摜����V�F�[�_���\�[�XView�̍쐬
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
		//�e�N�X�`���[�쐬
		TexMetadata imagedata;
		auto image = std::make_unique<ScratchImage>();
		if (FAILED(LoadFromWICFile(wstr_file_name.c_str(), WIC_FLAGS_NONE, &imagedata, *image)))
		{
			return false;
		}
		if (FAILED(CreateShaderResourceView(Direct3D::getDevice(), image->GetImages(), image->GetImageCount(), imagedata, &umTextures[file_name])))
		{
			// ���s
			return false;
		}
	}


	keyword = file_name;
	FbxFree(file_name);
	return true;
}

void FbxMeshFile::SetMaterialName(stMeshData& meshData, FbxMesh* pMesh)
{
	// �}�e���A����������ΏI���
	if (pMesh->GetElementMaterialCount() == 0)
	{
		meshData.materialName = "";
		return;
	}

	// Mesh���̃}�e���A�������擾
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