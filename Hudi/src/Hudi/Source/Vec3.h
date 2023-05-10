#pragma once
#include "Vec2.h"

#include <glm/glm.hpp>

namespace Hudi {

	struct Vec3
	{
		float x, y, z;

	public:
		Vec3();
		Vec3(float x, float y, float z);
		Vec3(float x, float y);
		Vec3(float val);
		Vec3(const glm::vec3& vec);

		static Vec3 Zero();
		static Vec3 One();

		float Length() const;

		Vec3 Nomalized() const;
		
	public:
		operator glm::vec3() const;

		Vec3 operator+ (const Vec3& v1) const;
		Vec3& operator+= (const Vec3& v1);

		Vec3 operator- (const Vec3& v1) const;
		Vec3& operator-= (const Vec3& v1);

		Vec3 operator* (const int& mul) const;
		Vec3 operator* (const float& mul) const;
		Vec3& operator*= (const int& mul);
		Vec3& operator*= (const float& mul);

		Vec3 operator/ (const int& divider) const;
		Vec3 operator/ (const float& divider) const;
		Vec3& operator/= (const int& divider);
		Vec3& operator/= (const float& divider);
	};


	// ---------------------- DEFINITIONS -------------------------

	inline Vec3::Vec3()
		: x(0.0f), y(0.0f), z(0.0f) {}
	inline Vec3::Vec3(float x, float y, float z)
		: x(x), y(y), z(z) {}
	inline Vec3::Vec3(float x, float y)
		: x(x), y(y), z(0) {}
	inline Vec3::Vec3(float val)
		: x(val), y(val), z(val) {}
	inline Vec3::Vec3(const glm::vec3& vec)
		: x(vec.x), y(vec.y), z(vec.z) {}

	inline Vec3 Vec3::Zero() { return Vec3(0.0f, 0.0f, 0.0f); }
	inline Vec3 Vec3::One() { return Vec3(1.0f, 1.0f, 1.0f); }

	inline float Vec3::Length() const { return sqrt(x * x + y * y + z * z); }

	inline Vec3 Vec3::Nomalized() const
	{
		float len = Length();
		if (len == 0.0f)
			len = 1.0f;

		return Vec3(x / len, y / len, z / len);
	}

	inline Vec3::operator glm::vec3() const
	{
		return glm::vec3(x, y, z);
	}

	inline Vec3 Vec3::operator+ (const Vec3& v1) const
	{
		Vec3 v;
		v.x = this->x + v1.x;
		v.y = this->y + v1.y;
		v.z = this->z + v1.z;
		return v;
	}
	inline Vec3& Vec3::operator+= (const Vec3& v1)
	{
		*this = *this + v1;
		return *this;
	}

	inline Vec3 Vec3::operator- (const Vec3& v1) const
	{
		Vec3 v;
		v.x = this->x - v1.x;
		v.y = this->y - v1.y;
		v.z = this->z - v1.z;
		return v;
	}
	inline Vec3& Vec3::operator-= (const Vec3& v1)
	{
		*this = *this + v1;
		return *this;
	}

	inline Vec3 Vec3::operator* (const int& mul) const
	{
		Vec3 v;
		v.x = this->x * mul;
		v.y = this->y * mul;
		v.z = this->z * mul;
		return v;
	}
	inline Vec3 Vec3::operator* (const float& mul) const
	{
		Vec3 v;
		v.x = this->x * mul;
		v.y = this->y * mul;
		v.z = this->z * mul;
		return v;
	}
	inline Vec3& Vec3::operator*= (const int& mul)
	{
		*this = *this * mul;
		return *this;
	}
	inline Vec3& Vec3::operator*= (const float& mul)
	{
		*this = *this * mul;
		return *this;
	}

	inline Vec3 Vec3::operator/ (const int& divider) const
	{
		Vec3 v;
		v.x = x / divider;
		v.y = y / divider;
		v.z = z / divider;
		return v;
	}
	inline Vec3 Vec3::operator/ (const float& divider) const
	{
		Vec3 v;
		v.x = x / divider;
		v.y = y / divider;
		v.z = z / divider;
		return v;
	}
	inline Vec3& Vec3::operator/= (const int& divider)
	{
		*this = *this / divider;
		return *this;
	}
	inline Vec3& Vec3::operator/= (const float& divider)
	{
		*this = *this / divider;
		return *this;
	}

	inline std::ostream& operator<<(std::ostream& os, const Vec3& v)
	{
		std::string s;
		s = "[" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + "]";
		return os << s;
	}
}