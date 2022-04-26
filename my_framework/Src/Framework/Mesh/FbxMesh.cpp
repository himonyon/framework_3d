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

	// �C���f�b�N�X�o�b�t�@�̐� = �}�e���A���̐��������b�V����`�悷��
		// IA(InputAssemblerStage)�ɓ��̓��C�A�E�g��ݒ肷��
	Direct3D::GetDeviceContext()->IASetInputLayout(Renderer3D::GetInputLayout());
	// IA�ɐݒ肷�钸�_�o�b�t�@�̎w��
	Direct3D::GetDeviceContext()->IASetVertexBuffers(
		0,								// �o�b�t�@���M�̃X���b�g�ԍ�
		1,								// �o�b�t�@�̐�
		&meshData->vertexBuffer,	// ���_�o�b�t�@
		&strides,						// �o�b�t�@�Ɏg�p���Ă���\���̂̃T�C�Y
		&offsets);						// �J�n�I�t�Z�b�g

	Direct3D::GetDeviceContext()->IASetIndexBuffer(
		meshData->indexBuffer,
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
	XMStoreFloat4x4(&Renderer3D::GetInputCB().world, XMMatrixTranspose(world_matrix));

	///�F�ݒ�
	SetMaterialColor(*pMaterial.get());

	// �e�N�X�`���ݒ�
	// Sampler�̐ݒ�
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

	// �R���X�^���g�o�b�t�@�X�V
	Direct3D::GetDeviceContext()->UpdateSubresource(Renderer3D::GetConstantBuffer(), 0, NULL, &Renderer3D::GetInputCB(), 0, 0);

	ID3D11Buffer* constant_buffer = Renderer3D::GetConstantBuffer();
	// �R���e�L�X�g�ɃR���X�^���g�o�b�t�@��ݒ�
	Direct3D::GetDeviceContext()->VSSetConstantBuffers(0, 1, &constant_buffer);
	Direct3D::GetDeviceContext()->PSSetConstantBuffers(0, 1, &constant_buffer);

	// �`��
	Direct3D::GetDeviceContext()->DrawIndexed(
		(UINT)meshData->indices.size(),	// ���_��
		0,								// �I�t�Z�b�g
		0);								// �J�n���_�̃C���f�b�N�X
}

