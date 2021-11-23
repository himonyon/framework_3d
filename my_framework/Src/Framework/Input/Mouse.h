/*-----------------------------------------------------------

	�}�E�X�N���X
		�}�E�X�̓��͂��󂯕t����

-------------------------------------------------------------*/
#pragma once

class Mouse {
public:
	//�}�E�X(L(1) R(4) R(2))
	static bool On(int mouse);
	static bool Trg(int mouse);
	static bool Rel(int mouse);

	static int GetX();
	static int GetY();
	static int GetR();
	static int GetDX();
	static int GetDY();

private:
	static int x;
	static int y;
	static int r;
	static int on;
	static int trg;
	static int rel;

public:
	//�}�E�X�̏�Ԃ�ݒ�
	static void SetMouseStatus(void);

	//�}�E�X�̃|�W�V�������Z�b�g����
	static void SetMousePosition(int _x, int _y);
};