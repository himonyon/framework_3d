#pragma once
/*-----------------------------------------------------------

	Renderer�N���X
		2D�`��̊��N���X

-------------------------------------------------------------*/

namespace MyFrameWork {
	class Renderer : public Component {
	protected:
		stColor4 color;

	public:
		Renderer(eComponentType type);
		virtual ~Renderer(void);

		virtual void Render(void) = 0;

		//�F�̐ݒ�
		virtual void SetColor(float r, float g, float b, float a) {};
		virtual void SetColor(stColor4 color) {};
		virtual stColor4 GetColor() { return { 0,0,0,0 }; };

	protected:
		//���W�A��]�A�X�P�[���̕`����W(�J�����K�p)
		stVector3 GetPosOnCam();
		stVector3 GetRotOnCam();
		stVector3 GetScaleOnCam();
	};
}