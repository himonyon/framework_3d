#include "../../environment.h"
#include <DirectXTex.h>
#pragma comment(lib, "DirectXTex.lib")


_NODISCARD static inline auto ToXMVECTOR(const DirectX::XMFLOAT3& vec)
{
	return DirectX::XMLoadFloat3(&vec);
}

//���_�C���v�b�g���C�A�E�g���`	
D3D11_INPUT_ELEMENT_DESC hInElementDesc_Model[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

ID3D11InputLayout* Model:: pVertexLayout = NULL;
ID3D11RasterizerState* Model::pRasterizerState = NULL;
ID3D11BlendState* Model:: pBlendState = NULL;
ID3D11DepthStencilState* Model::pDepthStencilState = NULL;
ID3D11Buffer* Model::pConstantBuffer0 = NULL;
ID3D11Buffer* Model::pConstantBuffer1 = NULL;
ID3D11SamplerState* Model::pSampleLinear = NULL;

bool Model::InitStaticMesh() {
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
	//���X�^���C�U�̃Z�b�g
	Direct3D::getDeviceContext()->RSSetState(pRasterizerState);

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
	//�u�����h�X�e�[�g���R���e�L�X�g�ɐݒ�
	Direct3D::getDeviceContext()->OMSetBlendState(pBlendState, NULL, 0xffffffff);

	//�[�x�X�e���V���X�e�[�g�̐ݒ�
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//�[�x�X�e���V���X�e�[�g�̍쐬
	Direct3D::getDevice()->CreateDepthStencilState(&dsDesc, &pDepthStencilState);
	//�[�x�X�e���V���X�e�[�g���R���e�L�X�g�ɐݒ�
	Direct3D::getDeviceContext()->OMSetDepthStencilState(pDepthStencilState, 0);

	//���_�C���v�b�g���C�A�E�g���쐬
	Shader::VertexShader* vs = Shader::getVertexShader(Shader::eVertexShader::VS_3D);
	UINT numElements = sizeof(hInElementDesc_Model) / sizeof(hInElementDesc_Model[0]);
	if (FAILED(Direct3D::getDevice()->CreateInputLayout(hInElementDesc_Model, numElements, vs->getCode(), vs->getLength(), &pVertexLayout)))
	{
		return FALSE;
	}

	//�R���X�^���g�o�b�t�@�[�쐬(�ϊ��s��p)
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ConstantBuffer0);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(Direct3D::getDevice()->CreateBuffer(&cb, NULL, &pConstantBuffer0)))
	{
		return false;
	}
	//�R���X�^���g�o�b�t�@�[�쐬(�}�e���A���p)
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ConstantBuffer1);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(Direct3D::getDevice()->CreateBuffer(&cb, NULL, &pConstantBuffer1)))
	{
		return false;
	}

	// �T���v���X�e�[�g�̐ݒ�
	D3D11_SAMPLER_DESC smpDesc;
	memset(&smpDesc, 0, sizeof(smpDesc));
	smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	smpDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	// �T���v���X�e�[�g�̐���
	Direct3D::getDevice()->CreateSamplerState(&smpDesc, &pSampleLinear);

	return true;
}

void Model::DestroyStaticMesh() {
	SAFE_RELEASE(pSampleLinear);
	SAFE_RELEASE(pConstantBuffer0);
	SAFE_RELEASE(pConstantBuffer1);
	SAFE_RELEASE(pVertexLayout);
	SAFE_RELEASE(pDepthStencilState);
	SAFE_RELEASE(pBlendState);
	SAFE_RELEASE(pRasterizerState);
}

