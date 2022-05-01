#include "../../../environment.h"
#include "../../../DirectXTex/DirectXTex.h"

using namespace MyFrameWork;

noDel_ptr<Model> ModelCreator::CreateModel(const char* fileName, noDel_ptr<MaterialManager> matManager) {
	//�}�e���A���}�l�[�W���[�Z�b�g
	this->matManager = matManager;

	//�g���q�擾
	const char* _ext = strchr(fileName, '.');
	//��ޕʂɃ��b�V�����쐬
	if (_ext) {
		bool createEnable = false;
		if (strcmp(_ext, ".obj") == 0) createEnable = CreateObjMeshes(fileName);
		else if (strcmp(_ext, ".fbx") == 0) createEnable = CreateFbxMeshes(fileName);

		if (createEnable == false) return NULL;
	}
	else return NULL;

	//�����������b�V���Q�Ń��f�����쐬
	noDel_ptr<Model> _model = noDel_ptr<Model>(new Model(vMeshList));
	return _model;
}
//Fbx
bool ModelCreator::CreateFbxMeshes(const char* fileName) {
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

	//pFbxScene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::eMayaYUp);

	FbxAxisSystem lSceneAxisSystem = pFbxScene->GetGlobalSettings().GetAxisSystem();
	FbxAxisSystem lTargetAxisSystem(FbxAxisSystem::DirectX);
	if (lSceneAxisSystem != lTargetAxisSystem)
		lTargetAxisSystem.ConvertScene(pFbxScene);

	//������
	pFbxImporter->Initialize(fileName);
	// scene�ɃC���|�[�g
	pFbxImporter->Import(pFbxScene);

	FbxGeometryConverter converter(pFbxManager);
	// ���b�V���Ɏg���Ă���}�e���A���P�ʂŃ��b�V���𕪊�����
	converter.SplitMeshesPerMaterial(pFbxScene, true);
	// �|���S�����O�p�`�ɂ���
	converter.Triangulate(pFbxScene, true);

	FbxSystemUnit SceneSystemUnit = pFbxScene->GetGlobalSettings().GetSystemUnit();
		// �V�[���̒P�ʂ�cm�ɐݒ肷��
		FbxSystemUnit::m.ConvertScene(pFbxScene);

	//�}�e���A���쐬
	int material_num = pFbxScene->GetSrcObjectCount<FbxSurfaceMaterial>();
	for (int i = 0; i < material_num; i++)
	{
		LoadMaterial(pFbxScene->GetSrcObject<FbxSurfaceMaterial>(i));
	}

	// FbxMesh�̐����擾
	int mesh_num = pFbxScene->GetSrcObjectCount<FbxMesh>();
	// Mesh�쐬
	std::unordered_map<FbxMesh*, noDel_ptr<Mesh>> _umMeshes;
	for (int i = 0; i < mesh_num; i++)
	{
		FbxMesh* _fbxMesh = pFbxScene->GetSrcObject<FbxMesh>(i);
		FbxMeshFile* _mesh = new FbxMeshFile(_fbxMesh);
		noDel_ptr<Mesh>_noDelMesh = noDel_ptr<Mesh>(_mesh);
		_umMeshes[_fbxMesh] = _noDelMesh;
	}

	//�}�e���A���Z�b�g
	for (int i = 0; i < _umMeshes.size(); i++)
	{
		FbxMesh* _fbxMesh = pFbxScene->GetSrcObject<FbxMesh>(i);
		SetMaterial(_umMeshes[_fbxMesh], _fbxMesh);
	}

	//�e�̐ݒ�
	ParentSetting(_umMeshes);

	//���b�V�����X�g�Ɋi�[
	for (auto& mesh : _umMeshes) {
		vMeshList.emplace_back(mesh.second);
	}

	//�j��
	pFbxImporter->Destroy();
	pFbxScene->Destroy();
	pFbxManager->Destroy();

	//��Ԑe�̃��b�V����Ԃ�
	return true;
}
void ModelCreator::LoadMaterial(FbxSurfaceMaterial* material)
{
	//�}�e���A���}�l�[�W���[�ɂ���ꍇ�͐������Ȃ�
	if (matManager->GetMaterial(material->GetName()) != NULL) return;

	//�}�e���A������
	stMaterial* _entryMaterial = new stMaterial;
	enum class eMaterialOrder
	{
		Ambient,
		Diffuse,
		Specular,
		MaxOrder,
	};

	FbxDouble3 colors[(int)eMaterialOrder::MaxOrder] = {};
	FbxDouble factors[(int)eMaterialOrder::MaxOrder] = {};
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
	_entryMaterial->SetAmbient((float)color[0], (float)color[1], (float)color[2], (float)factor);

	color = colors[(int)eMaterialOrder::Diffuse];
	factor = factors[(int)eMaterialOrder::Diffuse];
	_entryMaterial->SetDiffuse((float)color[0], (float)color[1], (float)color[2], (float)factor);

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
		_entryMaterial->pTexture = umTextures[keyword];
	}

	//�}�e���A���i�[
	_entryMaterial->materialName = material->GetName();
	matManager->AddMaterial(_entryMaterial->materialName,_entryMaterial);
}
bool ModelCreator::LoadTexture(FbxFileTexture* texture, std::string& keyword)
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

	//�����e�N�X�`��������΃p�X���w�肵�ďI��
	if (umTextures.count(file_name) > 0 &&
		umTextures[file_name] != nullptr)
	{
		keyword = file_name;
		FbxFree(file_name);
		return true;
	}

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
			Direct3D::GetDevice(),
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
		if (FAILED(CreateShaderResourceView(Direct3D::GetDevice(), image->GetImages(), image->GetImageCount(), imagedata, &umTextures[file_name])))
		{
			// ���s
			return false;
		}
	}


	keyword = file_name;
	FbxFree(file_name);
	return true;
}
void ModelCreator::SetMaterial(noDel_ptr<Mesh> pMeshFile, FbxMesh* pFbxMesh)
{
	// �}�e���A����������΃f�t�H���g�̃}�e���A����ݒ�
	if (pFbxMesh->GetElementMaterialCount() == 0)
	{
		noDel_ptr<stMaterial> _defMat = matManager->GetMaterial("default");
		pMeshFile->SetMaterial(_defMat);
		return;
	}

	// Mesh���̃}�e���A�������擾
	FbxLayerElementMaterial* _material = pFbxMesh->GetElementMaterial(0);
	int index = _material->GetIndexArray().GetAt(0);
	FbxSurfaceMaterial* _surface_material = pFbxMesh->GetNode()->GetSrcObject<FbxSurfaceMaterial>(index);

	//�}�e���A���Z�b�g
	noDel_ptr<stMaterial> _newMat = matManager->GetMaterial(_surface_material->GetName());
	if (_newMat != NULL) {
		pMeshFile->SetMaterial(_newMat);
	}

	//NULL�̏ꍇ�f�t�H���g�}�e���A����ݒ�
	if (pMeshFile->GetMaterial() == NULL) {
		noDel_ptr<stMaterial> _defMat = matManager->GetMaterial("default");
		pMeshFile->SetMaterial(_defMat);
	}
}
void ModelCreator::ParentSetting(std::unordered_map<FbxMesh*, noDel_ptr<Mesh>>& umMeshes) {
	for (auto& setMesh : umMeshes) {
		FbxNode* _node = setMesh.first->GetNode();
		FbxMesh* _parentMesh = _node->GetParent()->GetMesh();

		//�e��FBXMESH�������ꍇ�����������b�V���̐e��ݒ�
		for (auto& searchMesh : umMeshes) {
			if (_parentMesh == searchMesh.first) {
				setMesh.second->SetParent(searchMesh.second);
				break;
			}
		}
	}
}