bool FbxMeshFile::CreateFbx(FbxMesh* mesh) {
	// Mesh�쐬
	CreateMesh(mesh);
	
	return true;
}
bool FbxMeshFile::CreateVertexBuffer() {
	//���_�o�b�t�@�쐬
	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.ByteWidth = sizeof(stVertex3D) * (UINT)meshData->vertices.size();	// �o�b�t�@�̃T�C�Y
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;			// �g�p���@
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// BIND�ݒ�
	buffer_desc.CPUAccessFlags = 0;						// ���\�[�X�ւ�CPU�̃A�N�Z�X�����ɂ��Ă̐ݒ�
	buffer_desc.MiscFlags = 0;							// ���\�[�X�I�v�V�����̃t���O
	buffer_desc.StructureByteStride = 0;				// �\���̂̃T�C�Y

	D3D11_SUBRESOURCE_DATA sub_resource;
	sub_resource.pSysMem = &meshData->vertices[0];	// �o�b�t�@�̒��g�̐ݒ�
	sub_resource.SysMemPitch = 0;			// texture�f�[�^���g�p����ۂɎg�p���郁���o
	sub_resource.SysMemSlicePitch = 0;		// texture�f�[�^���g�p����ۂɎg�p���郁���o

	// �o�b�t�@�쐬
	if (FAILED(Direct3D::GetDevice()->CreateBuffer(
		&buffer_desc,								// �o�b�t�@���
		&sub_resource,								// ���\�[�X���
		&meshData->vertexBuffer)))	// �쐬���ꂽ�o�b�t�@�̊i�[��
	{
		return false;
	}

	return true;
}
bool FbxMeshFile::CreateIndexBuffer() {
	//���_�o�b�t�@�쐬
	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.ByteWidth = sizeof(UINT) * (UINT)meshData->indices.size();	// �o�b�t�@�̃T�C�Y
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;							// �g�p���@
	buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;					// BIND�ݒ�
	buffer_desc.CPUAccessFlags = 0;										// ���\�[�X�ւ�CPU�̃A�N�Z�X�����ɂ��Ă̐ݒ�
	buffer_desc.MiscFlags = 0;											// ���\�[�X�I�v�V�����̃t���O
	buffer_desc.StructureByteStride = 0;								// �\���̂̃T�C�Y

	D3D11_SUBRESOURCE_DATA sub_resource;
	sub_resource.pSysMem = &meshData->indices[0];							// �o�b�t�@�̒��g�̐ݒ�
	sub_resource.SysMemPitch = 0;										// texture�f�[�^���g�p����ۂɎg�p���郁���o
	sub_resource.SysMemSlicePitch = 0;									// texture�f�[�^���g�p����ۂɎg�p���郁���o

	// �o�b�t�@�쐬
	if (FAILED(Direct3D::GetDevice()->CreateBuffer(
		&buffer_desc,						// �o�b�t�@���
		&sub_resource,						// ���\�[�X���
		&meshData->indexBuffer)))		// �쐬���ꂽ�o�b�t�@�̊i�[��
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

	//���S���(�p��)�擾
	FbxNode* node = pMesh->GetNode();
	FbxDouble3 _meshPos, _meshRot, _meshScl; //���b�V�����W
	LoadMeshTransform(node, _meshPos, _meshRot, _meshScl);

	//��]�s��̍쐬
	XMMATRIX _rotXM = XMMatrixRotationX(float(_meshRot[0]));
	XMMATRIX _rotYM = XMMatrixRotationY(float(_meshRot[1]));
	XMMATRIX _rotZM = XMMatrixRotationZ(float(_meshRot[2]));
	XMMATRIX _rotM = _rotXM * _rotYM * _rotXM;
	XMMATRIX _newPos;
	//---------------------------------------------------------

	//�e���_�̍��W�̐ݒ�a
	for (int i = 0; i < polygonVertexCount; i++)
	{
		stVertex3D vertex;
		// �C���f�b�N�X�o�b�t�@���璸�_�ԍ����擾
		int index = indices[i];

		// ���_���W���X�g������W���擾 & ���f���̎p���𔽉f������
		vertex.pos.x = (float)-vertices[index][0] * float(_meshScl[0]) + float(_meshPos[0]);
		vertex.pos.y = (float)vertices[index][1] * float(_meshScl[1]) + float(_meshPos[1]);
		vertex.pos.z = (float)vertices[index][2] * float(_meshScl[2]) + float(_meshPos[2]);

		//��]���܂񂾐V���ȍ��W���擾
		XMMATRIX _translate = XMMatrixTranslation(vertex.pos.x, vertex.pos.y, vertex.pos.z);
		_newPos = _translate * _rotM;
		XMFLOAT4X4 _temp;
		XMStoreFloat4x4(&_temp, XMMatrixTranspose(_newPos));
		vertex.pos.x = _temp._14;
		vertex.pos.y = _temp._24;
		vertex.pos.z = _temp._34;

		// �ǉ�
		meshData.vertices.emplace_back(vertex);
	}
}
void FbxMeshFile::LoadNormals(stMeshData& meshData, FbxMesh* pMesh) {
	FbxArray<FbxVector4> normals;
	// �@�����X�g�̎擾
	pMesh->GetPolygonVertexNormals(normals);
	//���S���(�p��)�擾
	FbxNode* node = pMesh->GetNode();
	FbxDouble3 _meshPos, _meshRot, _meshScl; //���b�V�����W
	LoadMeshTransform(node, _meshPos, _meshRot, _meshScl);

	//��]�s��̍쐬
	XMMATRIX _rotXM = XMMatrixRotationX(float(_meshRot.mData[0]));
	XMMATRIX _rotYM = XMMatrixRotationY(float(_meshRot.mData[1]));
	XMMATRIX _rotZM = XMMatrixRotationZ(float(_meshRot.mData[2]));
	XMMATRIX _rotM = _rotXM * _rotYM * _rotXM;
	XMMATRIX _newNor;

	// �@���ݒ�
	for (int i = 0; i < normals.Size(); i++)
	{
		meshData.vertices[i].nor.x = (float)-normals[i][0] * float(_meshScl[0]) + float(_meshPos[0]);
		meshData.vertices[i].nor.y = (float)normals[i][1] * float(_meshScl[1]) + float(_meshPos[1]);
		meshData.vertices[i].nor.z = (float)normals[i][2] * float(_meshScl[2]) + float(_meshPos[2]);

		//��]���܂񂾐V���Ȗ@�����擾
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
	FbxDouble3 _parentPos, _parentRot, _parentScl; //�e���W

	// �e���W�擾
	pos = node->LclTranslation.Get(); //���W
	rot = node->LclRotation.Get(); //��]
	scl = node->LclScaling.Get(); //�X�P�[��
	while (parent != NULL) {
		_parentPos = parent->LclTranslation.Get();
		_parentRot = parent->LclRotation.Get();
		_parentScl = parent->LclScaling.Get();

		//�e���W�ƌ���
		for (int i = 0; i < 3; i++) {
			pos[i] += _parentPos[i];
			rot[i] += _parentRot[i];
			scl[i] *= _parentScl[i];
		}
		//�e�����Ȃ��Ȃ�܂ŒT��
		parent = parent->GetParent();
	}

	//�e�Đݒ�
	parent = node->GetParent();

	//cm���炍�֒�����ϊ�
	for (auto& scl : scl.mData) scl /= 100;
	//�e�����b�V���������Ă���ꍇ���W�𒷂��ϊ�
	if (parent->GetMesh() == NULL) for (auto& pos : pos.mData) pos /= 100;

	//�������v�Z�i�X�P�[�����O��̐e�Ƃ̌����̂��߁j
	FbxDouble3 _geometry = node->GeometricTranslation.Get();
	for (auto& pos : _geometry.mData) pos /= 100; //�P�ʕϊ�
	pos = { pos[0] - _geometry[0],pos[1] - _geometry[1], pos[2] - _geometry[2] };
	FbxDouble3 _defOnScl = { pos[0] * (1 - scl[0]), pos[1] * (1 - scl[1]), pos[2] * (1 - scl[2]) };
	pos = { pos[0] - _defOnScl[0],pos[1] - _defOnScl[1], pos[2] - _defOnScl[2] };
	pos = { _geometry[0] + pos[0],_geometry[1] + pos[1], _geometry[2] + pos[2] };
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
	FbxStringList _uvset_names;
	// UVSet�̖��O���X�g���擾
	pMesh->GetUVSetNames(_uvset_names);

	FbxArray<FbxVector2> _uv_buffer;

	// UVSet�̖��O����UVSet���擾����
	// �}���`�e�N�X�`���ɂ͑Ή����Ȃ��̂ōŏ��̖��O���g��
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