#pragma once
/*-----------------------------------------------------------

	MeshRendererコンポーネント
		メッシュを描画する

-------------------------------------------------------------*/

namespace MyFrameWork {
	class MeshRenderer : public Renderer3D {
	public:
		//描画対象メッシュ
		noDel_ptr<Mesh> pRenderMesh = NULL;

	private:
		//カラー
		stColor4 col; 

	public:
		MeshRenderer();
		~MeshRenderer(void);

		//コンポーネントの初期化
		void SetUpMeshRenderer(noDel_ptr<Mesh> mesh);

		//コンポーネント処理
		void Execute() override;

	private:
		//描画
		void Render() override;

	};
}