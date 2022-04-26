/*-----------------------------------------------------------

	Mesh�N���X
		���b�V���{��

-------------------------------------------------------------*/
#pragma once

namespace MyFrameWork {
	class Mesh {
	private:
		noDel_ptr<Mesh> pParent; //�e���b�V��
		std::vector<noDel_ptr<Mesh>> vChildren; //�q���b�V��

	protected:
		//���b�V���f�[�^
		stMeshData* meshData = NULL;

		//�}�e���A��
		noDel_ptr<stMaterial> pMaterial = NULL;

	public:
		Mesh();
		virtual ~Mesh();

		virtual void Render(stVector3 pos, stVector3 rot, stVector3 scl) = 0;

		//Setter,Getter
		void SetMaterial(noDel_ptr<stMaterial> mat) { pMaterial = mat; };
		noDel_ptr<stMaterial> GetMaterial() { return noDel_ptr<stMaterial>(pMaterial); }

		void SetParent(noDel_ptr<Mesh> parent);
		noDel_ptr<Mesh> GetParent();
		noDel_ptr<Mesh> GetChild(int index);
		int GetChildCount();
	};
}