#pragma once
/*-----------------------------------------------------------

	SpriteStateクラス
		描画する画像の情報クラス

-------------------------------------------------------------*/

namespace MyFrameWork {
	class SpriteState : public Renderer {
	public:
		noDel_ptr<Sprite> pRenderSprite = NULL;

		stVector2 size = { 1,1 };

	public:
		SpriteState(eComponentType type);
		virtual ~SpriteState();

		//サイズ設定
		void SetSize(float, float);

		//スプライトの状態を初期状態に戻す(UV)
		virtual void SetDefaultUV() {};
	};
}