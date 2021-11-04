#include "../../environment.h"


_NODISCARD static inline auto ToXMVECTOR(const DirectX::XMFLOAT3& vec)
{
	return DirectX::XMLoadFloat3(&vec);
}

//���_�C���v�b�g���C�A�E�g���`	
D3D11_INPUT_ELEMENT_DESC hInElementDesc_Model[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

ID3D11InputLayout* Model:: pVertexLayout = NULL;
ID3D11RasterizerState* Model::pRasterizerState = NULL;
ID3D11Buffer* Model::pConstantBuffer = NULL;

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

	//���_�C���v�b�g���C�A�E�g���쐬
	Shader::VertexShader* vs = Shader::getVertexShader(Shader::eVertexShader::VS_3D);
	UINT numElements = sizeof(hInElementDesc_Model) / sizeof(hInElementDesc_Model[0]);
	if (FAILED(Direct3D::getDevice()->CreateInputLayout(hInElementDesc_Model, numElements, vs->getCode(), vs->getLength(), &pVertexLayout)))
	{
		return FALSE;
	}

	//�R���X�^���g�o�b�t�@�[�쐬
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ConstantBuffer);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(Direct3D::getDevice()->CreateBuffer(&cb, NULL, &pConstantBuffer)))
	{
		return false;
	}

	return true;
}

void Model::DestroyStaticMesh() {
	SAFE_RELEASE(pConstantBuffer);
	SAFE_RELEASE(pVertexLayout);
	SAFE_RELEASE(pRasterizerState);
}

Model::Model(const WCHAR* fileName) {
	pVertexBuffer = NULL;
	pIndexBuffer = NULL;

	//���W�ǂݎ��p
	float x, y, z;
	int v1 = 0, v2 = 0, v3 = 0;
	int vertexNum = 0;
	char key[190] = { 0 };
	//�t�@�C�����J���ē��e��ǂݍ���
	FILE* fp = NULL;
	_wfopen_s(&fp, fileName, L"rt");
	if (fp == NULL) return;

	//�܂��͒��_���A�|���S�����𒲂ׂ�
	while (!feof(fp))
	{
		//�L�[���[�h�ǂݍ���
		fscanf_s(fp, "%s ", key, (int)sizeof(key));
		//���_
		if (strcmp(key, "v") == 0)
		{
			vertexNum++;
		}
		//�t�F�C�X�i�|���S���j
		if (strcmp(key, "f") == 0)
		{
			faceNum++;
		}
	}
	//�ꎞ�I�ȃ������m�ہi���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�j
	XMFLOAT3* pvVertexBuffer = new XMFLOAT3[vertexNum];
	int* piFaceBuffer = new int[faceNum * 3];//�R���_�|���S���Ȃ̂ŁA1�t�F�C�X=3���_(3�C���f�b�N�X)

	//�{�ǂݍ���	
	fseek(fp,SEEK_SET,0);
	DWORD dwVCount=0;//�ǂݍ��݃J�E���^�[
	DWORD dwFCount=0;//�ǂݍ��݃J�E���^�[
	while(!feof(fp))
	{
		//�L�[���[�h �ǂݍ���
		ZeroMemory(key,sizeof(key));
		fscanf_s(fp,"%s ",key, (int)sizeof(key));
		//���_ �ǂݍ���
		if(strcmp(key,"v")==0)
		{		
			fscanf_s(fp,"%f %f %f",&x,&y,&z);
			pvVertexBuffer[dwVCount].x=-x;//OBJ�͉E����W�n�Ȃ̂�x���邢�͂��𔽓]
			pvVertexBuffer[dwVCount].y=y;
			pvVertexBuffer[dwVCount].z=z;
			dwVCount++;
		}
		//�t�F�C�X�i�|���S���j �ǂݍ���
		if(strcmp(key,"f")==0)
		{
			fscanf_s(fp,"%d// %d// %d//",&v1,&v2,&v3);
			piFaceBuffer[dwFCount*3]=v1-1;
			piFaceBuffer[dwFCount*3+1]=v2-1;
			piFaceBuffer[dwFCount*3+2]=v3-1;
			dwFCount++;
		}
		
	}

	fclose(fp);

	//�o�[�e�b�N�X�o�b�t�@�[�쐬
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(XMFLOAT3) * vertexNum * 3;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pvVertexBuffer;
	if (FAILED(Direct3D::getDevice()->CreateBuffer(&bd, &InitData, &pVertexBuffer))) {
		return;
	}

	//�C���f�b�N�X�o�b�t�@�[���쐬
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int) * faceNum * 3;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	InitData.pSysMem = piFaceBuffer;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	if (FAILED(Direct3D::getDevice()->CreateBuffer(&bd, &InitData, &pIndexBuffer))) {
		return;
	}

	//�ꎞ�I�ȓ��ꕨ�́A���͂�s�v
	delete[] pvVertexBuffer;
	delete[] piFaceBuffer;

	return;
}

Model::~Model() {
	SAFE_RELEASE(pVertexBuffer);
	SAFE_RELEASE(pIndexBuffer);
}

void Model::Render() {
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX proj;
	//���[���h�g�����X�t�H�[���i��΍��W�ϊ��j
	world = XMMatrixRotationY(timeGetTime() / 1100.0f);//�P����yaw��]������
	// �r���[�g�����X�t�H�[���i���_���W�ϊ��j
	XMFLOAT3 vEyePt(0.0f, 1.0f, -2.0f); //�J�����i���_�j�ʒu
	XMFLOAT3 vLookatPt(0.0f, 0.0f, 0.0f);//�����ʒu
	XMFLOAT3 vUpVec(0.0f, 1.0f, 0.0f);//����ʒu
	view = XMMatrixLookAtLH(ToXMVECTOR(vEyePt), ToXMVECTOR(vLookatPt), ToXMVECTOR(vUpVec));
	// �v���W�F�N�V�����g�����X�t�H�[���i�ˉe�ϊ��j
	proj = XMMatrixPerspectiveFovLH(XM_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 110.0f);

	//�g�p����V�F�[�_�[�̓o�^	
	Direct3D::getDeviceContext()->VSSetShader(Shader::getVertexShader(Shader::eVertexShader::VS_3D)->getShader(), NULL, 0);
	Direct3D::getDeviceContext()->PSSetShader(Shader::getPixelShader(Shader::ePixelShader::PS_3D)->getShader(), NULL, 0);
	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
	D3D11_MAPPED_SUBRESOURCE pData;
	ConstantBuffer cb;
	if (SUCCEEDED(Direct3D::getDeviceContext()->Map(pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//���[���h�A�J�����A�ˉe�s���n��
		XMMATRIX m = world * view * proj;
		m = XMMatrixTranspose(m);
		cb.WVP = m;

		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(ConstantBuffer));
		Direct3D::getDeviceContext()->Unmap(pConstantBuffer, 0);
	}
	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	Direct3D::getDeviceContext()->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	Direct3D::getDeviceContext()->PSSetConstantBuffers(0, 1, &pConstantBuffer);
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	Direct3D::getDeviceContext()->IASetInputLayout(pVertexLayout);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	Direct3D::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(XMFLOAT3);
	UINT offset = 0;
	Direct3D::getDeviceContext()->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	//�C���f�b�N�X�o�b�t�@�[���Z�b�g
	stride = sizeof(int);
	offset = 0;
	Direct3D::getDeviceContext()->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//�v���~�e�B�u�������_�����O
	Direct3D::getDeviceContext()->DrawIndexed(faceNum * 3, 0, 0);
}