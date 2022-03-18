/*-----------------------------------------------------------

	Animatorコンポーネント
		アニメーション管理

-------------------------------------------------------------*/

#pragma once

namespace MyFrameWork {
	class Animator : public Component {
	public:
		//アニメーション格納クラス
		class AnimBox {
		public:
			AnimBox(std::string name, SpriteAnimation* anim);
			~AnimBox();

			bool trigger = false; //アニメーショントリガー
		public:
			std::string name;
			SpriteAnimation* anim;
		};

		//コンストラクタ
		Animator();
		//デストラクタ
		~Animator();

		//コンポーネント処理
		void Execute() override;

		//アニメーション一覧へアニメーションを加える
		void AddAnimation(std::string name, SpriteAnimation* anim);

		//アニメーション開始
		void PlayAnim(std::string name);
		//アニメーション実行中か
		bool IsPlayAnim(std::string name);
		//アニメーションストップ（全体）
		void StopAnim();
		//アニメーションストップ
		void StopAnim(std::string name);

	private:
		//アニメーション一覧
		std::vector<AnimBox*> animBoxes;
	};
}