#pragma once

namespace Hudi {

	template <typename T>
	struct Vec2
	{
		T x;
		T y;

	public:
		Vec2()
			: x(0), y(0) {}
		Vec2(T x, T y)
			: x(x), y(y) {}

		Vec2 Zero() { return Vec2((T)0, (T)0); }

		float length() const { return (float)sqrt(x * x + y * y); }

		Vec2 unitVec2() const 
		{
			float len = length();
			if (len == 0.f)
				len = 1.f;

			return Vec2(x / len, y / len);
		}

	public:
		Vec2 operator+ (const Vec2& v1) const
		{
			Vec2 v;
			v.x = this->x + v1.x;
			v.y = this->y + v1.y;
			return v;
		}
		Vec2& operator+= (const Vec2& v1) 
		{
			*this = *this + v1;
			return *this;
		}

		Vec2 operator- (const Vec2& v1) const
		{
			Vec2 v;
			v.x = this->x - v1.x;
			v.y = this->y - v1.y;
			return v;
		}
		Vec2& operator-= (const Vec2& v1)
		{
			*this = *this + v1;
			return *this;
		}

		Vec2 operator* (const int& mul) const 
		{
			Vec2 v;
			v.x = this->x * mul;
			v.y = this->y * mul;
			return v;
		}
		Vec2 operator* (const float& mul) const
		{
			Vec2 v;
			v.x = this->x * mul;
			v.y = this->y * mul;
			return v;
		}
		Vec2& operator*= (const int& mul) {
			*this = *this * mul;
			return *this;
		}
		Vec2& operator*= (const float& mul) {
			*this = *this * mul;
			return *this;
		}

		Vec2 operator/ (const int& divider) const
		{
			Vec2 v;
			v.x = x / divider;
			v.y = y / divider;
			return v;
		}
		Vec2 operator/ (const float& divider) const
		{
			Vec2 v;
			v.x = x / divider;
			v.y = y / divider;
			return v;
		}
		Vec2& operator/= (const int& divider) {
			*this = *this / divider;
			return *this;
		}
		Vec2& operator/= (const float& divider) {
			*this = *this / divider;
			return *this;
		}

		operator Vec2<int>() const {
			return Vec2<int>((int)x, (int)y);
		}

		Vec2& operator= (float other)
		{
			this->x = static_cast<T>(other.x)
		}
	};

	template <typename T>
	inline std::ostream& operator<<(std::ostream& os, const Vec2<T>& v)
	{
		std::string s;
		s = "[" + std::to_string(v.x) + ", " + std::to_string(v.y) + "]";
		return os << s;
	}

}