Model::Model(const WCHAR* fileName) {
	pVertexBuffer = NULL;
	ppIndexBuffer = NULL;

	//���W�ǂݎ��p
	float _x, _y, _z;
	int _v1 = 0, _v2 = 0, _v3 = 0;
	int _vn1 = 0, _vn2 = 0, _vn3 = 0;
	int _vt1 = 0, _vt2 = 0, _vt3 = 0;

	DWORD _dwVertexNum = 0; //���_���i�[
	DWORD _dwFaceNum = 0; //�ʂ̐��i�[

	DWORD _dwVCount = 0;//�ǂݍ��݃J�E���^�[
	DWORD _dwVNCount = 0;//�ǂݍ��݃J�E���^�[
	DWORD _dwVTCount = 0;//�ǂݍ��݃J�E���^�[
	DWORD _dwFCount = 0;//�ǂݍ��݃J�E���^�[

	char _key[256] = { 0 };
	//�t�@�C�����J���ē��e��ǂݍ���
	FILE* fp = NULL;
	_wfopen_s(&fp, fileName, L"rt");
	if (fp == NULL) return;

	//�܂��͒��_���A�|���S�����𒲂ׂ�
	while (!feof(fp))
	{
		//�L�[���[�h�ǂݍ���
		fscanf_s(fp, "%s ", _key, (int)sizeof(_key));
		//�}�e���A���ǂݍ���
		if (strcmp(_key, "mtllib") == 0)
		{
			fscanf_s(fp, "%s ", _key, (int)sizeof(_key));
			wchar_t wKey[256];
			mbstowcs_s(NULL, wKey, _key, (int)sizeof(_key));
			LoadMaterial((LPWSTR)wKey, &pMaterial);
		}
		//���_
		if (strcmp(_key, "v") == 0)
		{
			_dwVertexNum++;
		}
		//�@��
		if (strcmp(_key, "vn") == 0)
		{
			_dwVNCount++;
		}
		//�e�N�X�`���[���W
		if (strcmp(_key, "vt") == 0)
		{
			_dwVTCount++;
		}
		//�t�F�C�X�i�|���S���j
		if (strcmp(_key, "f") == 0)
		{
			_dwFaceNum++;
		}
	}
	//�ꎞ�I�ȃ������m�ہi���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�j
	int _indexNum = _dwFaceNum * 3;
	stVertex3D* _pVertexBuffer = new stVertex3D[_indexNum];
	XMFLOAT3* _pCoord = new XMFLOAT3[_dwVertexNum];
	for (int i = 0; i < sizeof(*_pCoord) / sizeof(_pCoord[0]); i++) _pCoord[i] = { 0,0,0 };
	XMFLOAT3* _pNormal = new XMFLOAT3[_dwVNCount];
	for (int i = 0; i < sizeof(*_pNormal) / sizeof(_pNormal[0]); i++) _pNormal[i] = { 0,0,0 };
	XMFLOAT2* _pTexture = new XMFLOAT2[_dwVTCount];
	for (int i = 0; i < sizeof(*_pTexture) / sizeof(_pTexture[0]); i++) _pTexture[i] = { 0,0 };

	int _norNum = (int)_dwVNCount;
	int _texNum = (int)_dwVTCount;

	//�{�ǂݍ���	
	fseek(fp,SEEK_SET,0);
	_dwVCount = 0;
	_dwVNCount = 0;
	_dwVTCount = 0;
	_dwFCount = 0;
	
	while(!feof(fp))
	{
		//�L�[���[�h �ǂݍ���
		ZeroMemory(_key,sizeof(_key));
		fscanf_s(fp,"%s ",_key, (int)sizeof(_key));
		
		//���_ �ǂݍ���
		if(strcmp(_key,"v")==0)
		{		
			fscanf_s(fp,"%f %f %f",&_x,&_y,&_z);
			if ((int)_dwVCount >= _dwVertexNum) continue;
			_pCoord[_dwVCount].x=-_x;//OBJ�͉E����W�n�Ȃ̂�x���邢�͂��𔽓]
			_pCoord[_dwVCount].y=_y;
			_pCoord[_dwVCount].z=_z;
			_dwVCount++;
		}
		//�@���ǂݍ���
		if (strcmp(_key, "vn") == 0)
		{
			fscanf_s(fp, "%f %f %f", &_x, &_y, &_z);
			if ((int)_dwVNCount >= _norNum) continue;
			_pNormal[_dwVNCount].x = -_x;//OBJ�͉E����W�n�Ȃ̂�x���邢�͂��𔽓]
			_pNormal[_dwVNCount].y = _y;
			_pNormal[_dwVNCount].z = _z;
			_dwVNCount++;
		}
		//�e�N�X�`���[���W �ǂݍ���
		if (strcmp(_key, "vt") == 0)
		{
			fscanf_s(fp, "%f %f", &_x, &_y);
			if ((int)_dwVTCount >= _texNum) continue;
			_pTexture[_dwVTCount].x = _x;
			_pTexture[_dwVTCount].y = 1 - _y;//OBJ�t�@�C����Y�������t�Ȃ̂ō��킹��
			_dwVTCount++;
		}
	}

	//�}�e���A���̐������C���f�b�N�X�o�b�t�@�[���쐬
	ppIndexBuffer = new ID3D11Buffer * [materialNum];

	//�t�F�C�X�ǂݍ��� �o���o���Ɏ��^����Ă���\��������̂ŁA�}�e���A�����𗊂�ɂȂ����킹��
	bool _flag = false;
	int* _pFaceBuffer = new int[_indexNum];//�R���_�|���S���Ȃ̂ŁA1�t�F�C�X=3���_(3�C���f�b�N�X)
	_dwFCount = 0;
	DWORD _dwPartFCount = 0;
	for (int i = 0; i < materialNum; i++)
	{
		_dwPartFCount = 0;
		fseek(fp, SEEK_SET, 0);

		while (!feof(fp))
		{
			//�L�[���[�h �ǂݍ���
			ZeroMemory(_key, sizeof(_key));
			fscanf_s(fp, "%s ", _key, (int)sizeof(_key));

			//�t�F�C�X �ǂݍ��݁����_�C���f�b�N�X��
			if (strcmp(_key, "usemtl") == 0)
			{
				fscanf_s(fp, "%s ", _key, (int)sizeof(_key));
				if (strcmp(_key, pMaterial[i].name) == 0)
				{
					_flag = true;
				}
				else
				{
					_flag = false;
				}
			}
			if (strcmp(_key, "f") == 0 && _flag == true)
			{
				if (pMaterial[i].pTexture != NULL)//�e�N�X�`���[����T�[�t�F�C�X
				{
					fscanf_s(fp, "%d/%d/%d %d/%d/%d %d/%d/%d", &_v1, &_vt1, &_vn1, &_v2, &_vt2, &_vn2, &_v3, &_vt3, &_vn3);
				}
				else//�e�N�X�`���[�����T�[�t�F�C�X
				{
					fscanf_s(fp, "%d//%d %d//%d %d//%d", &_v1, &_vn1, &_v2, &_vn2, &_v3, &_vn3);
				}

				//�C���f�b�N�X�o�b�t�@�[
				_pFaceBuffer[_dwPartFCount * 3] = _dwFCount * 3;
				_pFaceBuffer[_dwPartFCount * 3 + 1] = _dwFCount * 3 + 1;
				_pFaceBuffer[_dwPartFCount * 3 + 2] = _dwFCount * 3 + 2;
				//���_�\���̂ɑ��
				_pVertexBuffer[_dwFCount * 3].pos = _pCoord[_v1 - 1];
				_pVertexBuffer[_dwFCount * 3].nor = _pNormal[_vn1 - 1];
				_pVertexBuffer[_dwFCount * 3].tex = _pTexture[_vt1 - 1];
				_pVertexBuffer[_dwFCount * 3 + 1].pos = _pCoord[_v2 - 1];
				_pVertexBuffer[_dwFCount * 3 + 1].nor = _pNormal[_vn2 - 1];
				_pVertexBuffer[_dwFCount * 3 + 1].tex = _pTexture[_vt2 - 1];
				_pVertexBuffer[_dwFCount * 3 + 2].pos = _pCoord[_v3 - 1];
				_pVertexBuffer[_dwFCount * 3 + 2].nor = _pNormal[_vn3 - 1];
				_pVertexBuffer[_dwFCount * 3 + 2].tex = _pTexture[_vt3 - 1];

				_dwPartFCount++;
				_dwFCount++;

			}
		}
		if (_dwPartFCount == 0)//�g�p����Ă��Ȃ��}�e���A���΍�
		{
			ppIndexBuffer[i] = NULL;
			continue;
		}

		//�C���f�b�N�X�o�b�t�@�[���쐬
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * _dwPartFCount * 3;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = _pFaceBuffer;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		Direct3D::getDevice()->CreateBuffer(&bd, &InitData, &ppIndexBuffer[i]);

		pMaterial[i].faceNum = _dwPartFCount; //�}�e���A���̖ʂ̐����Z�b�g
	}
	delete[] _pFaceBuffer;

	fclose(fp);

	//�o�[�e�b�N�X�o�b�t�@�[�쐬
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(stVertex3D) * _indexNum;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = _pVertexBuffer;
	Direct3D::getDevice()->CreateBuffer(&bd, &InitData, &pVertexBuffer);

	//�ꎞ�I�ȓ��ꕨ��delete
	delete[] _pCoord;
	delete[] _pNormal;
	if(_dwVTCount != 0) delete[] _pTexture;
	delete[] _pVertexBuffer;
}

