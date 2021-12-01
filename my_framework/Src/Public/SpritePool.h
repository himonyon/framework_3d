#pragma once

/*-----------------------------------------------------------

	全シーン共通で利用するスプライトインスタンス

-------------------------------------------------------------*/

class SpritePool {
public:
	static void Initialize();
public:
	static noDel_ptr<Sprite> pSp0;
	static noDel_ptr<Sprite> pSp1;
};