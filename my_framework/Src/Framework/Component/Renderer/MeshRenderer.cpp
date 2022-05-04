#include "../../../../framework.h"
#include "../../../../environment.h"

using namespace MyFrameWork;

MeshRenderer::MeshRenderer() : Renderer(eComponentType::MeshRenderer)
{
}

MeshRenderer::~MeshRenderer() {}

//�R���|�[�l���g�̏�����
void MeshRenderer::SetUpMeshRenderer(noDel_ptr<Mesh> mesh) {
	if (mesh != nullptr) pRenderMesh = mesh;
}

void MeshRenderer::Execute() {
	Render();
}

void MeshRenderer::Render() {

	if (pRenderMesh == NULL) return;
	if (pRenderMesh->GetMeshData() == NULL) return;

	//�ړ��E��]�E�X�P�[�����O�̐ݒ�
	stVector3 _drawPos = GetPosOnCam();
	stVector3 _drawRot = GetRotOnCam();
	stVector3 _drawScale = GetScaleOnCam();

	//�`��O�ݒ�i���ʁj
	Renderer3D::StartRendering();

	//�F�ǂݍ���
	stCBuffer3D& _inputCB = Renderer3D::GetInputCB();
	_inputCB.color = { color.r, color.g, color.b, color.a };

	UINT strides = sizeof(stVertex3D);
	UINT offsets = 0;

	// IA(InputAssemblerStage)�ɓ��̓��C�A�E�g��ݒ肷��
	Direct3D::GetDeviceContext()->IASetInputLayout(Renderer3D::GetInputLayout());

	//���b�V���f�[�^�擾
	noDel_ptr<stMeshData> _meshData = pRenderMesh->GetMeshData();

	// IA�ɐݒ肷�钸�_�o�b�t�@�̎w��
	Direct3D::GetDeviceContext()->IASetVertexBuffers(
		0,								// �o�b�t�@���M�̃X���b�g�ԍ�
		1,								// �o�b�t�@�̐�
		&_meshData->vertexBuffer,	// ���_�o�b�t�@
		&strides,						// �o�b�t�@�Ɏg�p���Ă���\���̂̃T�C�Y
		&offsets);						// �J�n�I�t�Z�b�g

	Direct3D::GetDeviceContext()->IASetIndexBuffer(
		_meshData->indexBuffer,
		DXGI_FORMAT_R32_UINT,
		0);

	// ���[���h�}�g���N�X�ݒ�
	XMMATRIX _world_matrix;
	XMMATRIX _translate = XMMatrixTranslation(_drawPos.x, _drawPos.y, _drawPos.z);
	XMMATRIX _rotateX = XMMatrixRotationX(XMConvertToRadians(_drawRot.x));
	XMMATRIX _rotateY = XMMatrixRotationY(XMConvertToRadians(_drawRot.y));
	XMMATRIX _rotateZ = XMMatrixRotationZ(XMConvertToRadians(_drawRot.z));
	XMMATRIX _scaleMat = XMMatrixScaling(_drawScale.x, _drawScale.y, _drawScale.z);
	_world_matrix = _scaleMat * _rotateX * _rotateY * _rotateZ * _translate;

	// ���[���h�}�g���N�X���R���X�^���g�o�b�t�@�ɐݒ�
	XMStoreFloat4x4(&Renderer3D::GetInputCB().world, XMMatrixTranspose(_world_matrix));

	///�F�ݒ�
	SetMaterialColor(pRenderMesh->GetMaterial());

	// �e�N�X�`���ݒ�
	// Sampler�̐ݒ�
	ID3D11SamplerState* _sampleState = Renderer3D::GetSampleLinear();
	Direct3D::GetDeviceContext()->PSSetSamplers(0, 1, &_sampleState);	// ID3D11SamplerState

	if (pRenderMesh->GetMaterial()->pTexture != NULL)
	{
		Direct3D::GetDeviceContext()->PSSetShader(Shader::getPixelShader(Shader::ePixelShader::PS_3D)->getShader(), nullptr, 0);
		Direct3D::GetDeviceContext()->PSSetShaderResources(0, 1, &pRenderMesh->GetMaterial()->pTexture);
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

	stCBuffer3D& cbb = Renderer3D::GetInputCB();

	// �`��
	Direct3D::GetDeviceContext()->DrawIndexed(
		(UINT)_meshData->indices.size(),	// ���_��
		0,								// �I�t�Z�b�g
		0);								// �J�n���_�̃C���f�b�N�X
}

void MeshRenderer::SetMaterialColor(noDel_ptr<stMaterial> material)
{
	if (material == NULL) return;
	stCBuffer3D& _inputCB = Renderer3D::GetInputCB();
	_inputCB.diffuse = XMFLOAT4(material->diffuse.r, material->diffuse.g, material->diffuse.b, 1);
	_inputCB.ambient = XMFLOAT4(material->ambient.r, material->ambient.g, material->ambient.b, 1);
}