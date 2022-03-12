/*-----------------------------------------------------------

	ジョイスティッククラス
		ジョイスティックの入力を受け付ける

-------------------------------------------------------------*/
#pragma once

namespace MyFrameWork {

	//ジョイスティック
	//キーボードの数字と被らないため大きめに設定
#define JOY_SQUARE (300)
#define JOY_CROSS (301)
#define JOY_CIRCLE (302)
#define JOY_TRIANGLAR (303)
#define JOY_L (304)
#define JOY_R (305)
#define JOY_L2 (306)
#define JOY_R2 (307)
#define JOY_SHARE (308)
#define JOY_OPTION (309)
#define JOY_LSTICKPUSH (310)
#define JOY_RSTICKPUSH (311)
#define JOY_PS (312)
#define JOY_PAD (313)

//スティック
#define JOY_RSTICKR (350)
#define JOY_RSTICKL (351)
#define JOY_RSTICKU (352)
#define JOY_RSTICKD (353)
#define JOY_LSTICKR (354)
#define JOY_LSTICKL (355)
#define JOY_LSTICKU (356)
#define JOY_LSTICKD (357)
#define JOY_LSTICK (358)
#define JOY_RSTICK (359)

//十字キー
#define JOY_POVU (10000)
#define JOY_POVR (19000)
#define JOY_POVD (28000)
#define JOY_POVL (37000)

#define MOVE_SPEED 1 //ジョイスティック幅


	class Joystick {
	public:
		static float stickEnableRange; //ジョイスティックのブレの値を除外する範囲
		static float stickFlagRange; //ジョイスティックのボタンとしての判定範囲
	public:
		//ジョイスティック(0~)
		static bool IsValid();
		static bool On(int button);
		static bool Trg(int button);
		static bool Rel(int button);
		static float GetLX();
		static float GetLY();
		static float GetRX();
		static float GetRY();
		static bool PovOn(int pov);
		static bool PovTrg(int pov);
		static bool PovRel(int pov);
		static bool StickTrg(int dir);
		static bool StickOn(int dir);
		static bool StickRel(int dir);
	};
}
