#include "../../../../framework.h"
#include "../../../../environment.h"

//���_�v�f
D3D11_INPUT_ELEMENT_DESC hInElementDesc_Sprite[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
};


ID3D11Buffer* SpriteRenderer::pConstantBuffer = NULL;
ID3D11RasterizerState* SpriteRenderer::pRasterizerState = 0;
ID3D11SamplerState* SpriteRenderer::pSamplerState = 0;
ID3D11BlendState* SpriteRenderer::pBlendState = 0;
ID3D11DepthStencilState* SpriteRenderer::pDepthStencilState = 0;
ID3D11InputLayout* SpriteRenderer::pInputLayout = 0;
UINT SpriteRenderer::VertexStrides = sizeof(stVertex2D);
UINT SpriteRenderer::VertexOffsets = 0;

bool SpriteRenderer::Initialize() {
	//���X�^���C�U�̐ݒ�
	D3D11_RASTERIZER_DESC hRasterizerDesc = {
		D3D11_FILL_SOLID,	//�t�B�����[�h
		D3D11_CULL_NONE,	//�J�����O���[�h
		FALSE,
		0,
		0.0f,
		FALSE,
		FALSE,
		FALSE,
		FALSE,
		FALSE
	};

	//���X�^���C�U�̍쐬
	Direct3D::getDevice()->CreateRasterizerState(&hRasterizerDesc, &pRasterizerState);

	//�u�����h�X�e�[�g�̐ݒ�
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_RED | D3D11_COLOR_WRITE_ENABLE_GREEN | D3D11_COLOR_WRITE_ENABLE_BLUE;

	//�u�����h�X�e�[�g�̍쐬
	Direct3D::getDevice()->CreateBlendState(&BlendDesc, &pBlendState);

	// �T���v���X�e�[�g�̐ݒ�
	D3D11_SAMPLER_DESC smpDesc;
	memset(&smpDesc, 0, sizeof(smpDesc));
	smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	smpDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	smpDesc.MinLOD = 0;
	smpDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// �T���v���X�e�[�g�̐���
	Direct3D::getDevice()->CreateSamplerState(&smpDesc, &pSamplerState);

	//�[�x�X�e���V���X�e�[�g�̐ݒ�
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;

	//�[�x�X�e���V���X�e�[�g�̍쐬
	Direct3D::getDevice()->CreateDepthStencilState(&dsDesc, &pDepthStencilState);

	//���_���C�A�E�g�̍쐬
	Shader::VertexShader* vs = Shader::getVertexShader(Shader::eVertexShader::VS_2D);
	Direct3D::getDevice()->CreateInputLayout(hInElementDesc_Sprite, ARRAYSIZE(hInElementDesc_Sprite), vs->getCode(), vs->getLength(), &pInputLayout);

	//�R���X�^���g�o�b�t�@�[�쐬
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(float) * 4;
	cb.CPUAccessFlags = 0;
	cb.MiscFlags = 0;
	cb.StructureByteStride = sizeof(float) * 4;
	cb.Usage = D3D11_USAGE_DEFAULT;


	if (FAILED(Direct3D::getDevice()->CreateBuffer(&cb, NULL, &pConstantBuffer)))
	{
		return FALSE;
	}

	//�X�N���[�����W���r���[�|�[�g���W�֕ϊ�����W��
	float w = (float)SCREEN_WIDTH;
	float h = (float)SCREEN_HEIGHT;
	float scr[4];
	scr[0] = 2.0f / w;
	scr[1] = -2.0f / h;
	scr[2] = -w * 0.5f;
	scr[3] = -h * 0.5f;
	//�萔�o�b�t�@�֒l���Z�b�g
	Direct3D::getDeviceContext()->UpdateSubresource(pConstantBuffer, 0, NULL, scr, 0, 0);

	return TRUE;
}

void SpriteRenderer::Destroy() {
	SAFE_RELEASE(pRasterizerState);
	SAFE_RELEASE(pBlendState);
	SAFE_RELEASE(pSamplerState);
	SAFE_RELEASE(pDepthStencilState);
	SAFE_RELEASE(pInputLayout);
	SAFE_RELEASE(pConstantBuffer);
}

SpriteRenderer::SpriteRenderer() 
	: Component(eComponentType::SpriteRenderer)
{
	renderPriority = 0;
	sortSwitch = true;

	this->sizeX = sizeX;
	this->sizeY = sizeY;

	for (int i = 0; i < Sprite::VertexNum; i++) {
		vtx[i].r = 1; vtx[i].g = 1; vtx[i].b = 1; vtx[i].a = 1;
	}
}

SpriteRenderer::~SpriteRenderer() {
}

//�R���|�[�l���g�̏�����
void SpriteRenderer::SetUpSpriteRenderer(float sizeX, float sizeY, noDel_ptr<Sprite> sprite) {
	this->sizeX = sizeX;
	this->sizeY = sizeY;

	if (sprite != nullptr) {
		pRenderSprite = sprite;
		SetDefaultState();
	}
}

void SpriteRenderer::Execute() {
	Render();
}

