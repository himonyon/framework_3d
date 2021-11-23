#include "../../framework.h"
#include "../../environment.h"
#include "UtilFunc.h"

unsigned int ColorConvert16(stColor4 color) {
	unsigned int r = unsigned int((int(color.r * 255)) * (256 * 256));
	unsigned int g = unsigned int((int(color.g * 255)) * (256));
	unsigned int b = unsigned int(int(color.b * 255));
	unsigned int s = r + g + b + unsigned int(0xff000000);
	return s;
 }

