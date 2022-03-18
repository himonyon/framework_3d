#include "../../../../framework.h"
#include "../../../../environment.h"

using namespace MyFrameWork;

MeshRenderer::MeshRenderer()
{
	col = { 1,1,1,1 };
}

MeshRenderer::~MeshRenderer() {}

//コンポーネントの初期化
void MeshRenderer::SetUpMeshRenderer(noDel_ptr<Mesh> mesh) {
	if (mesh != nullptr) pRenderMesh = mesh;
}

void MeshRenderer::Execute() {
	Render();
}

void MeshRenderer::Render() {

	if (pRenderMesh == NULL) return;

	//移動・回転・スケーリングの設定
	stVector3 _drawPos = GetPosOnCam();
	stVector3 _drawRot = GetRotOnCam();
	stVector3 _drawScale = GetScaleOnCam();

	StartRendering();
	inputCB.color = { col.r, col.g, col.b, col.a };

	pRenderMesh->Render(_drawPos, _drawRot, _drawScale);
}
