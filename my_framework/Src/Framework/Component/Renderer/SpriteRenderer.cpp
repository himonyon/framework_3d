#include "../../../../framework.h"
#include "../../../../environment.h"

using namespace MyFrameWork;

float SpriteRenderer::WorldWHPos[4] = { 0,0,0,0 };

XMMATRIX SpriteRenderer::View = {};
XMMATRIX SpriteRenderer::Proj = {};

SpriteRenderer::SpriteRenderer()
{
	for (int i = 0; i < 6; i++) {
		vtx[i].nor = { 0,0,0 };
		vtx[i].col = { 1,1,1,1 };
		vtx[i].pos.z = 0;
	}
}

SpriteRenderer::~SpriteRenderer() {
	SAFE_RELEASE(pVertexBuffer);
}

//コンポーネントの初期化
void SpriteRenderer::SetUpRenderer2D(float sizeX, float sizeY, noDel_ptr<Sprite> sprite) {

	this->sizeX = sizeX;
	this->sizeY = sizeY;

	if (sprite != nullptr) {
		pRenderSprite = sprite;
		SetDefaultUV();
	}

	for (int i = 0; i < 6; i++) {
		vtx[i].nor = { 0,0,0 };
		vtx[i].col = { 1,1,1,1 };
		vtx[i].pos.z = 0;
	}

	vtx[0].pos.x = -0.5f;
	vtx[0].pos.y = 0.5f;
	vtx[1].pos.x = 0.5f;
	vtx[1].pos.y = 0.5f;
	vtx[2].pos.x = -0.5f;
	vtx[2].pos.y = -0.5f;
	vtx[3].pos.x = 0.5f;
	vtx[3].pos.y = -0.5f;

	//バーテックスバッファー作成
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(stVertex3D) * 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &vtx[0];
	InitData.SysMemPitch = 0;		
	InitData.SysMemSlicePitch = 0;	
	Direct3D::getDevice()->CreateBuffer(&bd, &InitData, &pVertexBuffer);
}


void SpriteRenderer::Execute() {
	Render();
}

void SpriteRenderer::SetVertexState() {
	float PI = 3.14f;

	//各頂点の座標を設定
	vtx[0].pos.x = -(sizeX / 2);
	vtx[0].pos.y = (sizeY / 2);

	vtx[1].pos.x = (sizeX / 2);
	vtx[1].pos.y = (sizeY / 2);

	vtx[2].pos.x = -(sizeX / 2);
	vtx[2].pos.y = -(sizeY / 2);
	
	vtx[3].pos.x = (sizeX / 2);
	vtx[3].pos.y = (sizeY / 2);

	vtx[4].pos.x = -(sizeX / 2);
	vtx[4].pos.y = -(sizeY / 2);

	vtx[5].pos.x = (sizeX / 2);
	vtx[5].pos.y = -(sizeY / 2);

	for (auto& v : vtx) {
		v.nor = { 0,0,0 };
		v.col = { 1,1,1,1 };
	}
}

