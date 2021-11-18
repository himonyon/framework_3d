#pragma once

//floatŒ^‚ğ‚R‚Â‚à‚Â\‘¢‘Ì
struct stVector3 {
public:
	const stVector3 operator -(stVector3& vec) {
		stVector3 temp;
		temp.x = x - vec.x;
		temp.y = y - vec.y;
		temp.z = z - vec.z;
		return temp;
	}
	const stVector3 operator +(stVector3& vec) {
		stVector3 temp;
		temp.x = x + vec.x;
		temp.y = y + vec.y;
		temp.z = z + vec.z;
		return temp;
	}
	const stVector3 operator +=(stVector3& vec) {
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}
	const bool operator !=(int num) {
		if (x != num || y != num || z != num) return true;
		return false;
	}

	float x;
	float y;
	float z;
};