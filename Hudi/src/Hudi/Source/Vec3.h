#pragma once
#include "Vec2.h"

namespace Hudi {

	struct Vec3
	{
		float x, y, z;

	public:
		Vec3()
			: x(0.0f), y(0.0f), z(0.0f) {}
		Vec3(float x, float y, float z)
			: x(x), y(y), z(z) {}
		Vec3(float x, float y)
			: x(x), y(y), z(0) {}
		Vec3(float val)
			: x(val), y(val), z(val) {}

		static Vec3 Zero() { return Vec3(0.0f, 0.0f, 0.0f); }
		static Vec3 One() { return Vec3(1.0f, 1.0f, 1.0f); }

		float Length() const { return sqrt(x * x + y * y + z * z); }

		Vec3 UnitVec3() const
		{
			float len = Length();
			if (len == 0.0f)
				len = 1.0f;

			return Vec3(x / len, y / len, z / len);
		}

	public:
		Vec3 operator+ (const Vec3& v1) const
		{
			Vec3 v;
			v.x = this->x + v1.x;
			v.y = this->y + v1.y;
			v.z = this->z + v1.z;
			return v;
		}
		Vec3& operator+= (const Vec3& v1)
		{
			*this = *this + v1;
			return *this;
		}

		Vec3 operator- (const Vec3& v1) const
		{
			Vec3 v;
			v.x = this->x - v1.x;
			v.y = this->y - v1.y;
			v.z = this->z - v1.z;
			return v;
		}
		Vec3& operator-= (const Vec3& v1)
		{
			*this = *this + v1;
			return *this;
		}

		Vec3 operator* (const int& mul) const
		{
			Vec3 v;
			v.x = this->x * mul;
			v.y = this->y * mul;
			v.z = this->z * mul;
			return v;
		}
		Vec3 operator* (const float& mul) const
		{
			Vec3 v;
			v.x = this->x * mul;
			v.y = this->y * mul;
			v.z = this->z * mul;
			return v;
		}
		Vec3& operator*= (const int& mul)
		{
			*this = *this * mul;
			return *this;
		}
		Vec3& operator*= (const float& mul) 
		{
			*this = *this * mul;
			return *this;
		}

		Vec3 operator/ (const int& divider) const
		{
			Vec3 v;
			v.x = x / divider;
			v.y = y / divider;
			v.z = z / divider;
			return v;
		}
		Vec3 operator/ (const float& divider) const
		{
			Vec3 v;
			v.x = x / divider;
			v.y = y / divider;
			v.z = z / divider;
			return v;
		}
		Vec3& operator/= (const int& divider) 
		{
			*this = *this / divider;
			return *this;
		}
		Vec3& operator/= (const float& divider) 
		{
			*this = *this / divider;
			return *this;
		}

		Vec3& operator= (float x)
		{
			*this = Vec3(x, x, x);
			return *this;
		}
	};

	inline std::ostream& operator<<(std::ostream& os, const Vec3& v)
	{
		std::string s;
		s = "[" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + "]";
		return os << s;
	}
}