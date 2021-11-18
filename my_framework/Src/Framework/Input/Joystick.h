#pragma once
//�W���C�X�e�B�b�N
#define JOY_SQUARE (0)
#define JOY_CROSS (1)
#define JOY_CIRCLE (2)
#define JOY_TRIANGLAR (3)
#define JOY_L (4)
#define JOY_R (5)
#define JOY_L2 (6)
#define JOY_R2 (7)
#define JOY_SHARE (8)
#define JOY_OPTION (9)
#define JOY_L_STICK (10)
#define JOY_R_STICK (11)
#define JOY_R_PS (12)
#define JOY_PAD (13)

#define MOVE_SPEED 1 //�W���C�X�e�B�b�N��


class Joystick {
public:
	//�W���C�X�e�B�b�N(0~)
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
};