//Obj
bool ModelCreator::CreateObjMeshes(const char* fileName) {
	std::vector<std::string> _outMaterialList; //�}�e���A�����X�g
	std::unordered_map<std::string, ObjMesh*> _umObjs; //��������I�u�W�F�N�g���X�g

	//�t�@�C���p�X�擾
	char _filePath[256];
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

	//Obj���b�V���쐬
	if (LoadObjMeshes(_umObjs, _outMaterialList, fileName) == false) {
		return false;
	}
	//�o�b�t�@�ǂݍ���
	for (auto& obj : _umObjs) {
		if(obj.second->CreateVertexBuffer() == false) {
			return false;
		}
		if (obj.second->CreateIndexBuffer() == false) {
			return false;
		}
	}
	//�}�e���A���ǂݍ���
	if (LoadMaterialFile(_outMaterialList, _filePath) == false) {
		return false;
	}
	//�}�e���A���Z�b�g
	for (auto& obj : _umObjs) {
		noDel_ptr<stMaterial> _setMat = matManager->GetMaterial(obj.first);
		if(_setMat != NULL) obj.second->SetMaterial(_setMat);
	}

	//���b�V���ꗗ�֊i�[
	for (auto& obj : _umObjs) {
		noDel_ptr<Mesh>_noDelMesh = noDel_ptr<Mesh>(obj.second);
		vMeshList.emplace_back(_noDelMesh);
	}

	return true;
}
bool ModelCreator::LoadObjMeshes(std::unordered_map<std::string, ObjMesh*>& objs, std::vector<std::string>& outMaterialList, const char* fileName) {
	FILE* fp = nullptr;
	fopen_s(&fp, fileName, "r");

	if (fp == nullptr)
	{
		return false;
	}

	std::vector<stVector3> _vertices; //���f���̒��_���W
	std::vector<stVector3> _textures;//���f���̃e�N�X�`�����W
	std::vector<stVector3> _normals;//���f���̖@�����W
	std::unordered_map<std::string, int> _umIndexList;
	std::string _curMatName = "";
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
			objs[_curMatName]->ParseFKeywordTag(_umIndexList, _curMatName, _vertices, _textures, _normals, &_parsePoint[1]);
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
			_curMatName = &_buffer[strlen("usemtl") + 1];
			if (objs.find(_curMatName) == objs.end()) {
				objs[_curMatName] = new ObjMesh();
			}
		}
	}

	fclose(fp);

	return true;
}
void ModelCreator::ParseVKeywordTag(std::vector<stVector3>& outVertices, char* buff) {
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
bool ModelCreator::LoadMaterialFile(std::vector<std::string> file_list, std::string file_path) {
	char buffer[1024];
	for (auto mat_file_name : file_list)
	{
		FILE* fp = nullptr;
		stMaterial* _newMat = NULL;
		std::string name = file_path;
		name += mat_file_name;

		fopen_s(&fp, name.c_str(), "r");

		if (fp == nullptr)
		{
			return false;
		}
		std::string _current_material_name = "";

		while (fgets(buffer, 1024, fp) != nullptr)
		{
			if (strstr(buffer, "newmtl") == buffer)
			{
				Replace('\n', '\0', buffer);
				//���O�擾
				_current_material_name = &buffer[strlen("newmtl") + 1];
				//�}�e���A���������}�e���A���}�l�[�W���[�Ɋi�[
				_newMat = NULL; //������
				if (matManager->GetMaterial(_current_material_name) == NULL) {
					_newMat = new stMaterial;
					_newMat->materialName = _current_material_name;
					matManager->AddMaterial(_newMat->materialName, _newMat);
				}
				
			}
			else if (strstr(buffer, "map_Kd") == buffer)
			{
				if (_newMat == NULL) continue;
				Replace('\n', '\0', buffer);
				std::string texture_name = &buffer[strlen("map_Kd") + 1];
				// �t�@�C���p�X���݂ŕۑ�
				_newMat->textureName = file_path + texture_name;

				std::vector<std::string> split;
				Split('.', (char*)texture_name.c_str(), split);
				if (split.size() > 1)
				{
					// �ǂݍ���
					if (LoadTexture(split[0], _newMat->textureName)) {
						_newMat->pTexture = umTextures[split[0]];
					}

				}
			}
			else if (strstr(buffer, "Ka") == buffer)
			{
				if (_newMat == NULL) continue;
				Replace('\n', '\0', buffer);
				std::vector<std::string> split;
				Split(' ', &buffer[strlen("Ka") + 1], split);

				float _col[4] = { 0,0,0,0 };
				for (int i = 0; i < split.size(); i++)
				{
					if (i < 4) _col[i] = (float)atof(split[i].c_str());
				}
				_newMat->ambient.SetColor(_col[0], _col[1], _col[2], _col[3]);
			}
			else if (strstr(buffer, "Kd") == buffer)
			{
				if (_newMat == NULL) continue;
				Replace('\n', '\0', buffer);
				std::vector<std::string> split;
				Split(' ', &buffer[strlen("Kd") + 1], split);

				float _col[4] = { 0,0,0,0 };
				for (int i = 0; i < split.size(); i++)
				{
					if (i < 4) _col[i] = (float)atof(split[i].c_str());
				}
				_newMat->diffuse.SetColor(_col[0], _col[1], _col[2], _col[3]);
			}
			else if (strstr(buffer, "Ks") == buffer)
			{
				if (_newMat == NULL) continue;
				Replace('\n', '\0', buffer);
				std::vector<std::string> split;
				Split(' ', &buffer[strlen("Ks") + 1], split);

				float _col[4] = { 0,0,0,0 };
				for (int i = 0; i < split.size(); i++)
				{
					if (i < 4) _col[i] = (float)atof(split[i].c_str());
				}
				_newMat->specular.SetColor(_col[0], _col[1], _col[2], _col[3]);
			}
		}

		fclose(fp);
	}

	return true;
}
bool ModelCreator::LoadTexture(std::string key_word, std::string file_name) {
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
	if (FAILED(CreateShaderResourceView(Direct3D::GetDevice(), image->GetImages(), image->GetImageCount(), imagedata, &umTextures[key_word])))
	{
		// ���s
		return false;
	}

	return true;
}