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
	//�p���擾
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
void FbxMeshFile::LoadVertices(stMeshData& meshData, FbxMesh* pMesh, FbxAMatrix& mat) {
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

		//-------------------------------------------------------------
		vertex.pos.x = (float)-vertices[index][0];
		vertex.pos.y = (float)vertices[index][1];
		vertex.pos.z = (float)vertices[index][2];

	/*	FbxVector4 _temp = { vertex.pos.x, vertex.pos.y, vertex.pos.z, 1 };
		FbxVector4 _res = mat.MultT(_temp);

		vertex.pos.x = (float)-_res[0];
		vertex.pos.y = (float)_res[1];
		vertex.pos.z = (float)_res[2];*/

		// �ǉ�
		meshData.vertices.emplace_back(vertex);
	}
}
void FbxMeshFile::LoadNormals(stMeshData& meshData, FbxMesh* pMesh, FbxAMatrix& mat) {
	FbxArray<FbxVector4> normals;
	// �@�����X�g�̎擾
	pMesh->GetPolygonVertexNormals(normals);

	// �@���ݒ�
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
	//�W�I���g���}�g���b�N�X�擾
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

	//���[�J����Matrix���擾
	FbxAMatrix _localMatrix = node->EvaluateLocalTransform();

	FbxNode* _pParentNode = node->GetParent();
	FbxAMatrix _parentMatrix = _pParentNode->EvaluateLocalTransform();
	while ((_pParentNode = _pParentNode->GetParent()) != NULL)
	{
		_parentMatrix = _pParentNode->EvaluateLocalTransform() * _parentMatrix;
	}
	//�W�I���g���E���[�J���E�e�̃}�g���N�X������
	FbxAMatrix matrix = _parentMatrix * _localMatrix * _matrixGeo;

	//����--------------------------------------------------------------------

	//���[�J�����W
	FbxDouble3 pp = node->LclTranslation.Get();
	FbxDouble3 rr = node->LclRotation.Get();
	FbxDouble3 ss = node->LclScaling.Get();

	FbxDouble3 newRot = node->LclRotation.Get();
	FbxDouble3 newScl = node->LclScaling.Get();

	//���[�J���̃}�g���N�X
	XMMATRIX _local_matrix;
	XMMATRIX _translate = XMMatrixTranslation((float)pp[0], (float)pp[1], (float)pp[2]);
	XMMATRIX _rotateX = XMMatrixRotationX(XMConvertToRadians((float)rr[0]));
	XMMATRIX _rotateY = XMMatrixRotationY(XMConvertToRadians((float)rr[1]));
	XMMATRIX _rotateZ = XMMatrixRotationZ(XMConvertToRadians((float)rr[2]));
	XMMATRIX _scaleMat = XMMatrixScaling((float)ss[0], (float)ss[1], (float)ss[2]);
	_local_matrix = _scaleMat * _rotateX * _rotateY * _rotateZ * _translate;

	//�e�̃}�g���N�X����
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

	//�i�[
	return matrix;
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