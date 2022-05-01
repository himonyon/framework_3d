#include "../../../environment.h"
#include "../../../DirectXTex/DirectXTex.h"

using namespace MyFrameWork;

noDel_ptr<Model> ModelCreator::CreateModel(const char* fileName, noDel_ptr<MaterialManager> matManager) {
	//マテリアルマネージャーセット
	this->matManager = matManager;

	//拡張子取得
	const char* _ext = strchr(fileName, '.');
	//種類別にメッシュを作成
	if (_ext) {
		bool createEnable = false;
		if (strcmp(_ext, ".obj") == 0) createEnable = CreateObjMeshes(fileName);
		else if (strcmp(_ext, ".fbx") == 0) createEnable = CreateFbxMeshes(fileName);

		if (createEnable == false) return NULL;
	}
	else return NULL;

	//生成したメッシュ群でモデルを作成
	noDel_ptr<Model> _model = noDel_ptr<Model>(new Model(vMeshList));
	return _model;
}
//Fbx
bool ModelCreator::CreateFbxMeshes(const char* fileName) {
	//FbxManagerの作成
	FbxManager* pFbxManager = FbxManager::Create();
	if (pFbxManager == nullptr) {
		return false;
	}
	//FbxImporterの作成
	FbxImporter* pFbxImporter = FbxImporter::Create(pFbxManager, "Importer");
	if (pFbxImporter == nullptr) {
		pFbxManager->Destroy();
		return false;
	}
	//FbxSceneの作成
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

	//初期化
	pFbxImporter->Initialize(fileName);
	// sceneにインポート
	pFbxImporter->Import(pFbxScene);

	FbxGeometryConverter converter(pFbxManager);
	// メッシュに使われているマテリアル単位でメッシュを分割する
	converter.SplitMeshesPerMaterial(pFbxScene, true);
	// ポリゴンを三角形にする
	converter.Triangulate(pFbxScene, true);

	FbxSystemUnit SceneSystemUnit = pFbxScene->GetGlobalSettings().GetSystemUnit();
		// シーンの単位をcmに設定する
		FbxSystemUnit::m.ConvertScene(pFbxScene);

	//マテリアル作成
	int material_num = pFbxScene->GetSrcObjectCount<FbxSurfaceMaterial>();
	for (int i = 0; i < material_num; i++)
	{
		LoadMaterial(pFbxScene->GetSrcObject<FbxSurfaceMaterial>(i));
	}

	// FbxMeshの数を取得
	int mesh_num = pFbxScene->GetSrcObjectCount<FbxMesh>();
	// Mesh作成
	std::unordered_map<FbxMesh*, noDel_ptr<Mesh>> _umMeshes;
	for (int i = 0; i < mesh_num; i++)
	{
		FbxMesh* _fbxMesh = pFbxScene->GetSrcObject<FbxMesh>(i);
		FbxMeshFile* _mesh = new FbxMeshFile(_fbxMesh);
		noDel_ptr<Mesh>_noDelMesh = noDel_ptr<Mesh>(_mesh);
		_umMeshes[_fbxMesh] = _noDelMesh;
	}

	//マテリアルセット
	for (int i = 0; i < _umMeshes.size(); i++)
	{
		FbxMesh* _fbxMesh = pFbxScene->GetSrcObject<FbxMesh>(i);
		SetMaterial(_umMeshes[_fbxMesh], _fbxMesh);
	}

	//親の設定
	ParentSetting(_umMeshes);

	//メッシュリストに格納
	for (auto& mesh : _umMeshes) {
		vMeshList.emplace_back(mesh.second);
	}

	//破棄
	pFbxImporter->Destroy();
	pFbxScene->Destroy();
	pFbxManager->Destroy();

	//一番親のメッシュを返す
	return true;
}
void ModelCreator::LoadMaterial(FbxSurfaceMaterial* material)
{
	//マテリアルマネージャーにある場合は生成しない
	if (matManager->GetMaterial(material->GetName()) != NULL) return;

	//マテリアル生成
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

	// テクスチャ読み込み(シングル対応)
	// Diffuseプロパティを取得
	prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
	FbxFileTexture* texture = nullptr;
	std::string keyword;
	int texture_num = prop.GetSrcObjectCount<FbxFileTexture>();
	if (texture_num > 0)
	{
		// propからFbxFileTextureを取得	
		texture = prop.GetSrcObject<FbxFileTexture>(0);
	}
	else
	{
		// FbxLayeredTextureからFbxFileTextureを取得	
		int layer_num = prop.GetSrcObjectCount<FbxLayeredTexture>();
		if (layer_num > 0)
		{
			texture = prop.GetSrcObject<FbxFileTexture>(0);
		}
	}

	if (texture != nullptr &&
		LoadTexture(texture, keyword) == true)
	{
		// 読み込んだテクスチャとマテリアルの関係を覚えておく
		_entryMaterial->pTexture = umTextures[keyword];
	}

	//マテリアル格納
	_entryMaterial->materialName = material->GetName();
	matManager->AddMaterial(_entryMaterial->materialName,_entryMaterial);
}
bool ModelCreator::LoadTexture(FbxFileTexture* texture, std::string& keyword)
{
	if (texture == nullptr)
	{
		return false;
	}
	// ファイル名を取得
	std::string _file_path = texture->GetRelativeFileName();

	// ファイル分解
	char buffer[256];
	ZeroMemory(buffer, sizeof(char) * 256);
	memcpy(buffer, _file_path.c_str(), sizeof(char) * 256);

	// 記号統一
	Replace('\\', '/', buffer);
	std::vector<std::string> split_list;
	std::string replace_file_name = buffer;
	// 「/」で分解
	Split('/', buffer, split_list);

	std::string root_path = "Data/Image/Fbx/";
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
	std::wstring wstr_file_name = cv.from_bytes(root_path + split_list[split_list.size() - 1]);

	// 文字化け対策
	char* file_name;
	size_t size = 0;
	FbxUTF8ToAnsi(split_list[split_list.size() - 1].c_str(), file_name, &size);

	//同じテクスチャがあればパスを指定して終了
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

		// 画像からシェーダリソースViewの作成
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
		//テクスチャー作成
		TexMetadata imagedata;
		auto image = std::make_unique<ScratchImage>();
		if (FAILED(LoadFromWICFile(wstr_file_name.c_str(), WIC_FLAGS_NONE, &imagedata, *image)))
		{
			return false;
		}
		if (FAILED(CreateShaderResourceView(Direct3D::GetDevice(), image->GetImages(), image->GetImageCount(), imagedata, &umTextures[file_name])))
		{
			// 失敗
			return false;
		}
	}


	keyword = file_name;
	FbxFree(file_name);
	return true;
}
void ModelCreator::SetMaterial(noDel_ptr<Mesh> pMeshFile, FbxMesh* pFbxMesh)
{
	// マテリアルが無ければデフォルトのマテリアルを設定
	if (pFbxMesh->GetElementMaterialCount() == 0)
	{
		noDel_ptr<stMaterial> _defMat = matManager->GetMaterial("default");
		pMeshFile->SetMaterial(_defMat);
		return;
	}

	// Mesh側のマテリアル情報を取得
	FbxLayerElementMaterial* _material = pFbxMesh->GetElementMaterial(0);
	int index = _material->GetIndexArray().GetAt(0);
	FbxSurfaceMaterial* _surface_material = pFbxMesh->GetNode()->GetSrcObject<FbxSurfaceMaterial>(index);

	//マテリアルセット
	noDel_ptr<stMaterial> _newMat = matManager->GetMaterial(_surface_material->GetName());
	if (_newMat != NULL) {
		pMeshFile->SetMaterial(_newMat);
	}

	//NULLの場合デフォルトマテリアルを設定
	if (pMeshFile->GetMaterial() == NULL) {
		noDel_ptr<stMaterial> _defMat = matManager->GetMaterial("default");
		pMeshFile->SetMaterial(_defMat);
	}
}
void ModelCreator::ParentSetting(std::unordered_map<FbxMesh*, noDel_ptr<Mesh>>& umMeshes) {
	for (auto& setMesh : umMeshes) {
		FbxNode* _node = setMesh.first->GetNode();
		FbxMesh* _parentMesh = _node->GetParent()->GetMesh();

		//親のFBXMESHが同じ場合生成したメッシュの親を設定
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
	std::vector<std::string> _outMaterialList; //マテリアルリスト
	std::unordered_map<std::string, ObjMesh*> _umObjs; //生成するオブジェクトリスト

	//ファイルパス取得
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

	//Objメッシュ作成
	if (LoadObjMeshes(_umObjs, _outMaterialList, fileName) == false) {
		return false;
	}
	//バッファ読み込み
	for (auto& obj : _umObjs) {
		if(obj.second->CreateVertexBuffer() == false) {
			return false;
		}
		if (obj.second->CreateIndexBuffer() == false) {
			return false;
		}
	}
	//マテリアル読み込み
	if (LoadMaterialFile(_outMaterialList, _filePath) == false) {
		return false;
	}
	//マテリアルセット
	for (auto& obj : _umObjs) {
		noDel_ptr<stMaterial> _setMat = matManager->GetMaterial(obj.first);
		if(_setMat != NULL) obj.second->SetMaterial(_setMat);
	}

	//メッシュ一覧へ格納
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

	std::vector<stVector3> _vertices; //モデルの頂点座標
	std::vector<stVector3> _textures;//モデルのテクスチャ座標
	std::vector<stVector3> _normals;//モデルの法線座標
	std::unordered_map<std::string, int> _umIndexList;
	std::string _curMatName = "";
	const int _lineBufferLength = 1024;
	char _buffer[_lineBufferLength];

	while (fgets(_buffer, _lineBufferLength, fp) != nullptr)
	{
		// コメントは無視
		if (_buffer[0] == '#')
		{
			continue;
		}

		char* _parsePoint = strchr(_buffer, ' ');
		if (_parsePoint == nullptr)
		{
			continue;
		}

		//改行は終末文字に変換
		Replace('\n', '\0', _buffer);

		// 頂点関連
		if (_buffer[0] == 'v')
		{
			// 頂点座標
			if (_buffer[1] == ' ')
			{
				ParseVKeywordTag(_vertices, &_parsePoint[1]);
			}
			// テクスチャ座標
			else if (_buffer[1] == 't')
			{
				ParseVKeywordTag(_textures, &_parsePoint[1]);
				// V軸を反転させる
				_textures[_textures.size() - 1].x = (1.0f - _textures[_textures.size() - 1].x);
				_textures[_textures.size() - 1].y = (1.0f - _textures[_textures.size() - 1].y);
			}
			// 法線座標
			else if (_buffer[1] == 'n')
			{
				ParseVKeywordTag(_normals, &_parsePoint[1]);
			}
		}
		// 面情報
		else if (_buffer[0] == 'f')
		{
			objs[_curMatName]->ParseFKeywordTag(_umIndexList, _curMatName, _vertices, _textures, _normals, &_parsePoint[1]);
		}
		else if (strstr(_buffer, "mtllib") == _buffer)
		{
			Replace('\n', '\0', _buffer);
			// 使用しているmtlファイルとして追加
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
				//名前取得
				_current_material_name = &buffer[strlen("newmtl") + 1];
				//マテリアル生成＆マテリアルマネージャーに格納
				_newMat = NULL; //初期化
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
				// ファイルパス込みで保存
				_newMat->textureName = file_path + texture_name;

				std::vector<std::string> split;
				Split('.', (char*)texture_name.c_str(), split);
				if (split.size() > 1)
				{
					// 読み込み
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

	// ファイル分解
	char buffer[256];
	ZeroMemory(buffer, sizeof(char) * 256);
	memcpy(buffer, file_name.c_str(), sizeof(char) * 256);

	// 記号統一
	Replace('\\', '/', buffer);
	std::vector<std::string> split_list;
	std::string replace_file_name = buffer;
	// 「/」で分解
	Split('/', buffer, split_list);

	std::string root_path = "Data/Image/Obj/";
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
	std::wstring wstr_file_name = cv.from_bytes(root_path + split_list[split_list.size() - 1]);

	//テクスチャー作成
	TexMetadata imagedata;
	auto image = std::make_unique<ScratchImage>();
	if (FAILED(LoadFromWICFile(wstr_file_name.c_str(), WIC_FLAGS_NONE, &imagedata, *image)))
	{
		return false;
	}
	if (FAILED(CreateShaderResourceView(Direct3D::GetDevice(), image->GetImages(), image->GetImageCount(), imagedata, &umTextures[key_word])))
	{
		// 失敗
		return false;
	}

	return true;
}