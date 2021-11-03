#include "../../framework.h"
#include "../../environment.h"


//�R���X�g���N�^
Scene::Scene()
{
	for (int i = 0; i < MaxSprites; i++) {
		pObject2D_array[i] = NULL;
	}
}

//�f�X�g���N�^
Scene::~Scene()
{
	for (int i = 0; i < MaxSprites; i++) {
		if (pObject2D_array[i] == NULL) continue;
		delete pObject2D_array[i];
		pObject2D_array[i] = NULL;
	}

	for (int i = 0; i < pSpriteAnimation_array.size(); i++) {
		delete pSpriteAnimation_array[i];
		pSpriteAnimation_array[i] = NULL;
		pSpriteAnimation_array.erase(pSpriteAnimation_array.begin() + i);
	}
}

//����
void Scene::Execute()
{
	//�摜�̏���
	bool isSorted = false; //���łɃ\�[�g���ꂽ��
	for (int i = 0; i < MaxSprites; i++) {
		if (pObject2D_array[i] == NULL) continue;
		if (pObject2D_array[i]->isExecuteEnable() == false) continue;
		pObject2D_array[i]->Execute();

		//�`�揇���ύX���ꂽ�ꍇ�\�[�g
		if (pObject2D_array[i]->isSortEnable()) {
			if (isSorted) pObject2D_array[i]->SetSortEnable(false);
			else {
				pObject2D_array[i]->SetSortEnable(false);
				RenderOrderSort(0, MaxSprites - 1);
				isSorted = true;
			}
		}
	}

	//�A�j���[�V��������
	for (int i = 0; i < pSpriteAnimation_array.size(); i++) {
		if (pSpriteAnimation_array[i]->isAnimEnable()) {
			pSpriteAnimation_array[i]->AnimOn();
		}
	}
}

void Scene::Render()
{
	for (int i = 0; i < MaxSprites; i++) {
		if (pObject2D_array[i] == NULL) return;
		if (pObject2D_array[i]->isRenderEnable() == false) continue;
		//�e�̃����_�[��Ԃ��m�F
		if (pObject2D_array[i]->pParent != NULL && pObject2D_array[i]->pParent->isRenderEnable() == false) continue;
		pObject2D_array[i]->Render();
	}
}


bool Scene::RegisterObject(Sprite* p) {
	if (p == NULL) return false;

	for (int i = 0; i < MaxSprites; i++) {
		if (pObject2D_array[i] != NULL) continue;
		pObject2D_array[i] = p;
		return true;
	}

	return false;
}

void Scene::DeleteObject(Sprite* p) {
	if (p == NULL) return;

	for (int i = 0; i < MaxSprites; i++) {
		if (pObject2D_array[i] == NULL) continue;
		if (pObject2D_array[i] == p) {
			delete pObject2D_array[i];
			pObject2D_array[i] = NULL;
			return;
		}
	}
}

void Scene::RenderOrderSort(int start, int end) {
	int left = start;
	int right = end;

	while (pObject2D_array[left] == NULL) {
		left++;
	}
	if (right <= left) return;

	int pivot = pObject2D_array[left]->GetRenderPriority();

	while (true) {
		while (pObject2D_array[left] == NULL) left++;
		while (pObject2D_array[right] == NULL) right--;
		while (pObject2D_array[left]->GetRenderPriority() < pivot) left++;
		while (pObject2D_array[right]->GetRenderPriority() > pivot) right--;

		if (left < right) {
			Sprite* sprite = pObject2D_array[left];
			pObject2D_array[left] = pObject2D_array[right];
			pObject2D_array[right] = sprite;

			left++;
			right--;
		}
		else {
			break;
		}
	}

	// �����ċA
	RenderOrderSort(start, left - 1);
	// �E���ċA
	RenderOrderSort(right + 1, end);
}

bool Scene::RegisterAnimation(SpriteAnimation* anim) {
	if (anim == NULL) return false;

	if (pSpriteAnimation_array.size() >= MaxAnimation) return false;
	pSpriteAnimation_array.emplace_back(anim);
	return true;
}


bool Scene::isCollider(Sprite* p, float x, float y) {
	if (x > p->vtx[0].x && x < p->vtx[1].x && y > p->vtx[0].y && y < p->vtx[2].y) {
		return true;
	}

	return false;
}
bool Scene::isCollider(Sprite* p, int x, int y) {
	if (x > p->vtx[0].x && x < p->vtx[1].x && y > p->vtx[0].y && y < p->vtx[2].y) {
		return true;
	}

	return false;
}