void SpriteRenderer::SetVertexState() {
	float PI = 3.14f;

	//���W
	float vtx_x = 0;
	float vtx_y = 0;

	stVector3* pos = &transform->position;
	stVector3* scale = &transform->scale;
	stVector3* rot = &transform->rotation;


	//�e���_�̍��W��ݒ�
	vtx_x = pos->x + (-sizeX * 0.5f) * scale->x;
	vtx_y = pos->y + (-sizeY * 0.5f) * scale->y;
	vtx[0].x = (vtx_x - pos->x) * cosf(rot->z) - (vtx_y - pos->y) * sinf(rot->z) + pos->x;
	vtx[0].y = (vtx_x - pos->x) * sinf(rot->z) + (vtx_y - pos->y) * cosf(rot->z) + pos->y;

	vtx_x = pos->x + (+sizeX * 0.5f) * scale->x;
	vtx_y = pos->y + (-sizeY * 0.5f) * scale->y;
	vtx[1].x = (vtx_x - pos->x) * cosf(rot->z) - (vtx_y - pos->y) * sinf(rot->z) + pos->x;
	vtx[1].y = (vtx_x - pos->x) * sinf(rot->z) + (vtx_y - pos->y) * cosf(rot->z) + pos->y;

	vtx_x = pos->x + (-sizeX * 0.5f) * scale->x;
	vtx_y = pos->y + (+sizeY * 0.5f) * scale->y;
	vtx[2].x = (vtx_x - pos->x) * cosf(rot->z) - (vtx_y - pos->y) * sinf(rot->z) + pos->x;
	vtx[2].y = (vtx_x - pos->x) * sinf(rot->z) + (vtx_y - pos->y) * cosf(rot->z) + pos->y;

	vtx_x = pos->x + (+sizeX * 0.5f) * scale->x;
	vtx_y = pos->y + (+sizeY * 0.5f) * scale->y;
	vtx[3].x = (vtx_x - pos->x) * cosf(rot->z) - (vtx_y - pos->y) * sinf(rot->z) + pos->x;
	vtx[3].y = (vtx_x - pos->x) * sinf(rot->z) + (vtx_y - pos->y) * cosf(rot->z) + pos->y;
}

void SpriteRenderer::Render() {
	if (pRenderSprite == NULL) return;

	//���_���W�̐ݒ�
	SetVertexState();

	//���_�o�b�t�@�̍X�V
	Direct3D::getDeviceContext()->UpdateSubresource(pRenderSprite->GetPVertexBuffer(), 0, NULL, vtx, 0, 0);

	//�u�����h�X�e�[�g���R���e�L�X�g�ɐݒ�
	Direct3D::getDeviceContext()->OMSetBlendState(pBlendState, NULL, 0xffffffff);
	//�[�x�X�e���V���X�e�[�g���R���e�L�X�g�ɐݒ�
	Direct3D::getDeviceContext()->OMSetDepthStencilState(pDepthStencilState, 0);
	//���X�^���C�U�[���R���e�L�X�g�ɐݒ�
	Direct3D::getDeviceContext()->RSSetState(pRasterizerState);
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	Direct3D::getDeviceContext()->IASetInputLayout(pInputLayout);
	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	Direct3D::getDeviceContext()->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	Direct3D::getDeviceContext()->IASetVertexBuffers(0, 1, pRenderSprite->GetPPVertexBuffer(), &VertexStrides, &VertexOffsets);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	Direct3D::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//�T���v���[�X�e�[�g���R���e�L�X�g�ɐݒ�
	Direct3D::getDeviceContext()->PSSetSamplers(0, 1, &pSamplerState);
	//�e�N�X�`���[���R���e�L�X�g�ɐݒ�
	Direct3D::setShaderResource(pRenderSprite->pTextureView);
	//�g�p����V�F�[�_�[�̓o�^	
	Direct3D::getDeviceContext()->VSSetShader(Shader::getVertexShader(Shader::eVertexShader::VS_2D)->getShader(), NULL, 0);
	Direct3D::getDeviceContext()->PSSetShader(Shader::getPixelShader(Shader::ePixelShader::PS_2D)->getShader(), NULL, 0);

	//�v���~�e�B�u�������_�����O
	Direct3D::getDeviceContext()->Draw(4, 0);
}

//setter/getter
void SpriteRenderer::SetSize(float width, float height) {
	sizeX = width;
	sizeY = height;
}
void SpriteRenderer::SetColor(float r, float g, float b, float a) {
	for (int i = 0; i < Sprite::VertexNum; i++) {
		vtx[i].r = r;
		vtx[i].g = g;
		vtx[i].b = b;
		vtx[i].a = a;
	}
}
void SpriteRenderer::SetColor(stColor4 color) {
	for (int i = 0; i < Sprite::VertexNum; i++) {
		this->vtx[i].r = color.r;
		this->vtx[i].g = color.g;
		this->vtx[i].b = color.b;
		this->vtx[i].a = color.a;
	}
}
void SpriteRenderer::SetDefaultState() {
	if (pRenderSprite == NULL) return; //�X�v���C�g���Ȃ��ꍇreturn
	for (int i = 0; i < Sprite::VertexNum; i++) {
		vtx[i] = pRenderSprite->GetVertexState(i);
	}
}
void SpriteRenderer::SetRenderPriority(int value) {
	if (renderPriority != value) SceneManager::GetScene(gameObject->GetSceneType())->SetSpriteSortEnable();
	renderPriority = value;
}
int SpriteRenderer::GetRenderPriority() {
	return renderPriority;
}
void SpriteRenderer::SetSortSwitch(bool flag) {
	sortSwitch = flag;
}
bool SpriteRenderer::isSortSwitch() {
	return sortSwitch;
}
