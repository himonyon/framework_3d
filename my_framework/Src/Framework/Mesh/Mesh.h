/*-----------------------------------------------------------

	Meshクラス
		メッシュ本体

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

		noDel_ptr<Mesh> pParent; //親メッシュ
		std::vector<noDel_ptr<Mesh>> vChildren; //子メッシュ

	protected:
		//メッシュデータ
		stMeshData* meshData = NULL;

		//マテリアル
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