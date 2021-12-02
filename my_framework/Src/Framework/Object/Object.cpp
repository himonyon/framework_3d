#include "../../../framework.h"
#include "../../../environment.h"

int Object::max_instance = 0;

Object::Object() {
}
Object::Object(int id) {
	objectID = max_instance;
	max_instance++;
}