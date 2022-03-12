/*-----------------------------------------------------------

	�t�H���g�N���X
		�t�H���g�̏o��
		Print�֐���ʂ��Ĕz��ɖ��t���[���t�H���g����ݒ�

-------------------------------------------------------------*/
#pragma once

namespace MyFrameWork {

#define FONT_CHARACTER_MAX	(0x200)	//�`��ł��镶���̐�

	enum class eTextAlignment {
		Left,
		Right,
		Center
	};

	class Font : public Renderer2D {
	private:
		//Direct2D�������p�ϐ�
		static ID2D1Factory* pD2d1Factory;
		static IDWriteFactory* pDWFactory;
		static ID2D1RenderTarget* pRenderTarget;

	public:
		//���[���h�I�u�W�F�N�g�̎�O�ɕ\�����邩���ɕ\�����邩
		bool isFrontFont = true;

	private:
		IDWriteTextFormat* pTextFormat = 0;
		ID2D1SolidColorBrush* pBrush;
		D2D1_RECT_F	rect;

		const float reciprocal = 1.0f / 255.0f; //�F�ϊ��p

		//���f�[�^
		std::wstring fontStyle;
		float size;
		float posX, posY;
		float rectL, rectR, rectT, rectB;
		DWORD color;
		eTextAlignment alignment;
		WCHAR* ptr;
		UINT32 count;

		//�Ă΂ꂽ������̓o�^
		void registerString(const WCHAR* string, UINT32 count);
		//�t�H���g�̍쐬
		bool Create(const WCHAR* fontname, float size);
		//�e�L�X�g�̕\����ʒu�̓K�p
		void AdjustTextAlignment();

	public:
		//������
		static bool Initialize(void* hdl);
		//�j��
		static void Destroy(void);

		Font();
		~Font();

		//�R���|�[�l���g����
		void Execute() override;

		//�����̏o��
		void RenderString();

		//�o�͕����̓o�^
		void Print(const WCHAR* string, ...);
		void Print(float left, float top, const WCHAR* string, ...);
		void Print(float left, float top, eTextAlignment alignment, const WCHAR* string, ...);

		//�`��
		void Render();

		//Setter,Getter
		void SetRect();
		void SetRect(float left, float top, float right, float bottom);
		void SetRectWH(float x, float y, float width, float height);
		void SetColor(DWORD color);
		void SetFontStyle(float size, const WCHAR* fontname);
		void SetFontSize(float size);
		void SetTextAlignment(eTextAlignment alignment);
		int GetRenderPriority() override;
		float GetTextLength();
	};
}