void SpriteRenderer::Render() {
	if (pRenderSprite == NULL) return;

	//移動・回転・スケーリングの設定
	stVector3 _drawPos = GetPosOnCam();
	stVector3 _drawRot = GetRotOnCam();
	stVector3 _drawScale = GetScaleOnCam();

	// ビュートランスフォーム（視点座標変換）
	XMFLOAT3 _EyePt(0.0f, 0.0f, -2.5f); //カメラ（視点）位置
	XMFLOAT3 _LookatPt(0.0f, 0.0f, 0.0f);//注視位置
	XMFLOAT3 _UpVec(0.0f, 1.0f, 0.0f);//上方位置
	XMMATRIX mView = XMMatrixLookAtLH(ToXMVECTOR(_EyePt), ToXMVECTOR(_LookatPt), ToXMVECTOR(_UpVec));
	// プロジェクショントランスフォーム（射影変換）
	XMMATRIX mProj = XMMatrixPerspectiveFovLH(XM_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 110.0f);

	// ライトの設定
	XMVECTOR light = XMVector3Normalize(XMVectorSet(0.0f, 0.5f, -5.0f, 0.0f));

	stCBuffer3D inputCB;

	inputCB.ambient = { 1,1,1,1 };
	inputCB.diffuse = { 1,1,1,1 };
	inputCB.specular = { 1,1,1,1 };
	inputCB.color = { 1,1,1,1 };

	// コンスタントバッファの設定
	XMStoreFloat4x4(&inputCB.view, XMMatrixTranspose(mView));
	XMStoreFloat4x4(&inputCB.projection, XMMatrixTranspose(mProj));
	XMStoreFloat4(&inputCB.lightVector, light);
	XMStoreFloat4(&inputCB.cameraPos, ToXMVECTOR(_EyePt));
	inputCB.lightColor = XMFLOAT4(1, 1, 1, 1);

	// プリミティブの形状を指定
	Direct3D::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ラスタライザのセット
	Direct3D::getDeviceContext()->RSSetState(MeshRenderer::GetRasterizerState());
	//ブレンドステートをコンテキストに設定
	Direct3D::getDeviceContext()->OMSetBlendState(MeshRenderer::GetBlendState(), NULL, 0xffffffff);
	//深度ステンシルステートをコンテキストに設定
	Direct3D::getDeviceContext()->OMSetDepthStencilState(MeshRenderer::GetDepthStencilState(), 0);

	// VerteXShader、PixelShaderを設定
	Direct3D::getDeviceContext()->VSSetShader(Shader::getVertexShader(Shader::eVertexShader::VS_3D)->getShader(), nullptr, 0);									// ClassInstanceの数
	Direct3D::getDeviceContext()->PSSetShader(Shader::getPixelShader(Shader::ePixelShader::PS_3D)->getShader(), nullptr, 0);

	// IA(InputAssemblerStage)に入力レイアウトを設定する
	Direct3D::getDeviceContext()->IASetInputLayout(pInputLayout1);

	//バーテックスバッファーをセット
	UINT stride = sizeof(stVertex3D);
	UINT offset = 0;
	Direct3D::getDeviceContext()->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	Direct3D::getDeviceContext()->IASetIndexBuffer(pIndexBuffer,DXGI_FORMAT_R32_UINT,0);

	// ワールドマトリクス設定
	XMMATRIX world_matrix;
	XMMATRIX translate = XMMatrixTranslation(_drawPos.x, _drawPos.y, _drawPos.z);
	XMMATRIX rotate_x = XMMatrixRotationX(XMConvertToRadians(_drawRot.x));
	XMMATRIX rotate_y = XMMatrixRotationY(XMConvertToRadians(_drawRot.y));
	XMMATRIX rotate_z = XMMatrixRotationZ(XMConvertToRadians(_drawRot.z));
	XMMATRIX scale_mat = XMMatrixScaling(_drawScale.x, _drawScale.y, _drawScale.z);
	world_matrix = scale_mat * rotate_x * rotate_y * rotate_z * translate;

	// ワールドマトリクスをコンスタントバッファに設定
	XMStoreFloat4x4(&inputCB.world, XMMatrixTranspose(world_matrix));

	// コンスタントバッファ更新
	Direct3D::getDeviceContext()->UpdateSubresource(MeshRenderer::GetConstantBuffer(), 0, NULL, &inputCB, 0, 0);
	ID3D11Buffer* _constantBuffer = MeshRenderer::GetConstantBuffer();
	// コンテキストにコンスタントバッファを設定
	Direct3D::getDeviceContext()->VSSetConstantBuffers(0, 1, &_constantBuffer);
	Direct3D::getDeviceContext()->PSSetConstantBuffers(0, 1, &_constantBuffer);

	//テクスチャーをシェーダーに渡す
	if (pRenderSprite->GetTexture() != NULL)
	{
		Direct3D::getDeviceContext()->PSSetSamplers(0, 1, &pSamplerState);
		Direct3D::getDeviceContext()->PSSetShaderResources(0, 1, &pRenderSprite->pTextureView);
	}

	//プリミティブをレンダリング
	Direct3D::getDeviceContext()->Draw(4,  0);
}


void SpriteRenderer::SetColor(float r, float g, float b, float a) {
	color.r = r; color.g = g; color.b = b; color.a = a;
}
void SpriteRenderer::SetColor(stColor4 color) {
	this->color.r = color.r; this->color.g = color.g; this->color.b= color.b; this->color.a = color.a;
}
stColor4 SpriteRenderer::GetColor() {
	return { color.r, color.g, color.b, color.a };
}
void SpriteRenderer::SetDefaultUV() {
	if (pRenderSprite == NULL) return; //スプライトがない場合return
	
	vtx[0].tex.x = pRenderSprite->GetVertexState(0).u;
	vtx[0].tex.y = pRenderSprite->GetVertexState(0).v;
	vtx[1].tex.x = pRenderSprite->GetVertexState(1).u;
	vtx[1].tex.y = pRenderSprite->GetVertexState(1).v;
	vtx[2].tex.x = pRenderSprite->GetVertexState(2).u;
	vtx[2].tex.y = pRenderSprite->GetVertexState(2).v;
	vtx[3].tex.x = pRenderSprite->GetVertexState(3).u;
	vtx[3].tex.y = pRenderSprite->GetVertexState(3).v;
}

