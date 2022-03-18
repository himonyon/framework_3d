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


stVector3 RoundDec3forVector(stVector3 vec) {
	vec.x = round(vec.x * 1000) / 1000;
	vec.y = round(vec.y * 1000) / 1000;
	vec.z = round(vec.z * 1000) / 1000;
	return vec;
}
float RoundDec3forFloat(float val) {
	val = round(val * 1000) / 1000;
	return val;
}

void Split(char split_char, char* buffer, std::vector<std::string>& out)
{
	int count = 0;
	if (buffer == nullptr)
	{
		return;
	}

	int start_point = 0;

	while (buffer[count] != '\0')
	{
		if (buffer[count] == split_char)
		{
			if (start_point != count)
			{
				char split_str[256] = { 0 };
				strncpy_s(split_str, 256, &buffer[start_point], count - start_point);
				out.emplace_back(split_str);
			}
			else
			{
				out.emplace_back("");
			}
			start_point = count + 1;
		}
		count++;
	}

	if (start_point != count)
	{
		char split_str[256] = { 0 };
		strncpy_s(split_str, 256, &buffer[start_point], count - start_point);
		out.emplace_back(split_str);
	}
}

void Replace(char search_char, char replace_char, char* buffer)
{
	int len = (int)strlen(buffer);

	for (int i = 0; i < len; i++)
	{
		if (buffer[i] == search_char)
		{
			buffer[i] = replace_char;
		}
	}
}


