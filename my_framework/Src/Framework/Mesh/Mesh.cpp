#include "../../environment.h"
#include <DirectXTex.h>
#pragma comment(lib, "DirectXTex.lib")




Mesh::Mesh(const WCHAR* fileName) {
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

Mesh::~Mesh() {
	delete[] pMaterial;
	SAFE_RELEASE(pVertexBuffer);
	delete[] ppIndexBuffer;
}

bool Mesh::LoadMaterial(LPWSTR fileName, stMaterial** ppMaterial) {
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

