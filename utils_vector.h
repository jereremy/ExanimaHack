#ifndef U_VEC
#define U_VEC

class Vector3
{
public:
	float x, y, z;
	std::string ToString()
	{
		return std::format("({}, {}, {})", x, y, z);
	}
};
#endif