#include "../../framework.h"
#include "../../environment.h"

void CreateSpriteFile(const WCHAR* texture_file, float left, float right, float top, float bottom) {

	WCHAR _filename[256] = L"";
	int _size = (int)wcslen(texture_file);
	int _nameSize = 0;
	for (int i = 0; i < _size; i++) {
		if (texture_file[i] == L'.') break;
		_nameSize++;
	}
	for (int i = 0; i < _nameSize; i++) {
		_filename[i] = texture_file[i];
	}

	wcscat_s(_filename, L".spr");

	FILE* fp = NULL;
	_wfopen_s(&fp, _filename, L"w");
	if (fp == NULL) {
		return;
	}
	fwprintf_s(fp, L"texture %s\n", texture_file);
	fwprintf_s(fp, L"name default uv %f %f %f %f", left, right, top, bottom);

	fclose(fp);
}
void AddSpriteFile(const WCHAR* texture_file, const WCHAR* sprite_name, float left, float right, float top, float bottom) {

	WCHAR _filename[256] = L"";
	int _size = (int)wcslen(texture_file);
	int _nameSize = 0;
	for (int i = 0; i < _size; i++) {
		if (texture_file[i] == L'.') break;
		_nameSize++;
	}
	for (int i = 0; i < _nameSize; i++) {
		_filename[i] = texture_file[i];
	}

	wcscat_s(_filename, L".spr");
	FILE* fp = NULL;
	_wfopen_s(&fp, _filename, L"a");
	if (fp == NULL) {
		return;
	}
	fwprintf_s(fp, L"\nname %s uv %f %f %f %f", sprite_name, left, right, top, bottom);

	fclose(fp);
}