stVector3 SpriteRenderer::GetPosOnCam() {
	//現在の座標を頂点座標にセット
	stVector3 pos;
	pos.x = transform->position.x;
	pos.y = transform->position.y;
	pos.z = transform->position.z;
	//カメラ座標を加えてスクリーン座標を設定する
	if (Camera::main != nullptr) {
		pos.x -= Camera::main->transform->position.x;
		pos.y -= Camera::main->transform->position.y;
		pos.z -= Camera::main->transform->position.z;
	}
	return stVector3{ pos.x, pos.y, pos.z };
}
stVector3 SpriteRenderer::GetRotOnCam() {
	stVector3 rot;
	rot.x = transform->rotation.x;
	rot.y = transform->rotation.y;
	rot.z = transform->rotation.z;

	if (Camera::main != nullptr) {
		rot.x -= Camera::main->transform->rotation.x;
		rot.y -= Camera::main->transform->rotation.y;
		rot.z -= Camera::main->transform->rotation.z;
	}
	return stVector3{ rot.x, rot.y, rot.z };
}
stVector3 SpriteRenderer::GetScaleOnCam() {
	stVector3 scl;
	scl.x = transform->scale.x;
	scl.y = transform->scale.y;
	scl.z = transform->scale.z;

	return stVector3{ scl.x, scl.y, scl.z };
}

XMVECTOR SpriteRenderer::CalcScreenToWorld(
	XMVECTOR& pout,
	int Sx,
	int Sy,
	float fZ,
	int Screen_w,
	int Screen_h) {

	float _x = Camera::main != NULL ? Camera::main->transform->position.x : 0;
	float _y = Camera::main != NULL ? Camera::main->transform->position.y : 0;
	float _z = Camera::main != NULL ? Camera::main->transform->position.z : 0;
	if (-0.2f + _z == 0) _z += -0.2f; //All0回避
	XMFLOAT3 tEyePt(0.0f, 0.0f, -0.2f + _z); //カメラ（視点）位置
	XMFLOAT3 tLookatPt(0.0f, 0.0f, 0.0f);//注視位置
	XMFLOAT3 tUpVec(0.0f, 1.0f, 0.0f);//上方位置
	XMMATRIX tView = XMMatrixLookAtLH(ToXMVECTOR(tEyePt), ToXMVECTOR(tLookatPt), ToXMVECTOR(tUpVec));
	// 各行列の逆行列を算出
	XMMATRIX InvView, InvPrj, VP, InvViewport;
	InvView = XMMatrixInverse(NULL, tView);
	InvPrj = XMMatrixInverse(NULL, Proj);
	VP = XMMatrixIdentity();
	VP.r[0].m128_f32[0] = Screen_w / 2.0f; VP.r[1].m128_f32[1] = -Screen_h / 2.0f;
	VP.r[3].m128_f32[0] = Screen_w / 2.0f; VP.r[3].m128_f32[1] = Screen_h / 2.0f;
	InvViewport = XMMatrixInverse(NULL, VP);

	// 逆変換
	XMMATRIX tmp = InvViewport * InvPrj * InvView;
	XMVECTOR temp = XMVECTOR{ (float)Sx,(float)Sy,fZ, 0 };
	pout = XMVector3TransformCoord(temp, tmp);

	return pout;
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
}

stVector3 SpriteRenderer::CalcScreenToXZ(
	int Sx,
	int Sy,
	int Screen_w,
	int Screen_h
) {
	XMVECTOR _pout;

	XMVECTOR nearpos;
	XMVECTOR farpos;
	XMVECTOR ray;
	float tem = (float)abs(Camera::main->transform->position.z + 0.2f) / 110.0f;
	CalcScreenToWorld(nearpos, Sx, Sy, 0.0f, Screen_w, Screen_h);
	CalcScreenToWorld(farpos, Sx, Sy, 1.0f, Screen_w, Screen_h);
	ray = farpos - nearpos;
	ray = XMVector3Normalize(ray);

	// XY平面との交差が起きている場合は交点を
	// 起きていない場合は遠くの壁との交点を出力
	if (ray.m128_f32[2] > 0) {
		// XY平面交点
		XMVECTOR temp = XMVECTOR{ 0,0,1,1 };
		XMVECTOR Lray = XMVector3Dot(ray, temp);
		XMVECTOR LP0 = XMVector3Dot((-nearpos), temp);
		_pout = nearpos + (LP0 / Lray) * ray;
		_pout.m128_f32[0] += Camera::main != NULL ? Camera::main->transform->position.x : 0;
		_pout.m128_f32[1] += Camera::main != NULL ? Camera::main->transform->position.y : 0;
	}
	else {
		_pout = farpos;
	}

	//stVector3に変換
	stVector3 _newVec;
	_newVec.x = _pout.m128_f32[0];
	_newVec.y = _pout.m128_f32[1];
	_newVec.z = _pout.m128_f32[2];

	return _newVec;
}