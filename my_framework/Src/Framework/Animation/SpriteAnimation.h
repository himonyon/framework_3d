/*-----------------------------------------------------------

	スプライトアニメーションクラス
		キーをフレーム数指定して設置し、各キーの時間になると
		そのキーに設定された処理を行うアニメーション機能

-------------------------------------------------------------*/
#pragma once

namespace MyFrameWork {
	class SpriteAnimation {
	private:
		bool isLoop = false;

		bool isEnd = false;

		float frameCount = 0; //進んだフレーム数

		int curKeyIndex = -1; //現在到達しているキーフレーム配列番号

		noDel_ptr<SpriteState> pAnimRenderer = NULL; //表示するスプライトレンダラー

	private:
		//アニメーションのキーフレームクラス
		class KeyFrame {
		public:
			Sprite* pSprite = 0;
			float frame = 0;
			float x = 0, y = 0;
			stColor4 color = { 1,1,1,1 };
			float scaleX = 1, scaleY = 1;
			float rot = 0;

			//キー設定時に変更が施されたプロパティか確認するためのトリガー
			struct stTrigger {
			public:
				bool scale = false;
				bool rotation = false;
				bool color = false;
			};

			stTrigger trigger;

		public:
			KeyFrame() {};
			~KeyFrame();
		};

	private:
		//キーの配列
		std::vector<KeyFrame*> keyFrames;

		//キー情報のデフォルト値セットアップ
		void SetUpDefaultValue(KeyFrame*);

		//キーフレームのスプライト状態の反映
		void SetKeySpriteState(KeyFrame*);

		//特定のキーフレーム状態に設定(座標以外)
		void SetKeyState(KeyFrame*);

		//次のキーに向けて変化　rateは変化率
		void TransPos(KeyFrame* key);
		void TransCol(KeyFrame* key, float rate);
		void TransScl(KeyFrame* key, float rate);
		void TransRot(KeyFrame* key, float rate);

	public:
		//コンストラクタ：ファイルデータからキー配列を作成
		SpriteAnimation(const WCHAR* animation_file, bool loop = false);
		//デストラクタ
		~SpriteAnimation();

		//アニメーション実行
		void AnimOn();
		//アニメーション終了
		void AnimOff();

		//アニメーションさせる対象レンダラー設定
		void SetAnimRenderer(noDel_ptr<SpriteState>);

		//Getter,Setter
		bool IsLoop() { return isLoop; }
		bool IsEnd() { return isEnd; }
	};
}
