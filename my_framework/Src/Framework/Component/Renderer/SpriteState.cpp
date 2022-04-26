#include "../../../../framework.h"
#include "../../../../environment.h"

using namespace MyFrameWork;

SpriteState::SpriteState(eComponentType type) : Renderer(type)
{
}

SpriteState::~SpriteState() {}

//setter/getter
void SpriteState::SetSize(float width, float height) {
	size.x = width;
	size.y = height;
}