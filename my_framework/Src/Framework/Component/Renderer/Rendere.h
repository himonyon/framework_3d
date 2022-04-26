#pragma once
/*-----------------------------------------------------------

	Rendererクラス
		2D描画の基底クラス

-------------------------------------------------------------*/

namespace MyFrameWork {
	class Renderer : public Component {
	protected:
		stColor4 color;

	public:
		Renderer(eComponentType type);
		virtual ~Renderer(void);

		virtual void Render(void) = 0;

		//色の設定
		virtual void SetColor(float r, float g, float b, float a) {};
		virtual void SetColor(stColor4 color) {};
		virtual stColor4 GetColor() { return { 0,0,0,0 }; };

	protected:
		//座標、回転、スケールの描画座標(カメラ適用)
		stVector3 GetPosOnCam();
		stVector3 GetRotOnCam();
		stVector3 GetScaleOnCam();
	};
}