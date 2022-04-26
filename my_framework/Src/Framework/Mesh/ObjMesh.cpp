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

	// �`�悷��
	// IA(InputAssemblerStage)�ɓ��̓��C�A�E�g��ݒ肷��
	Direct3D::GetDeviceContext()->IASetInputLayout(Renderer3D::GetInputLayout());
	// IA�ɐݒ肷�钸�_�o�b�t�@�̎w��
	Direct3D::GetDeviceContext()->IASetVertexBuffers(
		0,						// �o�b�t�@���M�̃X���b�g�ԍ�
		1,						// �o�b�t�@�̐�
		&meshData->vertexBuffer,		// ���_�o�b�t�@
		&strides,				// �o�b�t�@�Ɏg�p���Ă���\���̂̃T�C�Y
		&offsets);				// �J�n�I�t�Z�b�g

	Direct3D::GetDeviceContext()->IASetIndexBuffer(
		meshData->indexBuffer,
		DXGI_FORMAT_R32_UINT,
		0);

	// ���[���h�}�g���N�X�ݒ�
	XMMATRIX mWorld;
	XMMATRIX translate = XMMatrixTranslation(pos.x, pos.y, pos.z);
	XMMATRIX rotate_x = XMMatrixRotationX(XMConvertToRadians(rot.x));
	XMMATRIX rotate_y = XMMatrixRotationY(XMConvertToRadians(rot.y));
	XMMATRIX rotate_z = XMMatrixRotationZ(XMConvertToRadians(rot.z));
	XMMATRIX scale_mat = ::XMMatrixScaling(scl.x, scl.y, scl.z);
	mWorld = scale_mat * rotate_x * rotate_y * rotate_z * translate;

	stCBuffer3D& _inputCB = Renderer3D::GetInputCB();

	// ���[���h�}�g���N�X���R���X�^���g�o�b�t�@�ɐݒ�
	XMStoreFloat4x4(&_inputCB.world, XMMatrixTranspose(mWorld));

	//�}�e���A�����Z�b�g
	LoadMaterial(*pMaterial.get());

	// �e�N�X�`���ݒ�
	// Sampler�̐ݒ�
	ID3D11SamplerState* _sampleState = Renderer3D::GetSampleLinear();
	Direct3D::GetDeviceContext()->PSSetSamplers(0, 1, &_sampleState);	// ID3D11SamplerState
	// �}�e���A���Ƀe�N�X�`��������Ȃ�ݒ肷��
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
	// �R���X�^���g�o�b�t�@�X�V
	Direct3D::GetDeviceContext()->UpdateSubresource(_constantBuffer, 0, NULL, &_inputCB, 0, 0);

	// �R���e�L�X�g�ɃR���X�^���g�o�b�t�@��ݒ�
	Direct3D::GetDeviceContext()->VSSetConstantBuffers(0, 1, &_constantBuffer);
	Direct3D::GetDeviceContext()->PSSetConstantBuffers(0, 1, &_constantBuffer);

	// �`��
	Direct3D::GetDeviceContext()->DrawIndexed(
		(UINT)meshData->indices.size(),		// ���_��
		0,						// �I�t�Z�b�g
		0);		
}

bool ObjMesh::CreateVertexBuffer() {
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
		&buffer_desc,				// �o�b�t�@���
		&sub_resource,				// ���\�[�X���
		&meshData->vertexBuffer)))			// �쐬���ꂽ�o�b�t�@�̊i�[��
	{
		return false;
	}

	return true;
}
bool ObjMesh::CreateIndexBuffer() {

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
		&buffer_desc,					// �o�b�t�@���
		&sub_resource,					// ���\�[�X���
		&meshData->indexBuffer)))		// �쐬���ꂽ�o�b�t�@�̊i�[��
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

		// �������_���쐬
		for (int i = 0; i < 3; i++)
		{
			std::ostringstream sout;
			sout << std::setfill('0') << std::setw(5) << vertex_info[i];
			key += sout.str();
		}

		// �d���`�F�b�N
		if (index_list.count(key) > 0)
		{
			// �o�^����Ă��钸�_�o�b�t�@�̗v�f�ԍ����C���f�b�N�X�o�b�t�@�ɕۑ�����
			meshData->indices.push_back(index_list[key]);
		}
		else
		{
			// ���_���ǉ�
			meshData->vertices.push_back(vertex);

			// ���_�o�b�t�@�̗v�f�ԍ��ǉ�
			meshData->indices.push_back((UINT)meshData->vertices.size() - 1);

			// ���_�o�b�t�@�̗v�f�ԍ��ǉ�
			index_list[key] = (int)meshData->vertices.size() - 1;
		}

		add_index_count++;
	}

	UINT size = (UINT)meshData->indices.size();
	UINT temp = 0;

	// �uf�v�L�[���[�h�̕������ʂ�3�𒴂����ꍇ�͒��_��ǉ�����
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
	// Sampler�̐ݒ�
	ID3D11SamplerState* _sampleState = Renderer3D::GetSampleLinear();
	Direct3D::GetDeviceContext()->PSSetSamplers(0,	1, &_sampleState);

	// PixelShader�Ŏg�p����e�N�X�`���̐ݒ�
	Direct3D::GetDeviceContext()->PSSetShaderResources(0,1,	&texture);
}