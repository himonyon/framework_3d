#pragma once
/*-----------------------------------------------------------

	MeshRenderer�R���|�[�l���g
		���b�V����`�悷��

-------------------------------------------------------------*/

namespace MyFrameWork {
	class MeshRenderer : public Renderer3D {
	public:
		//�`��Ώۃ��b�V��
		noDel_ptr<Mesh> pRenderMesh = NULL;

	private:
		//�J���[
		stColor4 col; 

	public:
		MeshRenderer();
		~MeshRenderer(void);

		//�R���|�[�l���g�̏�����
		void SetUpMeshRenderer(noDel_ptr<Mesh> mesh);

		//�R���|�[�l���g����
		void Execute() override;

	private:
		//�`��
		void Render() override;

	};
}