Model::~Model() {
	delete[] pMaterial;
	SAFE_RELEASE(pVertexBuffer);
	delete[] ppIndexBuffer;
}

void Model::Render() {
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProj;
	
	mWorld = XMMatrixIdentity();
	mView = XMMatrixIdentity();
	mProj = XMMatrixIdentity();

	// <summary>
	//�ړ��E��]�E�X�P�[�����O�̐ݒ�
	// </summary>
	//�X�P�[�����O
	XMMATRIX mScale = GetScaleMatrix();
	mWorld = mWorld * mScale;//���[���h�s��֔��f
	//��]
	XMMATRIX mRot = GetRotMatrix();
	mWorld = mWorld * mRot;
	//�ړ�
	XMMATRIX mPos = GetPosMatrix();
	mWorld = mWorld * mPos; 

	// �r���[�g�����X�t�H�[���i���_���W�ϊ��j
	XMFLOAT3 _EyePt(0.0f, 0.0f, -0.5f); //�J�����i���_�j�ʒu
	XMFLOAT3 _LookatPt(0.0f, 0.0f, 0.0f);//�����ʒu
	XMFLOAT3 _UpVec(0.0f, 1.0f, 0.0f);//����ʒu
	mView = XMMatrixLookAtLH(ToXMVECTOR(_EyePt), ToXMVECTOR(_LookatPt), ToXMVECTOR(_UpVec));
	// �v���W�F�N�V�����g�����X�t�H�[���i�ˉe�ϊ��j
	mProj = XMMatrixPerspectiveFovLH(XM_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 110.0f);


	//�g�p����V�F�[�_�[�̓o�^	
	Direct3D::getDeviceContext()->VSSetShader(Shader::getVertexShader(Shader::eVertexShader::VS_3D)->getShader(), NULL, 0);
	Direct3D::getDeviceContext()->PSSetShader(Shader::getPixelShader(Shader::ePixelShader::PS_3D)->getShader(), NULL, 0);
	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(Direct3D::getDeviceContext()->Map(pConstantBuffer0, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		ConstantBuffer0 cb;
		//���[���h�s���n��
		cb.mW = mWorld;
		cb.mW = XMMatrixTranspose(cb.mW);
		//���[���h�A�J�����A�ˉe�s���n��
		XMMATRIX m = mWorld * mView * mProj;
		cb.mWVP = m;
		cb.mWVP = XMMatrixTranspose(cb.mWVP);
		//���C�g�̕�����n��
		XMFLOAT3 vLightDir(0, 0, 0);
		cb.lightDir = XMFLOAT4(vLightDir.x, vLightDir.y, vLightDir.z, 0.0f);
		//���_�ʒu��n��
		cb.eye = XMFLOAT4(_EyePt.x, _EyePt.y, _EyePt.z, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(ConstantBuffer0));
		Direct3D::getDeviceContext()->Unmap(pConstantBuffer0, 0);
	}
	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	Direct3D::getDeviceContext()->VSSetConstantBuffers(0, 1, &pConstantBuffer0);
	Direct3D::getDeviceContext()->PSSetConstantBuffers(0, 1, &pConstantBuffer0);
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	Direct3D::getDeviceContext()->IASetInputLayout(pVertexLayout);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	Direct3D::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(stVertex3D);
	UINT offset = 0;
	Direct3D::getDeviceContext()->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	//�}�e���A���̐������A���ꂼ��̃}�e���A���̃C���f�b�N�X�o�b�t�@�|��`��
	for (int i = 0; i < materialNum; i++)
	{
		//�g�p����Ă��Ȃ��}�e���A���΍�
		if (pMaterial[i].faceNum == 0)
		{
			continue;
		}
		//�C���f�b�N�X�o�b�t�@�[���Z�b�g
		stride = sizeof(int);
		offset = 0;
		Direct3D::getDeviceContext()->IASetIndexBuffer(ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);
		//�}�e���A���̊e�v�f���G�t�F�N�g�i�V�F�[�_�[�j�ɓn��
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(Direct3D::getDeviceContext()->Map(pConstantBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			ConstantBuffer1 cb;
			cb.ambient = pMaterial[i].ambient;//�A���r�G���g�����V�F�[�_�[�ɓn��
			cb.diffuse = pMaterial[i].diffuse;//�f�B�t���[�Y�J���[���V�F�[�_�[�ɓn��
			cb.specular = pMaterial[i].specular;//�X�y�L�����[���V�F�[�_�[�ɓn��
			memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(ConstantBuffer1));
			Direct3D::getDeviceContext()->Unmap(pConstantBuffer1, 0);
		}
		Direct3D::getDeviceContext()->VSSetConstantBuffers(1, 1, &pConstantBuffer1);
		Direct3D::getDeviceContext()->PSSetConstantBuffers(1, 1, &pConstantBuffer1);
		
		//�e�N�X�`���[���V�F�[�_�[�ɓn��
		if (pMaterial[i].textureName[0] != NULL)
		{
			Direct3D::getDeviceContext()->PSSetSamplers(0, 1, &pSampleLinear);
			Direct3D::getDeviceContext()->PSSetShaderResources(0, 1, &pMaterial[i].pTexture);
		}
		//�v���~�e�B�u�������_�����O
		Direct3D::getDeviceContext()->DrawIndexed(pMaterial[i].faceNum * 3, 0, 0);
	}
}

bool Model::LoadMaterial(LPWSTR fileName, stMaterial** ppMaterial) {
	//�}�e���A���t�@�C�����J���ē��e��ǂݍ���
	FILE* fp = NULL;
	wchar_t pass[256] = L"Data/Object/";
	wcscat_s(pass, fileName);

	_wfopen_s(&fp, pass, L"rt");
	if (fp == NULL) return false;
	char key[110] = { 0 };
	XMFLOAT4 v(0, 0, 0, 0);

	//�}�e���A�����𒲂ׂ�
	while (!feof(fp))
	{
		//�L�[���[�h�ǂݍ���
		fscanf_s(fp, "%s ", key, (int)sizeof(key));
		//�}�e���A����
		if (strcmp(key, "newmtl") == 0)
		{
			materialNum++;
		}
	}
	stMaterial* pMaterial = new stMaterial[materialNum];

	//�ǂݍ���
	fseek(fp, SEEK_SET, 0);
	int iMCount = -1;

	while (!feof(fp))
	{
		//�L�[���[�h�ǂݍ���
		fscanf_s(fp, "%s ", key, (int)sizeof(key));
		//�}�e���A����
		if (strcmp(key, "newmtl") == 0)
		{
			iMCount++;
			if (iMCount >= materialNum) break;
			fscanf_s(fp, "%s ", key, (int)sizeof(key));
			strcpy_s(pMaterial[iMCount].name, key);
		}
		//Ka�@�A���r�G���g
		if (strcmp(key, "Ka") == 0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			pMaterial[iMCount].ambient = v;
		}
		//Kd�@�f�B�t���[�Y
		if (strcmp(key, "Kd") == 0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			pMaterial[iMCount].diffuse = v;
		}
		//Ks�@�X�y�L�����[
		if (strcmp(key, "Ks") == 0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			pMaterial[iMCount].specular = v;
		}
		//map_Kd�@�e�N�X�`���[
		if (strcmp(key, "map_Kd") == 0)
		{
			fscanf_s(fp, "%s", &pMaterial[iMCount].textureName, (int)sizeof(pMaterial[iMCount].textureName));
			wchar_t wTextureName[256];
			mbstowcs_s(NULL, wTextureName, pMaterial[iMCount].textureName, (int)sizeof(pMaterial[iMCount].textureName));
			wchar_t iPass[256] = L"Data/Image/";
			wcscat_s(iPass, wTextureName);
			//�e�N�X�`���[�쐬
			TexMetadata imagedata;
			auto image = std::make_unique<ScratchImage>();
			if (FAILED(LoadFromWICFile(iPass, WIC_FLAGS_NONE, &imagedata, *image)))
			{
				return false;
			}
			if (FAILED(CreateShaderResourceView(Direct3D::getDevice(), image->GetImages(), image->GetImageCount(), imagedata, &pMaterial[iMCount].pTexture)))
			{
				// ���s
				return false;
			}
		}
	}
	fclose(fp);

	*ppMaterial = pMaterial;

	return true;
}

XMMATRIX Model::GetPosMatrix() {
	//���݂̍��W�𒸓_���W�ɃZ�b�g
	return XMMatrixTranslation(-posX, posY, posZ);
}

XMMATRIX Model::GetRotMatrix() {
	XMMATRIX mPitch, mHeading, mBank;//��]�s��p
	mPitch = XMMatrixRotationX(-rotX);
	mHeading = XMMatrixRotationY(rotY);
	mBank = XMMatrixRotationZ(rotZ);
	return mPitch * mHeading * mBank;
}

XMMATRIX Model::GetScaleMatrix() {
	return XMMatrixScaling(scaleX, scaleY, scaleZ);
}