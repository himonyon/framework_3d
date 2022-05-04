/*-----------------------------------------------------------

	Mesh�N���X
		���b�V���{��

-------------------------------------------------------------*/
#pragma once

namespace MyFrameWork {
	enum class eMeshFormat {
		Obj,
		Fbx,
	};

	class Mesh {
	private:
		eMeshFormat format;

		noDel_ptr<Mesh> pParent; //�e���b�V��
		std::vector<noDel_ptr<Mesh>> vChildren; //�q���b�V��

	protected:
		//���b�V���f�[�^
		stMeshData* meshData = NULL;

		//�}�e���A��
		noDel_ptr<stMaterial> pMaterial = NULL;

		stVector3 initPos = { 0,0,0 };
		stVector3 initRot = { 0,0,0 };
		stVector3 initScl = { 0,0,0 };

	public:
		Mesh(eMeshFormat format);
		virtual ~Mesh();

		//Setter,Getter
		noDel_ptr<stMeshData> GetMeshData() { return noDel_ptr<stMeshData>(meshData); }

		void SetMaterial(noDel_ptr<stMaterial> mat) { pMaterial = mat; };
		noDel_ptr<stMaterial> GetMaterial() { return noDel_ptr<stMaterial>(pMaterial); }

		void SetParent(noDel_ptr<Mesh> parent);
		noDel_ptr<Mesh> GetParent();
		noDel_ptr<Mesh> GetChild(int index);
		int GetChildCount();

		eMeshFormat GetMeshFormat() { return format; }

		stVector3 GetInitPos() { return initPos; }
		stVector3 GetInitRot() { return initRot; }
		stVector3 GetInitScl() { return initScl; }
	};
}