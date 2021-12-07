#include "../../../framework.h"
#include "../../../environment.h"

int Object::max_instance = 0;

Object::Object() {
	instanceID = max_instance;
	max_instance++;
}