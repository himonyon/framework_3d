#include "../../../environment.h"
#include "../../../DirectXTex/DirectXTex.h"

#include <iomanip>

#if _DEBUG
#pragma comment(lib,"DirectXTex/Bin/Debug/DirectXTex.lib")
#else
#pragma comment(lib,"DirectXTex/Bin/Release/DirectXTex.lib")
#endif

using namespace MyFrameWork;

ObjMesh::ObjMesh(const char* fileName) {
	char _filePath[256];
	std::vector<std::string> _outMaterialList;
	int _len = (int)strlen(fileName);
	int _pathTailPoint = 0;

	for (int i = _len - 1; i >= 0; i--)
	{
		if (fileName[i] == '/')
		{
			_pathTailPoint = i;
			break;
		}
	}

	int _cpyLen = _pathTailPoint + 1;
	strncpy_s(_filePath, fileName, _cpyLen);

	if (CreateMesh(_outMaterialList, fileName) == false)
	{
		return;
	}

	if (LoadMaterialFile(_outMaterialList, _filePath) == false)
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

ObjMesh::~ObjMesh() {
}

void ObjMesh::Render(stVector3 pos, stVector3 rot, stVector3 scl) {
	int count = 0;
	UINT strides = sizeof(stVertex3D);
	UINT offsets = 0;

	// �C���f�b�N�X�o�b�t�@�̐� = �}�e���A���̐��������b�V����`�悷��
	for (auto index : umIndices)
	{
		// IA(InputAssemblerStage)�ɓ��̓��C�A�E�g��ݒ肷��
		Direct3D::getDeviceContext()->IASetInputLayout(Renderer3D::GetInputLayout());
		// IA�ɐݒ肷�钸�_�o�b�t�@�̎w��
		Direct3D::getDeviceContext()->IASetVertexBuffers(
			0,						// �o�b�t�@���M�̃X���b�g�ԍ�
			1,						// �o�b�t�@�̐�
			&pVertexBuffer,		// ���_�o�b�t�@
			&strides,				// �o�b�t�@�Ɏg�p���Ă���\���̂̃T�C�Y
			&offsets);				// �J�n�I�t�Z�b�g

		Direct3D::getDeviceContext()->IASetIndexBuffer(
			vIndexBuffers[count],
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

		stCBuffer3D _inputCB = Renderer3D::GetInputCB();

		// ���[���h�}�g���N�X���R���X�^���g�o�b�t�@�ɐݒ�
		XMStoreFloat4x4(&_inputCB.world, XMMatrixTranspose(mWorld));

		SetMaterial(umMaterials[index.first]);
		ID3D11Buffer* _constantBuffer = Renderer3D::GetConstantBuffer();
		// �R���X�^���g�o�b�t�@�X�V
		Direct3D::getDeviceContext()->UpdateSubresource(_constantBuffer, 0, NULL, &_inputCB, 0, 0);

		// �R���e�L�X�g�ɃR���X�^���g�o�b�t�@��ݒ�
		Direct3D::getDeviceContext()->VSSetConstantBuffers(0, 1, &_constantBuffer);
		Direct3D::getDeviceContext()->PSSetConstantBuffers(0, 1, &_constantBuffer);

		// �}�e���A���Ƀe�N�X�`��������Ȃ�ݒ肷��
		if (umTextures.count(umMaterials[index.first].TextureKeyWord) > 0)
		{
			Direct3D::getDeviceContext()->PSSetShader(Shader::getPixelShader(Shader::ePixelShader::PS_3D)->getShader(), nullptr, 0);
			SetTexture(umTextures[umMaterials[index.first].TextureKeyWord]);
		}
		else
		{
			Direct3D::getDeviceContext()->PSSetShader(Shader::getPixelShader(Shader::ePixelShader::PS_3D_NOTEX)->getShader(), nullptr, 0);
		}

		// �`��
		Direct3D::getDeviceContext()->DrawIndexed(
			(UINT)index.second.size(),		// ���_��
			0,						// �I�t�Z�b�g
			0);						// �J�n���_�̃C���f�b�N�X

		count++;
	}
}

bool ObjMesh::CreateMesh(std::vector<std::string>& outMaterialList, const char* fileName) {
	FILE* fp = nullptr;
	fopen_s(&fp, fileName, "r");

	if (fp == nullptr)
	{
		return false;
	}

	std::vector<stVector3> _vertices;
	std::vector<stVector3> _textures;
	std::vector<stVector3> _normals;
	std::unordered_map<std::string, int> _umIndexList;
	std::string _CurMatName = "";
	const int _lineBufferLength = 1024;
	char _buffer[_lineBufferLength];

	while (fgets(_buffer, _lineBufferLength, fp) != nullptr)
	{
		// �R�����g�͖���
		if (_buffer[0] == '#')
		{
			continue;
		}

		char* _parsePoint = strchr(_buffer, ' ');
		if (_parsePoint == nullptr)
		{
			continue;
		}

		//���s�͏I�������ɕϊ�
		Replace('\n', '\0', _buffer);

		// ���_�֘A
		if (_buffer[0] == 'v')
		{
			// ���_���W
			if (_buffer[1] == ' ')
			{
				ParseVKeywordTag(_vertices, &_parsePoint[1]);
			}
			// �e�N�X�`�����W
			else if (_buffer[1] == 't')
			{
				ParseVKeywordTag(_textures, &_parsePoint[1]);
				// V���𔽓]������
				_textures[_textures.size() - 1].x = (1.0f - _textures[_textures.size() - 1].x);
				_textures[_textures.size() - 1].y = (1.0f - _textures[_textures.size() - 1].y);
			}
			// �@�����W
			else if (_buffer[1] == 'n')
			{
				ParseVKeywordTag(_normals, &_parsePoint[1]);
			}
		}
		// �ʏ��
		else if (_buffer[0] == 'f')
		{
			ParseFKeywordTag(vVertices, _umIndexList, _CurMatName, _vertices, _textures, _normals, &_parsePoint[1]);
		}
		else if (strstr(_buffer, "mtllib") == _buffer)
		{
			Replace('\n', '\0', _buffer);
			// �g�p���Ă���mtl�t�@�C���Ƃ��Ēǉ�
			outMaterialList.push_back(&_buffer[strlen("mtllib") + 1]);
		}
		else if (strstr(_buffer, "usemtl") == _buffer)
		{
			Replace('\n', '\0', _buffer);
			_CurMatName = &_buffer[strlen("usemtl") + 1];
		}
	}

	fclose(fp);

	return true;
}

bool ObjMesh::CreateVertexBuffer() {
	//���_�o�b�t�@�쐬
	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.ByteWidth = sizeof(stVertex3D) * (UINT)vVertices.size();	// �o�b�t�@�̃T�C�Y
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;			// �g�p���@
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// BIND�ݒ�
	buffer_desc.CPUAccessFlags = 0;						// ���\�[�X�ւ�CPU�̃A�N�Z�X�����ɂ��Ă̐ݒ�
	buffer_desc.MiscFlags = 0;							// ���\�[�X�I�v�V�����̃t���O
	buffer_desc.StructureByteStride = 0;				// �\���̂̃T�C�Y

	D3D11_SUBRESOURCE_DATA sub_resource;
	sub_resource.pSysMem = &vVertices[0];	// �o�b�t�@�̒��g�̐ݒ�
	sub_resource.SysMemPitch = 0;			// texture�f�[�^���g�p����ۂɎg�p���郁���o
	sub_resource.SysMemSlicePitch = 0;		// texture�f�[�^���g�p����ۂɎg�p���郁���o

	// �o�b�t�@�쐬
	if (FAILED(Direct3D::getDevice()->CreateBuffer(
		&buffer_desc,				// �o�b�t�@���
		&sub_resource,				// ���\�[�X���
		&pVertexBuffer)))			// �쐬���ꂽ�o�b�t�@�̊i�[��
	{
		return false;
	}

	return true;
}

bool ObjMesh::CreateIndexBuffer() {
	int _count = 0;
	for (auto index : umIndices)
	{
		vIndexBuffers.emplace_back();

		//���_�o�b�t�@�쐬
		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.ByteWidth = sizeof(UINT) * (UINT)index.second.size();	// �o�b�t�@�̃T�C�Y
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;							// �g�p���@
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;					// BIND�ݒ�
		buffer_desc.CPUAccessFlags = 0;										// ���\�[�X�ւ�CPU�̃A�N�Z�X�����ɂ��Ă̐ݒ�
		buffer_desc.MiscFlags = 0;											// ���\�[�X�I�v�V�����̃t���O
		buffer_desc.StructureByteStride = 0;								// �\���̂̃T�C�Y

		D3D11_SUBRESOURCE_DATA sub_resource;
		sub_resource.pSysMem = &index.second[0];							// �o�b�t�@�̒��g�̐ݒ�
		sub_resource.SysMemPitch = 0;										// texture�f�[�^���g�p����ۂɎg�p���郁���o
		sub_resource.SysMemSlicePitch = 0;									// texture�f�[�^���g�p����ۂɎg�p���郁���o

		// �o�b�t�@�쐬
		if (FAILED(Direct3D::getDevice()->CreateBuffer(
			&buffer_desc,					// �o�b�t�@���
			&sub_resource,					// ���\�[�X���
			&vIndexBuffers[_count])))		// �쐬���ꂽ�o�b�t�@�̊i�[��
		{
			return false;
		}

		_count++;
	}

	return true;
}

void ObjMesh::ParseVKeywordTag(std::vector<stVector3>& outVertices, char* buff) {
	std::vector<std::string> _splitStrs;
	Split(' ', buff, _splitStrs);

	int count = 0;
	float values[3] = { 0.0f };

	for (std::string str : _splitStrs)
	{
		values[count] = (float)atof(str.c_str());
		count++;
	}

	outVertices.push_back(stVector3{ values[0], values[1], values[2] });
}
void ObjMesh::ParseFKeywordTag(std::vector<stVertex3D>& outVertices, std::unordered_map<std::string, int>& index_list,
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
			umIndices[curMaterialName].push_back(index_list[key]);
		}
		else
		{
			// ���_���ǉ�
			outVertices.push_back(vertex);

			// ���_�o�b�t�@�̗v�f�ԍ��ǉ�
			umIndices[curMaterialName].push_back((UINT)outVertices.size() - 1);

			// ���_�o�b�t�@�̗v�f�ԍ��ǉ�
			index_list[key] = (int)outVertices.size() - 1;
		}

		add_index_count++;
	}

	UINT size = (UINT)umIndices[curMaterialName].size();
	UINT temp = 0;

	// �uf�v�L�[���[�h�̕������ʂ�3�𒴂����ꍇ�͒��_��ǉ�����
	if (space_split.size() > 3)
	{
		umIndices[curMaterialName].push_back(umIndices[curMaterialName][size - 4]);
		umIndices[curMaterialName].push_back(umIndices[curMaterialName][size - 2]);
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

bool ObjMesh::LoadMaterialFile(std::vector<std::string> file_list, std::string file_path) {
	char buffer[1024];

	for (auto mat_file_name : file_list)
	{
		FILE* fp = nullptr;
		std::string name = file_path;
		name += mat_file_name;

		fopen_s(&fp, name.c_str(), "r");

		if (fp == nullptr)
		{
			return false;
		}
		std::string current_material_name = "";

		while (fgets(buffer, 1024, fp) != nullptr)
		{
			if (strstr(buffer, "newmtl") == buffer)
			{
				Replace('\n', '\0', buffer);
				current_material_name = &buffer[strlen("newmtl") + 1];
			}
			else if (strstr(buffer, "map_Kd") == buffer)
			{
				Replace('\n', '\0', buffer);
				std::string texture_name = &buffer[strlen("map_Kd") + 1];
				// �t�@�C���p�X���݂ŕۑ�
				umMaterials[current_material_name].TextureName = file_path + texture_name;

				std::vector<std::string> split;
				Split('.', (char*)texture_name.c_str(), split);
				if (split.size() > 1)
				{
					umMaterials[current_material_name].TextureKeyWord = split[0];
					// �ǂݍ���
					LoadTexture(split[0], umMaterials[current_material_name].TextureName);
				}
			}
			else if (strstr(buffer, "Ka") == buffer)
			{
				Replace('\n', '\0', buffer);
				std::vector<std::string> split;
				Split(' ', &buffer[strlen("Ka") + 1], split);

				for (int i = 0; i < split.size(); i++)
				{
					umMaterials[current_material_name].Ambient[i] = (float)atof(split[i].c_str());
				}
			}
			else if (strstr(buffer, "Kd") == buffer)
			{
				Replace('\n', '\0', buffer);
				std::vector<std::string> split;
				Split(' ', &buffer[strlen("Kd") + 1], split);

				for (int i = 0; i < split.size(); i++)
				{
					umMaterials[current_material_name].Diffuse[i] = (float)atof(split[i].c_str());
				}
			}
			else if (strstr(buffer, "Ks") == buffer)
			{
				Replace('\n', '\0', buffer);
				std::vector<std::string> split;
				Split(' ', &buffer[strlen("Ks") + 1], split);

				for (int i = 0; i < split.size(); i++)
				{
					umMaterials[current_material_name].Specular[i] = (float)atof(split[i].c_str());
				}
			}
		}

		fclose(fp);
	}

	return true;
}
bool ObjMesh::LoadTexture(std::string key_word, std::string file_name) {
	if (umTextures.count(key_word) > 0 &&
		umTextures[key_word] != nullptr)
	{
		return true;
	}

	// �t�@�C������
	char buffer[256];
	ZeroMemory(buffer, sizeof(char) * 256);
	memcpy(buffer, file_name.c_str(), sizeof(char) * 256);

	// �L������
	Replace('\\', '/', buffer);
	std::vector<std::string> split_list;
	std::string replace_file_name = buffer;
	// �u/�v�ŕ���
	Split('/', buffer, split_list);

	std::string root_path = "Data/Image/Obj/";
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
	std::wstring wstr_file_name = cv.from_bytes(root_path + split_list[split_list.size() - 1]);

	//�e�N�X�`���[�쐬
	TexMetadata imagedata;
	auto image = std::make_unique<ScratchImage>();
	if (FAILED(LoadFromWICFile(wstr_file_name.c_str(), WIC_FLAGS_NONE, &imagedata, *image)))
	{
		return false;
	}
	if (FAILED(CreateShaderResourceView(Direct3D::getDevice(), image->GetImages(), image->GetImageCount(), imagedata, &umTextures[key_word])))
	{
		// ���s
		return false;
	}

	return true;
}

void ObjMesh::SetMaterial(stMaterial material) {
	stCBuffer3D cb = Renderer3D::GetInputCB();
	cb.ambient = XMFLOAT4(material.Ambient[0], material.Ambient[1], material.Ambient[2], 1);
	cb.diffuse = XMFLOAT4(material.Diffuse[0], material.Diffuse[1], material.Diffuse[2], 1);
	cb.specular = XMFLOAT4(material.Specular[0], material.Specular[1], material.Specular[2], 1);
}
void ObjMesh::SetTexture(ID3D11ShaderResourceView* texture) {
	// Sampler�̐ݒ�
	ID3D11SamplerState* _sampleState = Renderer3D::GetSampleLinear();
	Direct3D::getDeviceContext()->PSSetSamplers(0,	1, &_sampleState);

	// PixelShader�Ŏg�p����e�N�X�`���̐ݒ�
	Direct3D::getDeviceContext()->PSSetShaderResources(0,1,	&texture);
}