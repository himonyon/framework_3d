#include "../../../../framework.h"
#include "../../../../environment.h"

using namespace MyFrameWork;

noDel_ptr<Camera> Camera::main = 0;

Camera::Camera() : Component(eComponentType::Camera) {
}