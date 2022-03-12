#pragma once
/*-----------------------------------------------------------

	MeshRenderer�R���|�[�l���g
		���b�V����`�悷��

-------------------------------------------------------------*/

namespace MyFrameWork {
	class MeshRenderer : public Component {
	public:
		//�`��Ώۃ��b�V��
		noDel_ptr<Mesh> pRenderMesh = NULL;

	private:
		static ID3D11InputLayout* pInputLayout;
		static ID3D11RasterizerState* pRasterizerState;
		static ID3D11BlendState* pBlendState;
		static ID3D11DepthStencilState* pDepthStencilState;
		static ID3D11Buffer* pConstantBuffer;
		static ID3D11SamplerState* pSampleLinear;

		//�R���X�^���g�o�b�t�@�i�[�p
		static stCBuffer3D inputCB;

		//�J���[
		stColor4 col; 

	public:
		//������
		static bool Initialize();
		//�j��
		static void Destroy();

		MeshRenderer();
		~MeshRenderer(void);

		//�R���|�[�l���g�̏�����
		void SetUpMeshRenderer(noDel_ptr<Mesh> mesh);

		//�R���|�[�l���g����
		void Execute() override;

		//Getter,Setter
		static ID3D11InputLayout* GetInputLayout() { return pInputLayout; }
		static ID3D11RasterizerState* GetRasterizerState() { return pRasterizerState; }
		static ID3D11BlendState* GetBlendState() { return pBlendState; }
		static ID3D11DepthStencilState* GetDepthStencilState() { return pDepthStencilState; }
		static ID3D11Buffer* GetConstantBuffer() { return pConstantBuffer; }
		static ID3D11SamplerState** GetSampleLinear() { return &pSampleLinear; }
		static stCBuffer3D& GetInputCB() { return inputCB; }

	private:
		//�`��
		void Render(void);

		//���W�A��]�A�X�P�[���̕`����W(�J�����K�p)
		stVector3 GetPosOnCam();
		stVector3 GetRotOnCam();
		stVector3 GetScaleOnCam();
	};
}