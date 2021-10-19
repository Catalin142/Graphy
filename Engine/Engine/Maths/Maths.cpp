#include "Core/GrPch.h"
#include "Maths.h"

vec2 operator+(const vec2& left, const vec2& right)
{
	return vec2(left.x + right.x, left.y + right.y);
}

vec2 operator-(const vec2& left, const vec2& right)
{
	return vec2(left.x - right.x, left.y - right.y);
}

vec2 operator*(const vec2& left, const vec2& right)
{
	return vec2(left.x * right.x, left.y * right.y);
}

vec2 operator/(const vec2& left, const vec2& right)
{
	return vec2(left.x / right.x, left.y / right.y);
}

vec2 operator+(const vec2& left, float scalar)
{
	return vec2(left.x + scalar, left.y + scalar);
}

vec2 operator*(const vec2& left, float scalar)
{
	return vec2(left.x * scalar, left.y * scalar);
}

vec2 operator/(const vec2& left, float scalar)
{
	return vec2(left.x / scalar, left.y / scalar);
}

vec3 operator+(const vec3& left, const vec3& right)
{
	return vec3(left.x + right.x, left.y + right.y, left.z + right.z);
}

vec3 operator-(const vec3& left, const vec3& right)
{
	return vec3(left.x - right.x, left.y - right.y, left.z - right.z);
}

vec3 operator*(const vec3& left, const vec3& right)
{
	return vec3(left.x * right.x, left.y * right.y, left.z * right.z);
}

vec3 operator/(const vec3& left, const vec3& right)
{
	return vec3(left.x / right.x, left.y / right.y, left.z / right.z);
}

vec3 operator*(const vec3& left, float scalar)
{
	return vec3(left.x * scalar, left.y * scalar, left.z * scalar);
}

vec3 operator/(const vec3& left, float scalar)
{
	return vec3(left.x / scalar, left.y / scalar, left.z / scalar);
}

vec2 vec2::normalize()
{
	float n = x * x + y * y;
	float invSqrt = 1.0f / sqrt(n);
	x *= invSqrt;
	y *= invSqrt;

	return *this;
}

float vec2::magnitude()
{
	float n = x * x + y * y;
	return sqrt(n);
}

bool vec2::operator==(const vec2& other) const
{
	return (x == other.x && y == other.y);
}

vec3 vec3::normalize()
{
	float n = x * x + y * y + z * z;
	float invSqrt = 1.0f / sqrt(n);
	x *= invSqrt;
	y *= invSqrt;
	z *= invSqrt;

	return *this;
}

float vec3::magnitude()
{
	float n = x * x + y * y + z * z;
	return sqrt(n);
}

bool vec3::operator==(const vec3& other) const
{
	return (x == other.x && y == other.y && z == other.z);
}